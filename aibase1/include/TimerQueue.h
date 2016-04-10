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
 * \brief TimerQueue ��ʱ����������Գ���
 */

BEGIN_SUNRISE_NAMESPACE

typedef unsigned long TimerId;

/** \defgroup libsr �������߿�(libsr)
 * \{*/
/** \file TimerQueue.h 
 *  \brief ��ʱ������
 */

/** \class TimerQueue TimerQueue.h
 *
 * ʵ�ֿ������ķ�����ע�Ტ���ɵĶ�ʱ������, ����һ�����ȶ��У���ʱʱ�䱻�趨Ϊ�����ǰʱ���
 * Ԫ�ر����ڶ��ж���.ͨ������top()�ɷ������Ҫ��ʱ�Ķ�ʱ��.
 */
/** \}*/
class TimerQueue
{
public:
	/** \brief ���캯��. 
	 */
	TimerQueue ();

	/** \brief ��������
	 */
	~TimerQueue ();

	/** \brief �������Ƿ�Ϊ��
	 *  \return ���δ�շ���true,���򷵻�false
	 */
	bool isEmpty ();

	/** \brief ����в���һ����ʱ��(EventHandler����), ��ָ����ʱ�����ص���ʱ��

	    \param eh_ ��ʱʱ����õĺ���ָ��
	    \param tv_ ��ʱʱ��
	    \return ����Ψһȷ�������Timer��TimerId
	*/
	TimerId insert (EventHandler* eh_, const TimeVal& tv_);

	/** \brief ����ʱ�䴦�����Ļص�����ָ����Ҳ��Ƴ��¼�������
	    \param eh_ Ҫɾ����ʱ�䴦�����Ļص�����ָ��.
	    \return ����ɾ�����ļ�ʱ������
	 */
	int remove (EventHandler* eh_);

	/** \brief �Ƴ�ָ��TimerId�Ķ�ʱ��
	    \param tid_ Timer id.
	    \return ����Ƴ��ɹ�����true,����Ƴ�ʧ�ܷ���false
	 */
	bool remove (TimerId tid_);

	/** \brief ������ʱ������, �ҳ���ʱ�Ķ�ʱ�����ص��䴦���������Ӷ������Ƴ�.
	    \param tv_ ��ʱʱ��
	    \return ���ط����˶��ٸ��ص�
	*/
	int expire (const TimeVal& tv_);

	/** \brief ����λ�ڶ��ж����Ķ�ʱ����Ȼ��ת��ΪTimeVal���󣬷��������� .
	 */
	TimeVal& top (void);

	/** \brief �Ѷ�����Ϣд����־�ļ�
	 */
	void dump (void);

private:
	/** \brief ��ʱ������ */
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
