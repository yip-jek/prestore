//------------------------------------------------------------------------------
//                             Reactor.h
//------------------------------------------------------------------------------
#ifndef REACTOR_H
#define REACTOR_H

#include <sys/time.h>			// select(2)
#include <sys/resource.h>		// getrlimit(2)

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "EventHandler.h"

#ifdef sun
#ifndef USE_SIMPLE_SINGLETON
#define USE_SIMPLE_SINGLETON 1
#endif
#endif

#ifdef USE_SIMPLE_SINGLETON
#include "SSingleton.h"
#else
#include "Singleton.h"
#endif

#include "MaskSet.h"
#include "TimerQueue.h"
#include "TimerCountdown.h"

BEGIN_SUNRISE_NAMESPACE

/*! \example reactor_test.cpp
 * 中心分派器测试程序
 */

/*! \file Reactor.h 
	Reactor模式(反应堆模式, 中心分派器模式)的实现.

	中心分派器从文件描述字读写数据，处理文件描述字的I/O异常, 或处理时间调度事件.
一旦用户向中心分派器注册了某描述字的事件处理器(EventHandler)，中心分派器就会在
描述字I/O读写事件到达时回调所注册的事件处理器. 时间调度事件的情况相同，调用
Reactor::waitForEvents(TimeVal* tv_)后，一旦时间到，中心分派器将回调时间事件处
理器.
*/
/*! \class Reactor Reactor.h
 * \brief 中心分派器
 */
class Reactor 
{
public:
	/*! \brief 构造函数
	 */
	Reactor ();

	/*! \brief 析构函数
	 */
	~Reactor();

	/*! \brief 向中心分派器注册时间处理器. 
	 *  注册后，一旦时间到，中心分派器将回调相应的处理器的回调函数.

	    @param eh_ [RW] Pointer to the EventHandler
	    @param tv_ [RO] Timeout value
	    @return Timer id that can be used to cancel timer.
	*/
	TimerId registerTimerHandler (EventHandler* eh_, const TimeVal& tv_);

	/*! \brief 向中心分派器注册I/O事件处理器.
	    注册后，一旦I/O事件发生，中心分派器将回调相应处理器的回调函数.

	    @param eh_ Pointer to the EventHandler
	    @param fd_ File descriptor
	    @param et_ Event Type
	    @return true if success, false if error
	 */
	bool registerIOHandler (EventHandler* eh_, int fd_, 
							EventType et_ = RWE_EVENTS);

	/*! \brief 移除事件处理器
	 *  如果et_是TIMEOUT_EVENT,所有的时间处理器都会被移除.

		 @param eh_ Pointer to the EventHandler
	    @param et_ Event Type to remove. Default will remove
	               Event Handler for all events.
	    @return true if success, false if wasn't registered for any events.
	 */
	bool removeHandler (EventHandler* eh_, EventType et_ = ALL_EVENTS);

	/*! \breif 移除id_标定的时间处理器
	    @param id_ Timer Id returned by registerTimer.
	    @return true if timer found and removed; false otherwise
	*/
	bool removeTimerHandler (TimerId id_);

	/*! \brief 移除fd_标定的I/O事件处理器
	    @param fd_ File descriptor
	    @return true on success, false if fd_ is out of range
	*/
	bool removeIOHandler (int fd_);

	/*! \brief 事件等待循环 
	 */
	void waitForEvents (void);

	/*! \brief 在指定的时间段内等待事件发生.
	 *  如果tv_为NULL, 则行为和waitForEvents(void)一样。如果tv_为{0,0}, 
	 *  将使中心分派器以非阻塞方式轮询事件. 如果在事件段内发生了事件，
	 *  中心分派器回调相应的处理器处理函数。tv_则调整为剩下的时间。
	 *
	    @param tv_ [RW] is time to wait for. 
	*/
	void waitForEvents (TimeVal* tv_);

	/*! \brief 关闭中心分派器引擎.
	 * 所有在中心分派器注册的事件处理器将被移除。中心分派器引擎关闭后不能
	 * 开启.
	*/
	void stopReactor (void);

    /*! \brief 冻结中心分派器. 
	  * 中心分派器被冻结后将在事件处理循环里略过所有的信号。一般在信号处理
	  * 函数中的代码中调用这个方法，信号处理返回后，中心分派器自动被激活.
	  * @see GenServer::handle_signal()
     */
    void deactivate (void);

private:
	Reactor (const Reactor&);	         /// no cloning 
	Reactor& operator= (const Reactor&); /// no cloning

private:
	/*! \brief 处理select(2)调用轮询错误
	 */
	bool handleError (void);

	/*! \brief 事件产生时通知注册在本中心分派器的事件处理器。
	    @param minimum_ number of file descriptors ready.
	 */
	bool dispatch (int minimum_);

	/*! \brief 返回描述字集里已经准备好读(写)的文件描述字.
	 */
	int isAnyReady (void);

	/*! \brief 检查非法的文件描述字.
	    @return true if any fd(s) were found and removed; 
	    false otherwise
	*/
	bool checkFDs (void);

	/*! \brief 回调处理器处理函数，如果返回负值则从中心分派器移除这个事件处理器
	 */
	void dispatchHandler ( FdSet& mask_, EventHandler** fdSet_,
			       EH_IO_Callback callback_);
	
	/*! 计算最近的超时时间.
	    @param maxwait_ (in) how long we are expected to wait for event(s).
	    @param howlong_ (out) how long we are going to wait.
	*/
	void calculateTimeout (TimeVal*& howlong_, TimeVal* maxwait_);

	
private:
	/*! 每个进程所能处理的文件数目(各种OS的限制有差别,可通过ulimit/limit调用取得.)
	 */
	int m_noFiles;

	/*! \brief 最大的文件描述字+1
	 */
	int m_maxfd;

	/*! \brief 标识中心分派器是活跃还是被冻结的标志.
	 */
	bool m_active;

	/*! \brief 等待READ_EVENT的事件处理器
	 */
	EventHandler** m_readSet;

	/*! \brief 等待WRITE_EVENT的事件处理器
	 */
	EventHandler** m_writeSet;

	/*! \brief 等待EXCEPT_EVENT的事件处理器
	 */
	EventHandler** m_exceptSet;

	MaskSet m_waitSet;

	MaskSet m_readySet;

	/*! 定时器队列
	 */
	TimerQueue m_tqueue;
};
/*******************************************************************************
 Inline functions
*******************************************************************************/

inline void Reactor::deactivate (void) {  m_active = false; }

END_SUNRISE_NAMESPACE

#endif /* REACTOR_H */  
