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

/** \defgroup libsr �������߿�(libsr)
 * \{*/
/** \file TimerCountdown.h 
 *  \brief TimerCountdown�������ʧ��ʱ��.
 */

/** \class TimerCountdown
 * \brief ʱ�䵹������
 * ʱ�䵹������¼�Ӷ����쿪ʼ����ʧ��ʱ�䣬���������󣬹���ʱ�Ĳ���������Ϊ����ʣ�µ�
 * ʱ�䣬����Ѿ���ʱ���������������Ϊ{0,0} (zeroTime)
 */
/** \}*/
class TimerCountdown
{
public:
	/** \brief ���캯��
	 *  \param wait_time_ ����ʱʱ��
	 */
	TimerCountdown (TimeVal* wait_time_);

	/** \brief ��������,�޸ĵ���ʱʱ��,�����ʱ�򵹼�ʱʱ����zeroTime,
	           ��������ŵ�ʱ��ӵ���ʱʱ�������ȥ
	 */
	~TimerCountdown ();

private:
	TimeVal* m_maxWaitTime; /**< ���浹��ʱʱ��*/

	TimeVal  m_start; /**< ��¼����ʱ��ʼʱ��*/
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
