#ifndef AUTO_PTR_H
#define AUTO_PTR_H

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif

/** \example autoptr_test.cpp
 * \brief ����ָ����Գ���
 */


/** \defgroup libsr �������߿�(libsr)
/** \{*/
/** \file AutoPtr.h 
 * \brief ����ָ����, ġ��ָ����Ϊ�����Զ��ͷ��ڴ档
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

/** \defgroup libsr �������߿�(libsr)
/** \{*/
/** \class AutoPtr AutoPtr.h
 * \brief ����ָ��
 * ġ��c����ָ����Ϊ�������ֶ�delete�����ڷ��������Զ��ͷ��ڴ棬��ʹ����exception��Ҳ��
 * �����ͷš������ڴ�й©.
 * Ŀǰ�������ָ���ʵ�ֻ��кܶ���ƿռ䣬��Щ���ӣ���policyģʽʵ�ֶ���ӵ��Ȩ���ԣ�����
 * �߳�ģ��.
 */
/** \}*/
template <class X> class AutoPtr {
private:
    /** \brief ʵ��ָ��
	  */
    X* m_ptr;

    /** \brief ӵ��Ȩ��־.
	 */
    bool m_owns;

public:
    /** \brief ���캯��
	  * �ؼ���explicit��Ϊ�˷�ֹ��ʽ����.
	    \param p_ ʵ��ָ��
     */
    explicit AutoPtr(X* p_ = 0) 
		: m_ptr(p_), m_owns( (p_ ? true : false) ) 	{}

    /** \brief �������캯��.
	  * ��ֵ����ӵ��Ȩ.
 	  *  \param a_ ������ֵ
     */
    AutoPtr (AutoPtr& a_)  
		: m_ptr(a_.m_ptr), m_owns(a_.m_owns) { a_.m_owns = 0; }

#if 0
    // �����Ժ������֧���˺���ʵ��.
//      template <class T> AutoPtr(const AutoPtr<T>& a) 
//  	: m_ptr(a.m_ptr), m_owns(a.m_owns) {
//  	a.m_owns = 0;
//      }
#endif

    /** \brief ��ֵ������
	  * ��ֵ����ӵ��Ȩ
     *
     *  \param a_ ��ֵ��ֵ
     *  \return ���ر����������
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
    // �����Ժ������֧���˺���ʵ��.
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

    /** \brief ��������
     */
    ~AutoPtr() { if (m_owns) delete m_ptr;  }
    
    X& operator*() const  { return *m_ptr; }
    X* operator->() const  { return m_ptr; }
	
    /** \brief ��ȡʵ��ָ��
	  * ���޸�ӵ��Ȩ
     */
    X* get() const  { return m_ptr; }
	
    /** \brief ��ֹӵ��Ȩ������ʵ��ָ��
     */
    X* release() { m_owns = false; return m_ptr; }
};

END_SUNRISE_NAMESPACE

#endif /* AUTO_PTR_H */
