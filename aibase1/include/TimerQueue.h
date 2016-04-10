//------------------------------------------------------------------------------
//                          TimerQueue.h
//------------------------------------------------------------------------------
#ifndef TIMER_QUEUE_H
#define TIMER_QUEUE_H

#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "Logger.h"
#include "EventHandler.h"
#include "Timer.h"
#include "PriorityQueue.h"

/** \example timer_queue_test.cpp
 * \brief TimerQueue 定时器队列类测试程序
 */

BEGIN_SUNRISE_NAMESPACE

typedef unsigned long TimerId;

/** \defgroup libsr 基础工具库(libsr)
 * \{*/
/** \file TimerQueue.h 
 *  \brief 定时器队列
 */

/** \class TimerQueue TimerQueue.h
 *
 * 实现可向中心分派器注册并分派的定时器队列, 这是一个有先队列，超时时间被设定为最近当前时间的
 * 元素被放在队列顶端.通过调用top()可返回最快要超时的定时器.
 */
/** \}*/
class TimerQueue
{
public:
	/** \brief 构造函数. 
	 */
	TimerQueue ();

	/** \brief 析构函数
	 */
	~TimerQueue ();

	/** \brief 检测队列是否为空
	 *  \return 如果未空返回true,否则返回false
	 */
	bool isEmpty ();

	/** \brief 向队列插入一个定时器(EventHandler对象), 并指定定时器被回调的时间

	    \param eh_ 超时时候调用的函数指针
	    \param tv_ 超时时间
	    \return 返回唯一确定插入的Timer的TimerId
	*/
	TimerId insert (EventHandler* eh_, const TimeVal& tv_);

	/** \brief 根据时间处理器的回调函数指针查找并移除事件处理器
	    \param eh_ 要删除的时间处理器的回调函数指针.
	    \return 返回删除掉的计时器个数
	 */
	int remove (EventHandler* eh_);

	/** \brief 移除指定TimerId的定时器
	    \param tid_ Timer id.
	    \return 如果移除成功返回true,如果移除失败返回false
	 */
	bool remove (TimerId tid_);

	/** \brief 遍历定时器队列, 找出超时的定时器，回调其处理函数，并从队列上移除.
	    \param tv_ 超时时间
	    \return 返回发生了多少个回调
	*/
	int expire (const TimeVal& tv_);

	/** \brief 返回位于队列顶部的定时器，然后转换为TimeVal对象，返回其引用 .
	 */
	TimeVal& top (void);

	/** \brief 把队列信息写入日志文件
	 */
	void dump (void);

private:
	/** \brief 定时器队列 */
	PriorityQueue <Timer*, TimerCompare> m_queue;
};

//------------------------------------------------------------------------------
// Inline functions
//------------------------------------------------------------------------------

inline
TimerQueue::
TimerQueue () 
{
	trace("TimerQueue::TimerQueue");
}

inline bool
TimerQueue::
isEmpty ()
{ 
	return m_queue.size () == 0;
}

inline TimerId
TimerQueue::
insert (EventHandler* eh_, const TimeVal& tv_) 
{ 
	trace("TimerQueue::insert");

	Timer* t = new Timer(eh_, tv_);
	m_queue.insert (t);
	return (TimerId) t;
}

inline TimeVal&
TimerQueue::
top (void)
{
	return (TimeVal&) m_queue.top ()->getExpirationTime ();
}

inline void
TimerQueue::
dump (void)
{
	trace("TimerQueue::dump");

	if (m_queue.size() == 0) {
		DL((REACT,"Queue is empty\n"));
	}
	else {
		for (size_t i = 0; i < m_queue.size (); ) {
			m_queue[i++]->dump();
		}
	}
}

END_SUNRISE_NAMESPACE

#endif /* TIMER_QUEUE_H */  
