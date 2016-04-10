//------------------------------------------------------------------------------
//                        TimerCountdown.h
//------------------------------------------------------------------------------
#ifndef TIMER_COUNTDOWN_H
#define TIMER_COUNTDOWN_H

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "TimeVal.h"

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr 基础工具库(libsr)
 * \{*/
/** \file TimerCountdown.h 
 *  \brief TimerCountdown类跟踪流失的时间.
 */

/** \class TimerCountdown
 * \brief 时间倒数器类
 * 时间倒数器记录从对象构造开始后流失的时间，对象析构后，构造时的参数将被改为最新剩下的
 * 时间，如果已经超时，则这个参数被改为{0,0} (zeroTime)
 */
/** \}*/
class TimerCountdown
{
public:
	/** \brief 构造函数
	 *  \param wait_time_ 倒计时时间
	 */
	TimerCountdown (TimeVal* wait_time_);

	/** \brief 析构函数,修改倒计时时间,如果超时则倒计时时间变成zeroTime,
	           否则把流逝的时间从倒计时时间里面减去
	 */
	~TimerCountdown ();

private:
	TimeVal* m_maxWaitTime; /**< 保存倒计时时间*/

	TimeVal  m_start; /**< 记录倒计时开始时间*/
};

//------------------------------------------------------------------------------
// inlines
//------------------------------------------------------------------------------

inline
TimerCountdown::
TimerCountdown (TimeVal* wt_)
	: m_maxWaitTime (wt_), m_start (TimeVal::gettimeofday ())
{
}

inline
TimerCountdown::
~TimerCountdown ()
{
	if (m_maxWaitTime == NULL)
		return; 

	TimeVal elapsed (TimeVal::gettimeofday ());
	elapsed -= m_start;

	if ( *m_maxWaitTime > elapsed )
		*m_maxWaitTime -= elapsed;
	else
		*m_maxWaitTime = TimeVal::zeroTime ();
}

END_SUNRISE_NAMESPACE

#endif /* TIMER_COUNTDOWN_H */  
