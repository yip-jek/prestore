#ifndef _Handlers_h
#define _Handlers_h

#include <sys/types.h>
#include <sys/wait.h>

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif
#include "EventHandler.h"
#include "Assert.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file Handlers.h
	 ���������źŵ��źŴ�������
 */

/*! \class SIGINTHandler Handlers.h
 * \brief SIGINT�źŴ�������
*/
class SIGINTHandler : public EventHandler
{
public:
	/*! \brief ���캯��
	 */
	SIGINTHandler();

	/*! \brief SIGINT�źŵ���ʱ�Ĵ�����
	    @return ����ź���SIGINT, ����0, ���򷵻�-1.
	*/
	int handle_signal (int signum_);

	/*! \brief ��־�źŴ������Ƿ��׳��˰�ȫ�˳��ź�.
	    @return û���׳��Ļ�����0, ���򷵻�1.
	*/
	sig_atomic_t graceful_quit();

	/*! \brief ����״̬
	 */
	void resetState();

private:
	sig_atomic_t m_graceful_quit; /*!< �ź��Ƿ񱻲�׽���ı�־ */
};
 
inline
SIGINTHandler::
SIGINTHandler ()
	: m_graceful_quit (0)
{
	trace_with_mask("SIGINTHandler::SIGINTHandler", SIGHAND);
}

inline int
SIGINTHandler::
handle_signal (int signum_)
{
	trace_with_mask("SIGINTHandler::handle_signal", SIGHAND);

	if (signum_ == SIGINT) {
		m_graceful_quit = 1;
		return 0;
	}
	return -1;
}

inline sig_atomic_t
SIGINTHandler::
graceful_quit ()
{
	return m_graceful_quit;
}

inline void
SIGINTHandler::
resetState()
{
	trace_with_mask("SIGINTHandler::resetState", SIGHAND);

	m_graceful_quit = 0;
}

/*! \class SIGUSR1Handler Handlers.h
 * \brief SIGUSR1�źŴ�������
*/
class SIGUSR1Handler : public EventHandler
{
public:
	/*! \brief ���캯��
	 */
	SIGUSR1Handler() : m_count(0) {
		trace_with_mask("SIGUSR1Handler::SIGUSR1Handler", SIGHAND);
	}
    
	/*! \brief ��׽��USR1�źź�Ĵ�����
	 */
	int handle_signal(int signum_) {
		trace_with_mask("SIGUSR1Handler::handle_signal()", SIGHAND);
	    
		if (signum_ == SIGUSR1) { 
			m_count++; 
			DL((TRACE, "signal count = %d\n", m_count));
			return 0; 
		}
		return -1;
	}
    
	/*! \brief ���ز�׽����USR1�źŸ���
	 */
	sig_atomic_t received_count() const { return m_count; }
    
	/*! \brief ��׽����USR1�źŸ�����0.
	 */
	void resetState() { m_count = 0; }

private:
	sig_atomic_t m_count; /*!< ��׽����USR1�źŸ��� */
};


/*! \class SIGUSR2Handler Handlers.h
 * \brief SIGUSR2�źŴ�����
 */
class SIGUSR2Handler : public EventHandler
{
public:
	/*! \brief ���캯��
	 */
	SIGUSR2Handler() : m_count(0) {
		trace_with_mask("SIGUSR2Handler::SIGUSR2Handler", SIGHAND);
	}
    
	/*! \brief ��׽��USR2�źź�Ĵ�����
	 */
	int handle_signal(int signum_) {
		trace_with_mask("SIGUSR2Handler::handle_signal()", SIGHAND);
	    
		if (signum_ == SIGUSR2) { 
			m_count++; 
			DL((TRACE, "signal count = %d\n", m_count));
			return 0; 
		}
		return -1;
	}
    
	/*! \brief ���ز�׽����USR2�źŸ���
	 */
	sig_atomic_t received_count() const { return m_count; }
    
	/*! \brief ��׽����USR2�źŸ�����0.
	 */
	void resetState() { m_count = 0; }

private:
	sig_atomic_t m_count; /*!< ��׽����USR2�źŸ��� */
};

/*! \class SIGCHLDHandler Handlers.h
 * \brief SIGCHLD�źŴ�����
 */
class SIGCHLDHandler : public EventHandler
{
public:
	/*! \brief ���캯��
	 */
	SIGCHLDHandler() : m_child_exit_flag(0) {
		trace_with_mask("SIGCHLDHandler::SIGCHLDHandler", SIGHAND);
	}

	/*! \brief ��׽��SIGCHLD�źź�Ĵ�����
	 */
	int handle_signal(int signum_) {
		trace_with_mask("SIGCHLDHandler::handle_signal", SIGHAND);

		if (signum_ == SIGCHLD && wait(NULL) != -1) { 
			m_child_exit_flag = 1;
			return 0;
		}
		return -1;
	}

	/*! \brief ����ӽ����Ƿ��˳�
	 */
	sig_atomic_t child_exited() { return m_child_exit_flag; }

	/*! \brief �����ӽ����˳�״̬
	 */
	void resetState() { m_child_exit_flag = 0; }

private:
	sig_atomic_t m_child_exit_flag; /*!< �ӽ����˳�״̬ */
};

/*! \class SIGALRMHandler Handlers.h
 * \brief SIGALRM�źŴ�����
 */
class SIGALRMHandler : public EventHandler
{
public:
	/*! \brief ���캯��
	 */
	SIGALRMHandler() : m_alarm_flag(0) {
		trace_with_mask("SIGALRMHandler::SIGALRMHandler", SIGHAND);
	}

	/*! \brief ��׽��ALRM�źź�Ĵ�����
	 */
	int handle_signal(int signum_) {
		trace_with_mask("SIGALRMHandler::handle_signal", SIGHAND);

		if (signum_ == SIGALRM) {
			m_alarm_flag = 1; // notice that we have seen alarm
			return 0;
		}
		return -1;
	}

	/*! \breif ����Ƿ�����˾���(ALARM)�ź�
	 */
	sig_atomic_t alarmed () { return m_alarm_flag; }

	/*! \brief �����ڲ�״̬
	 */
	void resetState () { m_alarm_flag = 0; }

private:
	sig_atomic_t m_alarm_flag;
};

/*! \class SIGPOLLHander Handlers.h
 * \brief SIGPOLLHandlerʵ����һ����ٵ��¼�������, �����ڷǷ��׽�����
 * ����select(3)ʱ������SIGPOLL'��û'����
 */
class SIGPOLLHandler : public EventHandler
{
public:
	/*! \brief ���캯��
	 */
	SIGPOLLHandler () {
		trace_with_mask("SIGPOLLHandler", SIGHAND);
	}

	/*! \brief ��׽��SIGPOLL�źź�Ĵ�����
	 */
	int handle_signal ( int signum_ ) { 
		trace_with_mask("SIGPOLLHandler::handle_signal", SIGHAND);
			
		return (signum_ == SIGPOLL) ? 0 : -1; 
	}
};

END_SUNRISE_NAMESPACE

#endif
