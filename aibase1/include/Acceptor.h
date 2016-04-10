//------------------------------------------------------------------------
//                            Acceptor.h
//------------------------------------------------------------------------
#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Prelude.h"
#include "Logger.h"
#include "EventHandler.h"
#include "Address.h"
#include "Reactor.h"
#include "ServiceHandler.h"

/*! \file Acceptor.h

  Acceptor��װ����TCP/IP���������ͨ�ò���.

   This abstract class implementes the generic strategy for passive
   initializing communication services.

   SERVICE_HANDLER is the type of service. It shall be a type derived
   from ServiceHandler interface class.
   
   PEER_ACCEPTOR is the type of concrete Socket class - particular
   transport mechanism used by the Acceptor to passively establish 
   the connection. It should be derived from Socket interface class.

   @see Reactor
   @see ServiceHandler
*/

BEGIN_SUNRISE_NAMESPACE

/*! \class Acceptor
 * \brief TCP/IP�������������.
 * ��װTCP/IP��������, ������ķ���ģʽ. �ڿͻ����������󵽴�ʱ�������ķ�����
 * ע������׽��ֵĶ�/д�¼�������.
 */
template<class SERVICE_HANDLER, class PEER_ACCEPTOR>
class Acceptor : public virtual EventHandler
{
public:
    /*! \brief ȱʡ���캯��
		@param r_ ���ķ�����ʵ��, ͨ���Ǹ�singleton.
    */
    Acceptor (Reactor* r_);

    /*! \brief ��������
    */
    virtual ~Acceptor ();
	
    /*! \brief ��ʼ��ͨѶ��.

		@return �ɹ�����0, ���򷵻�-1.
		ʧ�ܵ�ԭ����PEER_ACCEPTOR��open��������bind�����ĵ���ʧ�ܲ���.
    */
    virtual int open (const Address& local_addr_);

    /*! \brief �ر�PEER_ACCEPTOR��.
		@return �ɹ�����0, ���򷵻�-1.
    */
    virtual int close (void);

    /*! \brief ���µ��������󵽴���ķ��������ص��ĺ���
	  * ȱʡ�Ĳ����ǽ�������.���������ͨ�������޸���Ϊ.

		@return �ɹ�����0,���򷵻�-1.
		����-1ָʹ���ķ��������Լ����¼�����������ɾ���Լ�.
    */
    int handle_read (int fd);

    /*! \brief ��PEER_ACCEPTOR�����ִ���,�������ķ�������ֹ֪ͨͣ�¼���ѯ��
	  * ʱ��ص��������. ȱʡ����Ϊ��ɾ���Լ�.������ö�����reactor������
	  * ͨ��Reactor::removeHandler(this->id()), ɾ���Լ������������ͨ������
	  * �޸���Ϊ.
	  * @return ����������Ƿ���-1.
    */
    virtual int handle_close (int fd);

protected:
    /*! \brief ΪServiceHandler���崴������.
		@return ����SERVICE_HANDLERָ��
    */
    virtual SERVICE_HANDLER* makeServiceHandler (PEER_ACCEPTOR* sock_);

    /*! \brief ���������ӵ�ȱʡ���ԡ�
	  * ���������ͨ�������޸��������.
	
		@param new_socket_ �����´�����PEER_STREAMָ�룬���ʧ�ܷ���0
		@return �ɹ�����0, ʧ�ܷ���-1.
    */
    virtual int acceptServiceHandler (PEER_ACCEPTOR*& new_socket_);

    /*! \brief ���岢�в���.
	  * ȱʡ��Ϊ��ǰ����(�߳�)����SERVICE_HANDLER. ������open������ʹ
	  * ���ķ���������I/O�¼����������ͨ�������޸��������.
		
		@param new_socket_ [����] Ҫ�����PEER_STREAMָ��.
		@return �ɹ�����0, ʧ�ܷ���-1.
    */
    virtual int activateServiceHandler (PEER_ACCEPTOR* new_socket_);

private:
    /*! \brief �ڲ���ͨѶ��.
     */
    PEER_ACCEPTOR m_listenSocket;

    /*! \brief ���ķ�����ʵ��
     */
    Reactor* m_reactor;
};

// Convenience definitions

#define SH SERVICE_HANDLER
#define PA PEER_ACCEPTOR

//------------------------------------------------------------------------------
// Template member functions definitions
//------------------------------------------------------------------------------

template<class SH, class PA>
inline
Acceptor<SH, PA>::Acceptor (Reactor* r_) 
    : m_reactor (r_)
{
    trace("Acceptor::Acceptor");
}

template<class SH, class PA>
inline
Acceptor<SH, PA>::~Acceptor () 
{
    trace("Acceptor::~Acceptor");
}

template<class SH, class PA> 
inline int 
Acceptor<SH, PA>::close (void) 
{
    trace("Acceptor::close");
    m_listenSocket.close ();
    return 0;
}

template<class SH, class PA> 
inline int 
Acceptor<SH, PA>::handle_close (int /* fd */) 
{
    trace("Acceptor::handle_close");
	
    // Reactor::getInstance ()->removeHandler (this->id());
	
    // NOT IMPLEMENTED: This spot requires validation
    // whether Acceptor is created on the heap or
    // automatic memory.
	
    delete this;
    return -1;
}

template<class SH, class PA> 
inline SERVICE_HANDLER* 
Acceptor<SH, PA>::makeServiceHandler (PEER_ACCEPTOR* sock_) 
{
    trace("Acceptor<>::makeServiceHandler");
	
    return new SERVICE_HANDLER (sock_);
}

template<class SH, class PA> 
inline int 
Acceptor<SH, PA>::acceptServiceHandler (PEER_ACCEPTOR*& new_socket_) 
{
    trace("Acceptor::acceptServiceHandler");
	
    new_socket_ = m_listenSocket.accept ();
    return new_socket_ ? 0 : -1;
}

template<class SH, class PA> int 
Acceptor<SH, PA>::activateServiceHandler (PA* new_socket_) 
{
    trace("Acceptor::activateServiceHandler");

    if (!new_socket_) {
		return -1;
	}
    SH* sh = makeServiceHandler (new_socket_);
    sh->open ();
    return 0;
}

template<class SH, class PA> int
Acceptor<SH, PA>::open (const Address& local_addr_)
{
    trace("Acceptor::open");
	
    if ( !m_listenSocket.open (local_addr_.getAddress ()->sa_family) ) {
		return -1;
	}

    if ( !m_listenSocket.bind (local_addr_) ) {
		return -1;
	}

    m_reactor->registerIOHandler (
		this, m_listenSocket.getHandler (), READ_EVENT);
	
    DL((TRACE,"Acceptor on FD - %d\n", m_listenSocket.getHandler ()));
    return 0;
}

//------------------------------------------------------------------------------
// Accept all connections waiting in listen queue at once. This avoids going 
// through Reactor's event loop for each new connection.
//------------------------------------------------------------------------------

template <class SH, class PA> int
Acceptor<SH, PA>::handle_read (int fd_)
{
    trace("Acceptor<>::handle_read");

    int fd = m_listenSocket.getHandler ();

    if (fd != fd_) return -1;

    FdSet mask;
    timeval poll = {0, 0};
    PA* new_socket;

    do {
		if ( acceptServiceHandler (new_socket) == -1 ) {
			return -1;
		}
		if ( !activateServiceHandler (new_socket) == -1 ) {
			return -1;
		}
		mask.reset ();
		mask.setFd (fd);
    }
    while ( (::select (fd+1, &mask, NULL, NULL, &poll) == 1) );

    return 0;
}

END_SUNRISE_NAMESPACE

#endif /* ACCEPTOR_H */  
