//------------------------------------------------------------------------------
//                            SigSet.h
//------------------------------------------------------------------------------
#ifndef _SigSet_h
#define _SigSet_h

// System includes
//
#include <signal.h>
#include <errno.h>

#include "Prelude.h"

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr  信号处理模块(libsr)  
@{ 
*/

/*! \example sighand_test.cpp
 * SigSet测试程序
 */

/*! \file SigSet.h 

\brief UNIX系统sigset_t结构体的包裹类, 提供维护sigset_t结构体的函数

把<tt>SitSet</tt>转换为<tt>sigset_t</tt>指针如下所示 :

<pre>
   SigSet  source;
   sigset_t*  targetp;

   targetp = source;
</pre>

SigSet类定义了一个转换操作符把<tt>SigSet</tt>转换成类型为<tt>sigset_t*</tt>的
<tt>lvalue</tt>(左值)
<p>
另一个例子:

<pre>
   struct sigaction action;
   SigSet siganls_to_block;

   // manipulat signal set in some meaningful way

   action.sa_mask = *signals_to_block;
</pre>
*/

/*! \class SigSet SigSet.h
 * \brief UNIX系统sigset_t结构体的包裹类.
 */
class SigSet
{
public:
	/*! \brief 缺省构造函数
	*/
	SigSet();

	/*! \brief 拷贝构造函数
	 */
	SigSet(sigset_t* source_);
	
	/*! \brief 析构函数
	 */
	~SigSet();

	/*! \brief 重置信号集
	    @return 成功则返回0, 否则返回-1并设置errno值
	*/
	int empty (void);

	/*! \brief 初始化信号集为所有在POSIX.1里定义的信号.
	    @return 成功则返回0, 否则返回-1并设置errno值
	*/
	int fill(void);

	/*! \brief 增加信号到信号集
	    @return 成功则返回0, 否则返回-1并设置errno值
	*/
	int add(int signo_);

	/*! \brief 从信号集里移除信号
	    @return 成功则返回0, 否则返回-1并设置errno值
	*/
	int del(int signo_);

	/*! \brief 检测某信号是否在信号集里
	    @return 成功则返回0, 否则返回-1并设置errno值
	*/
	int is_member(int signo_);

	/*! \brief 转换操作符
	 */
	operator sigset_t *();

private:
	sigset_t m_sigset;  /*!< POSIX信号集 */
};

inline
SigSet::
SigSet() { 	(int) sigemptyset(&m_sigset); }

inline
SigSet::
SigSet(sigset_t* s_) { m_sigset = *s_; }

inline 
SigSet::
~SigSet() { /* no-op */ }

inline int
SigSet::
empty(void) { return sigemptyset(&m_sigset); }

inline int
SigSet::
fill(void) { return sigfillset(&m_sigset); }

inline int
SigSet::
add(int signo_) { return sigaddset(&m_sigset,signo_); }

inline int
SigSet::
del(int signo_) { return sigdelset(&m_sigset,signo_); }

inline int
SigSet::
is_member(int signo_) { return sigismember(&m_sigset,signo_); }

inline 
SigSet::
operator sigset_t *() {	return &m_sigset; }

/** @} */

END_SUNRISE_NAMESPACE

#endif /* _SigSet_h */
