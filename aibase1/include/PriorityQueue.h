//------------------------------------------------------------------------------
//                            PriorityQueue.h
//------------------------------------------------------------------------------
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <unistd.h>
#include <limits.h>

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "Assert.h"
#include "PriorityQueue_Impl.h"
#include "PriorityQueue_Heap.h"

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr 基础工具库(libsr)
 * \{*/
/** \file PriorityQueue.h
 *  \brief 优先队列类定义, 这里使用了Bridge模式,  具体实现是PriorityQueue_Impl.
*/

template<class T, class Compare> class PriorityQueue_Impl;

/** \class PriorityQueue
 * \brief 优先队列类
 */
/** \}*/
template<class T, class Compare >
class PriorityQueue
{
public:
	/** \brief 构造函数
	  * \param max_ 指定优先队列的最大长度,默认20
	  */
	PriorityQueue (size_t max_ = 20);
	/** \brief 构造函数
	 *  \param max_ 指定优先队列的最大长度
	 *  \param Compare 指定的比较函数类
	 */
	PriorityQueue (size_t max_, const Compare&);

	/** \brief 析构函数*/
	virtual ~PriorityQueue ();

	/** \brief 向优先队列插入元素*/
	virtual void     insert (const T&);
	/** \brief 返回优先队列中队头的元素,并从优先队列中删除*/
	virtual T        pop ();
	/** \brief 返回优先队列的对头元素*/
	virtual const T& top () const;
	/** \brief 移除优先队列中和传入参数相等的元素*/
	virtual bool     remove (T&);
	/** \brief 返回优先队列里面元素的个数*/
	virtual size_t   size ();
	/** \brief 重载[],返回指定位置的元素*/
	virtual T&       operator[] (int);

	/** \brief 设置用堆排序实现的优先队列实现方式*/
	virtual void setHeapImpl (size_t, const Compare&);

protected:
	const PriorityQueue_Impl<T, Compare>* getPriorityQueueImpl () const;
	Compare m_comp;

	PriorityQueue (const PriorityQueue&);
	PriorityQueue& operator= (const PriorityQueue&);

private:
	PriorityQueue_Impl< T, Compare >* m_impl;
};

//----------------------------------------------------------------------------
// Member functions
//----------------------------------------------------------------------------

template <class T, class Compare>
inline 
PriorityQueue<T, Compare>::
PriorityQueue (size_t maxsz_)
	: m_impl (0)
{
	// This is a perfect place for using Factory to decide which
	// implementation to use
	// const char self[]="PriorityQueue::PriorityQueue(maxsz)"; trace();

	setHeapImpl (maxsz_, m_comp);
}

template <class T, class Compare>
inline 
PriorityQueue<T, Compare>::
PriorityQueue (size_t maxsz_, const Compare& x_)
	: m_comp (x_), m_impl (0)
{
	// This is perfect place for using Factory to decide which
	// implementation to use
	setHeapImpl (maxsz_, m_comp);
}

template <class T, class Compare>
inline void
PriorityQueue<T, Compare>::
setHeapImpl (size_t maxsz_, const Compare& x_)
{
	// Maybe here you would want to copy contents of the old
	// implementation to the new one?
	//

	if (m_impl != 0)
		delete m_impl;

	m_impl = new PriorityQueue_Heap<T, Compare> (maxsz_, x_);
}

template <class T, class Compare>
inline
PriorityQueue<T, Compare>::
~PriorityQueue ()
{
	delete m_impl;
}

template <class T, class Compare> void
inline
PriorityQueue<T, Compare>::
insert (const T& el_)
{
	m_impl->insert (el_);
}

template <class T, class Compare> T
inline
PriorityQueue<T, Compare>::
pop ()
{
	return m_impl->pop ();
}

template <class T, class Compare> 
inline const T&
PriorityQueue<T, Compare>::
top () const
{
	return m_impl->top ();
}

template <class T, class Compare> 
inline bool
PriorityQueue<T, Compare>::
remove (T& t_)
{
	return m_impl->remove (t_);
}

template <class T, class Compare> 
inline size_t
PriorityQueue<T, Compare>::
size ()
{
	return m_impl->size ();
}

template <class T, class Compare> 
inline const PriorityQueue_Impl<T, Compare>*
PriorityQueue<T, Compare>::
getPriorityQueueImpl () const
{
	return (const PriorityQueue_Impl<T, Compare>*) m_impl;
}

template <class T, class Compare> 
inline T&
PriorityQueue<T, Compare>::
operator[] (int idx)
{
	return (*m_impl)[idx];
}

END_SUNRISE_NAMESPACE

#endif /* PRIORITY_QUEUE_H */  
