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

/** \defgroup libsr �������߿�(libsr)
 * \{*/
/** \file PriorityQueue_Heap.h
 *  \brief  ���ڶѵ����ȼ������㷨. �ɲο�<<Algorithms in C++>> - 11.
*/

/** \class PriorityQueue_Heap
 * \brief ���ڶѵ����ȼ�������, ��PriorityQueue_Impl�ӿڵ�ʵ���࣬��PriorityQueue�����Bridgeģʽ.
 */
/** \}*/
template< class T, class Compare >
class PriorityQueue_Heap : public PriorityQueue_Impl< T, Compare >
{
public:
	/** \brief ���캯��,ͨ������ָ�����ȶ�����󳤶�*/
	PriorityQueue_Heap (size_t max_ = 0);
	/** \brief ���캯��,ͨ������ָ�����ȶ��е���󳤶ȼ��Ƚ���*/
	PriorityQueue_Heap (size_t, const Compare&);
	/** \brief �������캯��*/
	PriorityQueue_Heap (const PriorityQueue_Heap&);
	/** \brief ��������*/
	~PriorityQueue_Heap ();
	/** \brief ���ظ�ֵ����*/
	PriorityQueue_Heap& operator= (const PriorityQueue_Heap&);

	/** \brief �����ȶ��в���Ԫ��*/
	void     insert (const T&);
	/** \brief �������ȶ����ж�ͷ��Ԫ��,�������ȶ�����ɾ��*/
	T        pop ();
	/** \brief �������ȶ��еĶ�ͷԪ��*/
	const T& top () const;
	/** \brief �Ƴ����ȶ����кʹ��������ȵ�Ԫ��*/
	bool     remove (T);
	/** \brief �������ȶ�������Ԫ�صĸ���*/
	size_t   size ();
	/** \brief ����[],����ָ��λ�õ�Ԫ��*/
	T&       operator[] (int idx);

protected:
	/** \brief �Ӳ���ָ����λ�����ϵ����ѵĽṹ,���ֶ��ǰ�˳���ŵ�*/
	void upheap (size_t);
	/** \brief �Ӳ���ָ����λ�����µ����ѵĽṹ,��֤���ǰ�˳���ŵ�*/
	void downheap (size_t);
	/** \brief ���·���ѵĿռ�*/
	bool resize (size_t);

	Compare m_comp; /**< �Ƚϵ��ຯ��*/
	
private:
	void allocate (size_t);

	T* m_queue;				/**< ָ�򱣴�ѵ������ָ��*/
	size_t  m_size;				/**< ����ѵ�����Ĵ�С*/
	size_t  m_curr;				/**< ����ѵ�������ǰ���һ�����ÿռ�*/
	size_t  m_lwm;				/**< �������С��С,Ĭ��Ϊ20*/
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
