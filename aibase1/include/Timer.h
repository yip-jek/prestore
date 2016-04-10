//------------------------------------------------------------------------------
//                             Timer.h
//------------------------------------------------------------------------------
#ifndef TIMER_H
#define TIMER_H

#include <sys/select.h>
#include <sys/time.h>

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "EventHandler.h"
#include "TimeVal.h"

BEGIN_SUNRISE_NAMESPACE

class TimerQueue;		// forward declaration

/** \defgroup libsr 基础工具库(libsr)
 * \{*/
/** \file Timer.h 
 *  \brief 定时器类实现存储在定时器队列里的元素
 */

/** \class Timer
 * \brief  定时器类
 */
/** \}*/
class Timer 
{
public:
	/** \brief 构造函数
	 */
	Timer ();

	/** \brief 构造函数
	 * 通过事件处理器对象指针和时间对象构造.
	 */
	Timer (const EventHandler* eh_, const TimeVal& tm_);

	/** \brief 拷贝构造函数
	 */
	Timer (const Timer& t_);

	/** \brief  析构函数
	 */
	~Timer ();

	/** \brief 赋值运算符
	 */
	Timer& operator= (const Timer& t_);

	/** \brief 小于运算符
	 */
	bool operator< (const Timer& t_) const;

	/** \brief 等于运算符
	 */
	bool operator== (const Timer& t_) const;

	/** \brief 获取事件处理器指针
	 */
	EventHandler* getHandler () const { return m_eh; }

	/** \brief 获取超时时间
	 */
	const TimeVal& getExpirationTime () const { return m_timer; }

	/** \brief 把定时器状态写入日志文件
	 */
	void dump (void);

private:
	EventHandler* m_eh; /**< 事件处理器指针 */
	TimeVal m_timer;  /**< 时间对象, 指定事件处理器何时触发 */
};
//------------------------------------------------------------------------------
// Timer class inlines
//------------------------------------------------------------------------------

inline	
Timer::
Timer () 
	: m_eh (NULL) 
{
	trace("Timer::Timer");
}

inline
Timer::
Timer (const EventHandler* eh_, const TimeVal& tm_)
	: m_eh ((EventHandler*) eh_), m_timer (tm_) 
{
	trace("Timer::Timer(EH*, TV&)");
}

inline
Timer::
Timer (const Timer& t_)
	: m_eh (t_.m_eh), m_timer (t_.m_timer) 
{
	trace("Timer::Timer(Timer&)");
}

inline
Timer::
~Timer ()
{
	trace("Timer::~Timer");
}

inline Timer& 
Timer::
operator=(const Timer& t_)
{
	m_eh = t_.m_eh;
	m_timer = t_.m_timer;
	return *this;
}

inline bool 
Timer::
operator<(const Timer& t_) const
{
	return m_timer < t_.m_timer;
}

inline bool 
Timer::
operator==(const Timer& t_) const
{
	return m_timer == t_.m_timer;
}

inline void
Timer::
dump (void)
{
	DL((REACT,"EvtHdlr: 0x%x, Expire: %s\n",
	    m_eh, m_timer.fmtString().c_str()));
}

/** 
 * TimerCompare class compares two Timers base on their expiration timestamp.
 */
struct TimerCompare 
{
	bool operator() (const Timer* t1_, const Timer* t2_) const 
		{
			return *t1_ < *t2_;
		}
};

END_SUNRISE_NAMESPACE

#endif /* TIMER_H */  
