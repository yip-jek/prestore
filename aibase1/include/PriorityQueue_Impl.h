//------------------------------------------------------------------------------
//                        PriorityQueue_Impl.h
//------------------------------------------------------------------------------

#ifndef PRIORITY_QUEUE_IMPL_H
#define PRIORITY_QUEUE_IMPL_H

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr �������߿�(libsr)
 * \{*/
/** \file PriorityQueue_Impl.h 
 *  \brief ���������ȼ�����ʵ����ӿ�,��PriorityQueue�����Bridgeģʽ.
*/

// less<> is borrowed from STL implementation.
/** \struct Bfunc
	PriorityQueue_Impl�õĺ�������
*/
/** \}*/
template <class Arg1, class Arg2, class Result>
struct Bfunc {
	typedef Arg1   first_argument_type;
	typedef Arg2   second_argument_type;
	typedef Result result_type;
};
 
/** \defgroup libsr �������߿�(libsr)
 * \{*/
/** \struct Less
	PriorityQueue_Impl�õıȽϺ�������.
*/
/** \}*/
template <class T>
struct Less : public Bfunc<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x < y; }
};

/** \defgroup libsr �������߿�(libsr)
 *  \{*/
/** \class PriorityQueue_Impl.
 * \brief ʵ��PriorityQueue�㷨�Ľӿڣ��������ṩ����ʵ��.
 */
/** \}*/
template< class T, class Compare >
class PriorityQueue_Impl
{
public:
	/** \brief ��������*/
	virtual ~PriorityQueue_Impl ();

	/** \brief �����ȶ��в���Ԫ��,���麯��*/
	virtual void     insert (const T&) =0;
	/** \brief �������ȶ����ж�ͷ��Ԫ��,�������ȶ�����ɾ��,���麯��*/
	virtual T        pop () =0;
	/** \brief �������ȶ��еĶ�ͷԪ��,���麯��*/
	virtual const T& top () const =0;
	/** \brief �Ƴ����ȶ����кʹ��������ȵ�Ԫ��,���麯��*/
	virtual bool     remove (T) =0;
	/** \brief �������ȶ�������Ԫ�صĸ���,���麯��*/
	virtual size_t   size () =0;
	/** \brief ����[],����ָ��λ�õ�Ԫ��,���麯��*/
	virtual T&       operator[] (int) =0;
	
};

template<class T, class Compare>
inline
PriorityQueue_Impl<T, Compare>::
~PriorityQueue_Impl () 
{ 
	// Here is the twist: we must provide a definition for the virtual
	// destructor. We need the definition here because the way virtual 
	// destructors work. Most derived class's destructor is called first,
	// then the destructor of each base class is called. That means that
	// the compiler will generate a call to ~PriorityQueue_Impl, even
	// though the class is abstract - that's why we need body here.
	
	/* no-op */ 
}

END_SUNRISE_NAMESPACE

#endif /* PRIORITY_QUEUE_IMPL_H */  
