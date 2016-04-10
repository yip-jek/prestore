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

/** \defgroup libsr �������߿�(libsr)
 * \{*/
/** \file Timer.h 
 *  \brief ��ʱ����ʵ�ִ洢�ڶ�ʱ���������Ԫ��
 */

/** \class Timer
 * \brief  ��ʱ����
 */
/** \}*/
class Timer 
{
public:
	/** \brief ���캯��
	 */
	Timer ();

	/** \brief ���캯��
	 * ͨ���¼�����������ָ���ʱ�������.
	 */
	Timer (const EventHandler* eh_, const TimeVal& tm_);

	/** \brief �������캯��
	 */
	Timer (const Timer& t_);

	/** \brief  ��������
	 */
	~Timer ();

	/** \brief ��ֵ�����
	 */
	Timer& operator= (const Timer& t_);

	/** \brief С�������
	 */
	bool operator< (const Timer& t_) const;

	/** \brief ���������
	 */
	bool operator== (const Timer& t_) const;

	/** \brief ��ȡ�¼�������ָ��
	 */
	EventHandler* getHandler () const { return m_eh; }

	/** \brief ��ȡ��ʱʱ��
	 */
	const TimeVal& getExpirationTime () const { return m_timer; }

	/** \brief �Ѷ�ʱ��״̬д����־�ļ�
	 */
	void dump (void);

private:
	EventHandler* m_eh; /**< �¼�������ָ�� */
	TimeVal m_timer;  /**< ʱ�����, ָ���¼���������ʱ���� */
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
