#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <unistd.h>		// fcntl(2)
#include <fcntl.h>		// fcntl(2)
#include <sys/types.h>		// getsockopt(3)
#include <sys/socket.h>		// getsockopt(3)
#include <string.h>		// strerror(3)
#include <errno.h>		// errno(3)

#include "Prelude.h"
#include "Logger.h"
#include "EventHandler.h"
#include "Reactor.h"
#include "TimeVal.h"
#include "Address.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file Connector.h
	��װ��TCP/IP����������������Ϊ
*/

/*! \class Connector
 *	��װ��TCP/IP����������������Ϊ����������

	SERVICE_HANDLER��ServiceHandler��������

	PEER_CONNECTOR��Socket��������.
*/
template<class SERVICE_HANDLER, class PEER_CONNECTOR>
class Connector : public virtual EventHandler
{
public:
	/*! \enum ConnectMode.
		\brief ����ģʽ
	*/
	enum ConnectMode { 
		sync,					/*!< ͬ��ģʽ */
		async					/*!< �첽ģʽ. */
	};

	/*! \brief ȱʡ���캯��
	 */
	Connector ();

	/*! \brief �������� 
	 */
	virtual ~Connector ();
	
	/*! \brief ��ʼ��������.

	    @param tv_ ��ʱʱ��(ȱʡ��5.0��)
	    @param mode_ ����ģʽ
	    @param r_ ���ķ�����(�첽ģʽ).
	    @return 0 on success, -1 on error.
	*/
	virtual int open (const TimeVal& tv_ = TimeVal (5.0), 
					  ConnectMode mode_ = sync,
					  Reactor* r_ = (Reactor*)NULL);

	/*! \brief �ر�����
	    @return 0 on success, -1 on error.
	 */
	virtual int close (void);

	/*! \brief �����˽������ӵĲ���.
	 * ȱʡ����ͬ����ʽ��������
	 * �첽ģʽ�£�����Connector::connect()�������ء�
	 * ȱʡ��ʱ��5�룬��ͨ��Connector::open()��������
	 *
	    @param sh_ ServiceHandler����ָ��
	    @param addr_ ���ӵ�ַ
	    @param protocol_ internet�׽ӿ���AF_INET,UNIX���׽ӿ���AF_UNIX.(ȱʡ��AF_INET)
	    
	    @return 0 on success, -1 on error.
	*/
	virtual int connect (SERVICE_HANDLER* sh_, 
						 Address& addr_, 
						 int protocol_ = AF_INET);

	virtual int handle_write (int fd);

	/*! \brief ���ӳ�ʱ������
	 */
	virtual int handle_timeout (TimerId tid);

protected:
	/*! \enum ProgressState.
	 * \brief ����״̬
	*/
	enum ProgressState { 
		idle,		/*!< ��ɳ�ʼ��.  */
		waiting,   /*!< �첽�ȴ�������� */
		conned,		/*!< ����������.  */
		failed		/*!< ��������ʧ�� */
	};

	/*! \brief ����ServiceHandler�Ĵ�������
	 * ȱʡ�Ƕ�̬����SERVICE_HANDLER, ���sh_Ϊ0
	    @param  sh_ SERVICE_HANDLERָ�룬���ΪNULL, ��̬����
	    @return ���ش�����SERVICE_HANDLERָ��
	*/
	virtual SERVICE_HANDLER* makeServiceHandler (SERVICE_HANDLER* sh_);

	/*! \brief �޳�ʱ����ͬ�����ӵ�ȱʡ����
	 * �������ͨ�������޸������
	    @return 0 on success, -1 on error.
	 */
	virtual int connectServiceHandler (Address& addr, int protocol);

	virtual int activateServiceHandler ();

protected:
	/*!  \brief ��ų�ʱ
	 */
	TimeVal m_timeout;

	/*! \brief ��ʱ��id
	 */
	TimerId m_tid;

	/*! \brief ���������õ����ķ�����(�첽)
	 */
	Reactor* m_reactor;
	
	/*! \brief ���ӹ��̵�״̬
	 */
	ProgressState m_state;

	/*! \brief  Socketѡ��
	 */
	int m_flags;

	SERVICE_HANDLER* m_sh;

	/*! \brief Socket���ļ�������
	 */
	int m_fd;

	/*! \brief ����ģʽ(ͬ��/�첽)
	 */
	ConnectMode m_mode;

private:
	void doAsync (void);

	int doSync (void);
};

// Convenience definitions

#define SH SERVICE_HANDLER
#define PC PEER_CONNECTOR

//------------------------------------------------------------------------------
// Template member functions definitions
//------------------------------------------------------------------------------

template<class SH, class PC> 
Connector<SH, PC>::Connector () 
	: m_tid (0), m_reactor (0), m_state (idle),
	  m_flags (0), m_sh ((SERVICE_HANDLER*)NULL), m_fd (-1), m_mode (sync)
{
	trace_with_mask("Connector::Connector",SOCKTRACE);
}

template<class SH, class PC> 
Connector<SH, PC>::~Connector () 
{
	trace_with_mask("Connector::~Connector",SOCKTRACE);
	// If I created SERVICE_HANDLER, probably I should delete it?
}

template<class SH, class PC> int
Connector<SH, PC>::open (const TimeVal& tv_, ConnectMode mode_, Reactor* r_) 
{
	trace_with_mask("Connector::open", SOCKTRACE);

	m_timeout = tv_;
	if (async == mode_ && (Reactor*) NULL == r_) 
		return -1;
	m_mode = mode_;
	m_reactor = r_;
	return 0;
}

template<class SH, class PC> int
Connector<SH, PC>::close () 
{
	trace_with_mask("Connector::close",SOCKTRACE);
	return 0;
}

template<class SH, class PC> int
Connector<SH, PC>::connect (SH* sh_, Address& addr_, int protocol_family_)
{
	/*
	 * We restore socket to its original mode only on 
	 * successful connection. If error occured, client would have
	 * to close socket anyway.
	 *
	 * NOTE: If sh_==0, then result is dangling pointer
	 * new_sh produced ! Destructor should determine whether 
	 * SERVICE_HANDLER has been created dynamically and if so, delete
	 * it.
	 */
	trace_with_mask("Connector::connect",SOCKTRACE);
	errno = 0;

	m_sh = makeServiceHandler (sh_);
	PEER_CONNECTOR& s = *m_sh;

	if (addr_.bad ()) {
		errno = EFAULT;	// Bad address
		EL((ERROR,"Bad address (errno %d)\n", errno));
		return -1;
	}

	if (connectServiceHandler (addr_, protocol_family_) == -1) {

		if (errno == EINPROGRESS) {

			if (async == m_mode) { 
				doAsync (); 
				return 0; 
			}
			return doSync ();
		}
		return -1;
	}
	/* Switch to blocking mode */
	fcntl (s.getHandler (), F_SETFL, m_flags);

	return activateServiceHandler ();
}

template<class SH, class PC> SERVICE_HANDLER*
Connector<SH, PC>::makeServiceHandler (SERVICE_HANDLER* sh_) 
{
	trace_with_mask("Connector::makeServiceHandler",SOCKTRACE);

	SERVICE_HANDLER* new_sh = sh_;

	if (sh_ == 0) {
		new_sh = new SERVICE_HANDLER;
	}
	return new_sh;
}

template<class SH, class PC> int
Connector<SH, PC>::connectServiceHandler (Address& addr_, int protocol_family_)
{
	trace_with_mask("Connector::connectServiceHandler",SOCKTRACE);

	PEER_CONNECTOR& s = *m_sh;
	
	if ( !s.open (protocol_family_) ) {
		EL((ERROR,"Socket::open (protocol=%d) failed\n",
		    protocol_family_));
		return -1;
	}
	// Switch to non-blocking mode

	m_fd = s.getHandler ();
	m_flags = fcntl (m_fd, F_GETFL, 0);
	fcntl (m_fd, F_SETFL, m_flags | O_NONBLOCK);

	return s.connect (addr_) ? 0 : -1;
}

template<class SH, class PC> int
Connector<SH, PC>::activateServiceHandler ()
{
	trace_with_mask("Connector::activateServiceHandler",SOCKTRACE);

	return m_sh->open ();
}

template<class SH, class PC> void
Connector<SH, PC>::doAsync (void)
{
	trace_with_mask("Connector::doAsync",SOCKTRACE);

	/* We are doing async and 3-way handshake is in
	 * progress - hook up with Reactor and wait on timer.
	 * Write event will be our indicator whether connection
	 * was completed or not.
	 */
	m_reactor->registerIOHandler (this, m_fd, WRITE_EVENT);

	m_tid = m_reactor->registerTimerHandler (this, m_timeout);
	m_state = waiting;
}

template<class SH, class PC> int
Connector<SH, PC>::doSync (void)
{
	trace_with_mask("Connector::doSync",SOCKTRACE);

	m_reactor = new Reactor;

	m_reactor->registerIOHandler (this, m_fd, WRITE_EVENT);
	m_reactor->registerTimerHandler (this, m_timeout);
	m_state = waiting;

	m_reactor->waitForEvents (&m_timeout); // Let the ball rolling ...

	m_reactor->removeHandler (this);	// Remove all handlers.
	delete m_reactor;
	m_reactor = 0;

	if (conned == m_state) {
		DL((SOCKTRACE,"Synchronous connect() completed\n"));
		fcntl (m_fd, F_SETFL, m_flags); // Switch to blocking mode
		return 0;
	}
	EL((ERROR,"Synchronous connect() timed out\n"));
	errno = ETIMEDOUT;	// Connection timed out
	return -1;
}

template<class SH, class PC> int
Connector<SH, PC>::handle_write (int fd_)
{
	trace_with_mask("Connector::handle_write",SOCKTRACE);

	/* Precondition 
	 */
	if (fd_ != m_fd) {
		return -1;
	}

	/* This method serves both sync and async modes - thus the
	 * differences. For async we remove Timer here. sync runs
	 * its own private Reactor and handler termination is 
	 * handled in doSync().
	 */

	if (async == m_mode) { // Complete SH activation
		m_reactor->removeTimerHandler (m_tid);
		m_tid = 0;
	}

	/*
	 * Although SUN and Linux man pages on connect(3) claims that
	 * "upon asynchronous establishement of connection, select(3)
	 * will indicate that the file descriptor for the socket is ready
	 * for writing", as discussed in W.S.Stevens "UNIX network
	 * programming", Vol I, 2nd edition, BSD-derived systems also
	 * mark file descriptor both readable and writable when the
	 * connection establishment encouters an error.
	 *
	 * Therefore we need an extra step to find out what really happened.
	 * One way to do so is to look at socket pending errors...
	 */

	int error;
	int ret;
	error = ret = errno = 0;
	size_t n = sizeof (error);

	/* Always remove IO handler first. */
	m_reactor->removeHandler (this, WRITE_EVENT);	

#if defined(linux)
	ret = getsockopt (m_fd, SOL_SOCKET, SO_ERROR, (void*)&error, &n);
#elif defined(solaris)  // Solaris 2.6 
	ret = getsockopt (m_fd, SOL_SOCKET, SO_ERROR, (char*)&error, (int*)&n);
#else //AIX
	ret = getsockopt (m_fd, SOL_SOCKET, SO_ERROR, (char*)&error, (unsigned long*)&n);
#endif

	if (ret == 0) {
		if (error == 0) {
			if (activateServiceHandler () == 0) {
				DL((SOCKTRACE,"Nonblocking connect() completed\n"));
				m_state = conned;
			}
			else {
				DL((SOCKTRACE,"Nonblocking connect() failed\n"));
				m_state = failed;
			}
			return (0);			// return value doesn't really matter
		}
		/* Socket pending error - propagate it via errno. */
		EL((ERROR,"Socket pending error: %d\n",error));
		errno = error;
	}
	else {
		/* Solaris pending error. */
		EL((ERROR,"getsockopt(3) = %d\n", ret));
		EL((ERROR,"Solaris pending error!\n"));
	}
	m_state = failed;

        EL((ERROR,"Nonblocking connect (2) failed\n"));

	if (errno == ECONNREFUSED) {
		EL((ERROR,"Try to compare port "
			 "numbers on client and service hosts.\n"));
	}
        /* This is the only way to tell SH that we failed to connect. 
	 */
	if (async == m_mode) {
		m_sh->close ();	
	}

	/* Don't alter fd mask - SERVICE_HANDLER::open() could have changed 
	 * it already for application processing needs.
	 */
	return 0;
}

template<class SH, class PC> int
Connector<SH, PC>::handle_timeout (TimerId tid_)
{
	trace_with_mask("Connector::handle_timeout",SOCKTRACE);

	m_state = failed;
	errno = ETIMEDOUT;	// Connection timed out

	if (async == m_mode) {
		m_reactor->removeHandler (this, WRITE_EVENT);
	}
	return -1;		// Remove Timer Handler
}

END_SUNRISE_NAMESPACE

#endif /* CONNECTOR_H */  
