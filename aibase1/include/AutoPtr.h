#ifndef AUTO_PTR_H
#define AUTO_PTR_H

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif

/** \example autoptr_test.cpp
 * \brief 智能指针测试程序
 */


/** \defgroup libsr 基础工具库(libsr)
/** \{*/
/** \file AutoPtr.h 
 * \brief 智能指针类, 摹拟指针行为，能自动释放内存。
 */
/** \}*/

#if defined(sun) && (__SUNPRO_CC == 0x420)
#if !defined(explicit)

/** \def explicit
	explicit keyword is missing from old Sun CC 4.2
*/
#define explicit
#endif
#endif

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr 基础工具库(libsr)
/** \{*/
/** \class AutoPtr AutoPtr.h
 * \brief 智能指针
 * 摹拟c语言指针行为，不需手动delete，能在非生命期自动释放内存，即使产生exception，也能
 * 正常释放。避免内存泄漏.
 * 目前这个智能指针的实现还有很多改善空间，过些日子，用policy模式实现多种拥有权策略，多种
 * 线程模型.
 */
/** \}*/
template <class X> class AutoPtr {
private:
    /** \brief 实物指针
	  */
    X* m_ptr;

    /** \brief 拥有权标志.
	 */
    bool m_owns;

public:
    /** \brief 构造函数
	  * 关键字explicit是为了防止隐式调用.
	    \param p_ 实物指针
     */
    explicit AutoPtr(X* p_ = 0) 
		: m_ptr(p_), m_owns( (p_ ? true : false) ) 	{}

    /** \brief 拷贝构造函数.
	  * 右值放弃拥有权.
 	  *  \param a_ 拷贝右值
     */
    AutoPtr (AutoPtr& a_)  
		: m_ptr(a_.m_ptr), m_owns(a_.m_owns) { a_.m_owns = 0; }

#if 0
    // 留待以后编译器支持了后再实现.
//      template <class T> AutoPtr(const AutoPtr<T>& a) 
//  	: m_ptr(a.m_ptr), m_owns(a.m_owns) {
//  	a.m_owns = 0;
//      }
#endif

    /** \brief 赋值操作符
	  * 右值放弃拥有权
     *
     *  \param a_ 赋值右值
     *  \return 返回本对象的引用
     */
    AutoPtr& operator= (AutoPtr& a_)  {
		if (&a_ != this) {
			if (m_owns)
				delete m_ptr;
			m_owns = a_.m_owns;
			m_ptr = a_.m_ptr;
			a_.m_owns = 0;
		}
		return *this;
    }

#if 0
    // 留待以后编译器支持了后再实现.
//      template <class T> AutoPtr& operator=(const AutoPtr<T>& a)  {
//  	if (&a != this) {
//  	    if (m_owns)
//  		delete m_ptr;
//  	    m_owns = a.m_owns;
//  	    m_ptr = a.m_ptr;
//  	    a.m_owns = 0;
//  	}
//      }
#endif

    /** \brief 析构函数
     */
    ~AutoPtr() { if (m_owns) delete m_ptr;  }
    
    X& operator*() const  { return *m_ptr; }
    X* operator->() const  { return m_ptr; }
	
    /** \brief 获取实物指针
	  * 不修改拥有权
     */
    X* get() const  { return m_ptr; }
	
    /** \brief 终止拥有权并返回实物指针
     */
    X* release() { m_owns = false; return m_ptr; }
};

END_SUNRISE_NAMESPACE

#endif /* AUTO_PTR_H */
