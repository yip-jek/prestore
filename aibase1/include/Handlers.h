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
	 几个常用信号的信号处理器类
 */

/*! \class SIGINTHandler Handlers.h
 * \brief SIGINT信号处理器类
*/
class SIGINTHandler : public EventHandler
{
public:
	/*! \brief 构造函数
	 */
	SIGINTHandler();

	/*! \brief SIGINT信号到达时的处理函数
	    @return 如果信号是SIGINT, 返回0, 否则返回-1.
	*/
	int handle_signal (int signum_);

	/*! \brief 标志信号处理器是否抛出了安全退出信号.
	    @return 没有抛出的话返回0, 否则返回1.
	*/
	sig_atomic_t graceful_quit();

	/*! \brief 重置状态
	 */
	void resetState();

private:
	sig_atomic_t m_graceful_quit; /*!< 信号是否被捕捉到的标志 */
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
 * \brief SIGUSR1信号处理器类
*/
class SIGUSR1Handler : public EventHandler
{
public:
	/*! \brief 构造函数
	 */
	SIGUSR1Handler() : m_count(0) {
		trace_with_mask("SIGUSR1Handler::SIGUSR1Handler", SIGHAND);
	}
    
	/*! \brief 捕捉到USR1信号后的处理函数
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
    
	/*! \brief 返回捕捉到的USR1信号个数
	 */
	sig_atomic_t received_count() const { return m_count; }
    
	/*! \brief 捕捉到的USR1信号个数清0.
	 */
	void resetState() { m_count = 0; }

private:
	sig_atomic_t m_count; /*!< 捕捉到的USR1信号个数 */
};


/*! \class SIGUSR2Handler Handlers.h
 * \brief SIGUSR2信号处理类
 */
class SIGUSR2Handler : public EventHandler
{
public:
	/*! \brief 构造函数
	 */
	SIGUSR2Handler() : m_count(0) {
		trace_with_mask("SIGUSR2Handler::SIGUSR2Handler", SIGHAND);
	}
    
	/*! \brief 捕捉到USR2信号后的处理函数
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
    
	/*! \brief 返回捕捉到的USR2信号个数
	 */
	sig_atomic_t received_count() const { return m_count; }
    
	/*! \brief 捕捉到的USR2信号个数清0.
	 */
	void resetState() { m_count = 0; }

private:
	sig_atomic_t m_count; /*!< 捕捉到的USR2信号个数 */
};

/*! \class SIGCHLDHandler Handlers.h
 * \brief SIGCHLD信号处理类
 */
class SIGCHLDHandler : public EventHandler
{
public:
	/*! \brief 构造函数
	 */
	SIGCHLDHandler() : m_child_exit_flag(0) {
		trace_with_mask("SIGCHLDHandler::SIGCHLDHandler", SIGHAND);
	}

	/*! \brief 捕捉到SIGCHLD信号后的处理函数
	 */
	int handle_signal(int signum_) {
		trace_with_mask("SIGCHLDHandler::handle_signal", SIGHAND);

		if (signum_ == SIGCHLD && wait(NULL) != -1) { 
			m_child_exit_flag = 1;
			return 0;
		}
		return -1;
	}

	/*! \brief 检测子进程是否退出
	 */
	sig_atomic_t child_exited() { return m_child_exit_flag; }

	/*! \brief 重置子进程退出状态
	 */
	void resetState() { m_child_exit_flag = 0; }

private:
	sig_atomic_t m_child_exit_flag; /*!< 子进程退出状态 */
};

/*! \class SIGALRMHandler Handlers.h
 * \brief SIGALRM信号处理类
 */
class SIGALRMHandler : public EventHandler
{
public:
	/*! \brief 构造函数
	 */
	SIGALRMHandler() : m_alarm_flag(0) {
		trace_with_mask("SIGALRMHandler::SIGALRMHandler", SIGHAND);
	}

	/*! \brief 捕捉到ALRM信号后的处理函数
	 */
	int handle_signal(int signum_) {
		trace_with_mask("SIGALRMHandler::handle_signal", SIGHAND);

		if (signum_ == SIGALRM) {
			m_alarm_flag = 1; // notice that we have seen alarm
			return 0;
		}
		return -1;
	}

	/*! \breif 检测是否产生了警告(ALARM)信号
	 */
	sig_atomic_t alarmed () { return m_alarm_flag; }

	/*! \brief 重置内部状态
	 */
	void resetState () { m_alarm_flag = 0; }

private:
	sig_atomic_t m_alarm_flag;
};

/*! \class SIGPOLLHander Handlers.h
 * \brief SIGPOLLHandler实现了一个虚假的事件处理器, 处理在非法套接字上
 * 调用select(3)时产生的SIGPOLL'烟没'现象。
 */
class SIGPOLLHandler : public EventHandler
{
public:
	/*! \brief 构造函数
	 */
	SIGPOLLHandler () {
		trace_with_mask("SIGPOLLHandler", SIGHAND);
	}

	/*! \brief 捕捉到SIGPOLL信号后的处理函数
	 */
	int handle_signal ( int signum_ ) { 
		trace_with_mask("SIGPOLLHandler::handle_signal", SIGHAND);
			
		return (signum_ == SIGPOLL) ? 0 : -1; 
	}
};

END_SUNRISE_NAMESPACE

#endif
