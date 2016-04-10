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

/** \defgroup libsr 基础工具库(libsr)
/** \{*/
/** \file PriorityQueue_STLPQ.h  
 *  \brief   基于STL优先队列(priority_queue)的实现
*/

/** \class PriorityQueue_Heap
 * \brief 基于STL中优先队列容器(priority_queue), 是PriorityQueue_Impl接口的实现类，和PriorityQueue类组成Bridge模式.
 */
/** \}*/
template< class T, class Compare >
class PriorityQueue_STLPQ : 
	public PriorityQueue_Impl< T, Compare >
{
public:
	/** \brief 析构函数*/
        ~PriorityQueue_STLPQ ();

	/** \brief 向优先队列里面插入元素 */
	void     insert (const T&);
	/** \brief 返回优先队列里面最前面的元素,并从优先队列中删除
	 *  \return 返回优先队列里面最前面的元素*/
	T        pop ();
	/** \brief 返回优先队列里面最前面的元素
	 *  \return 返回优先队列里面最前面的元素*/
	const T& top () const;
	/** \brief 删除优先队列中,handler的id为指定值的元素
	 *  \param 要删除的元素的handler id
	 *  \return 操作是否成功 true成功,false失败*/
	bool     remove (const int);
	/** \brief 返回优先队列中元素的个数
	 *  \return 返回优先队列中元素的个数*/
	size_t   size ();
	/** \brief 顺次调用各个元素的dump函数 */
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
