//-----------------------------------------------------------------------------
- //                          PriorityQueue_STLPQ.h  //-------------------------
-----------------------------------------------------                    #ifndef
PRIORITY_QUEUE_STLPQ_H #define PRIORITY_QUEUE_STLPQ_H

#include <stack>
#include <deque>
#include <list>
#include <queue>
using namespace std;

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "Timer.h"

namespace sunrise {

/** \defgroup libsr �������߿�(libsr)
/** \{*/
/** \file PriorityQueue_STLPQ.h  
 *  \brief   ����STL���ȶ���(priority_queue)��ʵ��
*/

/** \class PriorityQueue_Heap
 * \brief ����STL�����ȶ�������(priority_queue), ��PriorityQueue_Impl�ӿڵ�ʵ���࣬��PriorityQueue�����Bridgeģʽ.
 */
/** \}*/
template< class T, class Compare >
class PriorityQueue_STLPQ : 
	public PriorityQueue_Impl< T, Compare >
{
public:
	/** \brief ��������*/
        ~PriorityQueue_STLPQ ();

	/** \brief �����ȶ����������Ԫ�� */
	void     insert (const T&);
	/** \brief �������ȶ���������ǰ���Ԫ��,�������ȶ�����ɾ��
	 *  \return �������ȶ���������ǰ���Ԫ��*/
	T        pop ();
	/** \brief �������ȶ���������ǰ���Ԫ��
	 *  \return �������ȶ���������ǰ���Ԫ��*/
	const T& top () const;
	/** \brief ɾ�����ȶ�����,handler��idΪָ��ֵ��Ԫ��
	 *  \param Ҫɾ����Ԫ�ص�handler id
	 *  \return �����Ƿ�ɹ� true�ɹ�,falseʧ��*/
	bool     remove (const int);
	/** \brief �������ȶ�����Ԫ�صĸ���
	 *  \return �������ȶ�����Ԫ�صĸ���*/
	size_t   size ();
	/** \brief ˳�ε��ø���Ԫ�ص�dump���� */
	void     dump ();

private:
	priority_queue<T*, deque<T*>, Compare>  m_queue;
};

template< class T, class Compare>
inline
PriorityQueue_STLPQ<T, Compare>::
~PriorityQueue_STLPQ ()
{
	trace("PriorityQueue_STLPQ::~PriorityQueue_STLPQ"); 

	while ( m_queue.size () ) {
		delete m_queue.top ();
		m_queue.pop ();
	}
}

template< class T, class Compare>
inline void
PriorityQueue_STLPQ<T, Compare>::
insert (const T& t_)
{
	trace("PriorityQueue_STLPQ::insert");
	m_queue.push (t_);
}

template< class T, class Compare>
inline T
PriorityQueue_STLPQ<T, Compare>::
pop ()
{
	trace("PriorityQueue_STLPQ::pop");

	T t = m_queue.top ();
	m_queue.pop ();
	return t;
}

template< class T, class Compare>
inline const T&
PriorityQueue_STLPQ<T, Compare>::
top () const
{
	trace("PriorityQueue_STLPQ::top");
	return (const T&) m_queue.top ();
}

/*******************************************************************************
   STL priority queue doesn't allow to remove arbitrary
   element from the queue. Only top element can be removed.
   To search for the element, I extract top one, and if it 
   doesn't match my search, put it into list<>. When either
   found or reached end of queue, I restore all elements
   in the list<> back to the priority queue.
   This needs rethinking!
*******************************************************************************/
template< class T, class Compare>
bool
PriorityQueue_STLPQ<T, Compare>::
remove (const int id_)
{
	trace("PriorityQueue_STLPQ::remove");

	list<Timer*> t_list;
	register Timer* t_ptr = 0;
	register int cnt = 0;

	while (m_queue.size () > 0) {
		t_ptr = m_queue.top ();
		if (t_ptr->getHandler ()-> id() == id_) {
			delete t_ptr;
			cnt++;
		}
		else {
			t_list.push_back (t_ptr);
		}
		m_queue.pop ();
	}
	// Restore queue
	
	list<Timer*>::iterator i;

	for (i = t_list.begin (); i != t_list.end (); i++) {
		m_queue.push (*i);
	}

	return cnt;
}

template< class T, class Compare>
inline size_t
PriorityQueue_STLPQ<T, Compare>::
size ()
{
	return m_queue.size ();
}

template< class T, class Compare>
inline void
PriorityQueue_STLPQ<T, Compare>::
dump () 
{
	trace("PriorityQueue_STLPQ::dump");

	list<Timer*> t_list;
	register Timer* t_ptr = 0;
	DL((TRACE,"======TimerQueue start=======\n"));
	while (m_queue.size () > 0) {
		t_ptr = m_queue.top ();
		t_ptr->dump ();
		t_list.push_back (t_ptr);
	}
	DL((TRACE,"======TimerQueue end=========\n"));
	list<Timer*>::iterator i;

	for (i = t_list.begin (); i != t_list.end (); i++) {
		m_queue.push (*i);
	}
}

} // end namespace sunrise

#endif /* PRIORITY_QUEUE_STLPQ_H */  
