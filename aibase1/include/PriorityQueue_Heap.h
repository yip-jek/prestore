//------------------------------------------------------------------------------
//                        PriorityQueue_Heap.h
//------------------------------------------------------------------------------
#ifndef PRIORITY_QUEUE_HEAP_H
#define PRIORITY_QUEUE_HEAP_H

#include <sys/types.h>
#include <string.h>

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "PriorityQueue_Impl.h"

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr 基础工具库(libsr)
 * \{*/
/** \file PriorityQueue_Heap.h
 *  \brief  基于堆的优先级队列算法. 可参看<<Algorithms in C++>> - 11.
*/

/** \class PriorityQueue_Heap
 * \brief 基于堆的优先级队列类, 是PriorityQueue_Impl接口的实现类，和PriorityQueue类组成Bridge模式.
 */
/** \}*/
template< class T, class Compare >
class PriorityQueue_Heap : public PriorityQueue_Impl< T, Compare >
{
public:
	/** \brief 构造函数,通过参数指定优先队列最大长度*/
	PriorityQueue_Heap (size_t max_ = 0);
	/** \brief 构造函数,通过参数指定优先队列的最大长度及比较类*/
	PriorityQueue_Heap (size_t, const Compare&);
	/** \brief 拷贝构造函数*/
	PriorityQueue_Heap (const PriorityQueue_Heap&);
	/** \brief 析构函数*/
	~PriorityQueue_Heap ();
	/** \brief 重载赋值操作*/
	PriorityQueue_Heap& operator= (const PriorityQueue_Heap&);

	/** \brief 向优先队列插入元素*/
	void     insert (const T&);
	/** \brief 返回优先队列中队头的元素,并从优先队列中删除*/
	T        pop ();
	/** \brief 返回优先队列的对头元素*/
	const T& top () const;
	/** \brief 移除优先队列中和传入参数相等的元素*/
	bool     remove (T);
	/** \brief 返回优先队列里面元素的个数*/
	size_t   size ();
	/** \brief 重载[],返回指定位置的元素*/
	T&       operator[] (int idx);

protected:
	/** \brief 从参数指定的位置向上调整堆的结构,保持堆是按顺序存放的*/
	void upheap (size_t);
	/** \brief 从参数指定的位置向下调整堆的结构,保证堆是按顺序存放的*/
	void downheap (size_t);
	/** \brief 重新分配堆的空间*/
	bool resize (size_t);

	Compare m_comp; /**< 比较的类函数*/
	
private:
	void allocate (size_t);

	T* m_queue;				/**< 指向保存堆的数组的指针*/
	size_t  m_size;				/**< 保存堆的数组的大小*/
	size_t  m_curr;				/**< 保存堆的数组中前面的一个可用空间*/
	size_t  m_lwm;				/**< 数组的最小大小,默认为20*/
};

template< class T, class Compare>
inline
PriorityQueue_Heap<T, Compare>::
PriorityQueue_Heap (size_t maxsz_)
	: m_curr (1), m_lwm (20)
{
	trace("PriorityQueue_Heap::PriorityQueue_Heap");
	allocate (maxsz_);
}

template< class T, class Compare>
inline
PriorityQueue_Heap<T, Compare>::
PriorityQueue_Heap (size_t maxsz_, const Compare& x_)
	: m_comp (x_), m_curr (1), m_lwm (20)
{
	allocate (maxsz_);
}

template< class T, class Compare>
inline void
PriorityQueue_Heap<T, Compare>::
allocate (size_t s_)
{
	m_size = s_ > m_lwm ? s_ : m_lwm;
	m_queue = new T [m_size];
}

template< class T, class Compare>
inline 
PriorityQueue_Heap<T, Compare>::
PriorityQueue_Heap (const PriorityQueue_Heap& h_)
	: m_comp (h_.m_comp), m_size (h_.m_size), m_curr (h_.m_curr),
	  m_lwm (h_.m_lwm)
{
	allocate (m_size);
	::memcpy (m_queue, h_.m_queue, sizeof(T)*m_curr);
}

template< class T, class Compare>
PriorityQueue_Heap<T, Compare>&
PriorityQueue_Heap<T, Compare>::
operator= (const PriorityQueue_Heap& h_)
{
	delete [] m_queue;
	m_comp = h_.m_comp;
	m_size = h_.m_size;
	m_curr = h_.m_curr;
	m_lwm  = h_.m_lwm;
	allocate (m_size);
	::memcpy (m_queue, h_.m_queue, sizeof(T)*m_curr);
	return *this;
}

template< class T, class Compare>
inline 
PriorityQueue_Heap<T, Compare>::
~PriorityQueue_Heap ()
{
	delete [] m_queue;
}

template< class T, class Compare>
void
PriorityQueue_Heap<T, Compare>::
insert (const T& t_)
{
	if (m_curr+1 == m_size)	 // if array filled up 
		resize (m_size*2); // then resize array

	m_queue [m_curr] = t_;
	upheap (m_curr);
	m_curr++;
}

template< class T, class Compare>
void
PriorityQueue_Heap<T, Compare>::
upheap (size_t k_)
{
	T v = m_queue[k_];
	m_queue[0] = 0;

	while ( k_/2 != 0 && m_comp (v, m_queue[k_/2]) ) {
		m_queue[k_] = m_queue[k_/2];
		k_ = k_/2;
	}
	m_queue[k_] = v;
}

template< class T, class Compare>
T
PriorityQueue_Heap<T, Compare>::
pop ()
{
	T v = m_queue[1];
	m_queue[1] = m_queue[--m_curr];
	
	downheap (1);
	if (m_curr*3 <= m_size && m_curr*2 > m_lwm) {
		resize (m_curr*2);
	}
	return v;
}

template< class T, class Compare>
inline const T&
PriorityQueue_Heap<T, Compare>::
top () const
{
	return (const T&) m_queue[1];
}

template< class T, class Compare>
void
PriorityQueue_Heap<T, Compare>::
downheap (size_t k_)
{
	register size_t j;
	T v = m_queue[k_];

	while (k_ <= m_curr/2) {
		j = 2*k_;
		if (j < m_curr && m_comp (m_queue[j+1], m_queue[j])) 
			j++;
		if (m_comp (v, m_queue[j])) 
			break;
		m_queue[k_] = m_queue[j];
		k_ = j;
	}
	m_queue[k_] = v;
}

template< class T, class Compare>
bool
PriorityQueue_Heap<T, Compare>::
remove (T t_)
{
	register size_t i;

	for (i=1; i < m_curr; i++) 
		if (m_queue[i] == t_)
			break;

	if (i == m_curr)	// not found
		return false;

	m_curr--;
	if (i == m_curr)	// last element in queue
		return true;

	m_queue[i] = m_queue[m_curr];
	downheap (i);

	return true;
}

template< class T, class Compare>
inline size_t
PriorityQueue_Heap<T, Compare>::
size ()
{
	return m_curr-1;
}

template< class T, class Compare>
bool
PriorityQueue_Heap<T, Compare>::
resize (size_t newsz_)
{
	if (m_size == newsz_)
		return true;

	T* new_chunk = new T[newsz_];
 	::memcpy (new_chunk, m_queue, m_curr * sizeof(T));
	delete [] m_queue;
	m_queue = new_chunk;
	m_size = newsz_;
	return true;
}

template< class T, class Compare>
T&
PriorityQueue_Heap<T, Compare>::
operator[] (int idx)
{
	return m_queue[idx+1];
}

END_SUNRISE_NAMESPACE

#endif /* PRIORITY_QUEUE_HEAP_H */  
