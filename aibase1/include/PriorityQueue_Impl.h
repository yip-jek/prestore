//------------------------------------------------------------------------------
//                        PriorityQueue_Impl.h
//------------------------------------------------------------------------------

#ifndef PRIORITY_QUEUE_IMPL_H
#define PRIORITY_QUEUE_IMPL_H

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr 基础工具库(libsr)
 * \{*/
/** \file PriorityQueue_Impl.h 
 *  \brief 定义了优先级队列实现类接口,和PriorityQueue类组成Bridge模式.
*/

// less<> is borrowed from STL implementation.
/** \struct Bfunc
	PriorityQueue_Impl用的函数对象
*/
/** \}*/
template <class Arg1, class Arg2, class Result>
struct Bfunc {
	typedef Arg1   first_argument_type;
	typedef Arg2   second_argument_type;
	typedef Result result_type;
};
 
/** \defgroup libsr 基础工具库(libsr)
 * \{*/
/** \struct Less
	PriorityQueue_Impl用的比较函数对象.
*/
/** \}*/
template <class T>
struct Less : public Bfunc<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x < y; }
};

/** \defgroup libsr 基础工具库(libsr)
 *  \{*/
/** \class PriorityQueue_Impl.
 * \brief 实现PriorityQueue算法的接口，衍生类提供具体实现.
 */
/** \}*/
template< class T, class Compare >
class PriorityQueue_Impl
{
public:
	/** \brief 析构函数*/
	virtual ~PriorityQueue_Impl ();

	/** \brief 向优先队列插入元素,纯虚函数*/
	virtual void     insert (const T&) =0;
	/** \brief 返回优先队列中队头的元素,并从优先队列中删除,纯虚函数*/
	virtual T        pop () =0;
	/** \brief 返回优先队列的对头元素,纯虚函数*/
	virtual const T& top () const =0;
	/** \brief 移除优先队列中和传入参数相等的元素,纯虚函数*/
	virtual bool     remove (T) =0;
	/** \brief 返回优先队列里面元素的个数,纯虚函数*/
	virtual size_t   size () =0;
	/** \brief 重载[],返回指定位置的元素,纯虚函数*/
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
