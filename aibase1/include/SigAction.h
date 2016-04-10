//------------------------------------------------------------------------------
//                            SigAction.h
//------------------------------------------------------------------------------

#ifndef _SigAction_h
#define _SigAction_h

// System includes
//
#include <signal.h>
#include <errno.h>

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "Assert.h"
#include "SigSet.h"

// some convenient typedefs
//
extern "C" {
typedef struct sigaction SIGACTION;
typedef void (*C_SIG_HANDLER)( int );
}

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr  信号处理模块(libsr)  
@{ 
*/


/*! \example sighand_test.cpp
 * SigAction的测试程序
 */

/*! \file SigAction.h

\brief SigAction是UNIX系统中sigaction结构的C++包裹类

SigAction类实现sigaction结构的包裹,并提供所有可能在sigaction上
的操作，包括sigaction(2)系统调用

sigaction结构定义是：

<pre>
   struct sigaction {
       void      (*sa_handler) ();
       sigset_t  sa_mask;
       int       sa_flags;
       void      (*sa_sigaction) (int, siginfo_t*, void*);
   };
</pre>

用于设定你的进程在某信号到达的时候执行什么操作。SigAction类封装
在特定信号发生时的动作。

最重要的成员是存放函数指针的sa_handler,当进程收到某信号是这个函数
将被调用。信号定义在POSIX.1规范里.

SigAction类构造函数的参数C_SIG_HANDLER
会是以下的选项之一:
   - "C" 信号处理函数(sigaction结构的sa_handler):\n
      void sig_handler (int signum)
   - SIG_ERR 
   - SIG_DFL 使用系统缺省的信号处理函数,通常就是简单调用abort，使进程退出。
   - SIG_IGN 忽略信号(当然, signal 9不能被忽略)

sa_mask掩码指定当信号处理函数正在处理的时候哪些信号将被阻塞.
另外，一些信号也能使正在执行的信号处理函数阻塞(SIGSTOP和SIGKILL
不能被阻塞).

sa_flags标志集(由个flag按位与组成)指定如何影响信号处理函数
的执行时或者执行后的行为.具体的OS会有些区别。
可通过man 2 sigaction命令查看.

SigAction作为sigaction(2)的包裹类，要适应各主流的OS.
在sig_handler处理返回(或者处理中),操作系统将重新安装或者重置
当前的信号处理函数.要重置为SIG_DFL, SUN Solaris使用SA_RESETHAND
标志，FreeBSD和Linux OS都使用SA_ONESHOT标志，而AIX使用SA_RESETHAND.
麻烦的移植问题. :)
*/
/*! \class SigAction SigAction.h
 * \brief SigAction是UNIX系统中sigaction结构的C++包裹类
 */
class SigAction
{
public:
	/*! \brief 缺省构造函数.
	*/
	SigAction();

	/*! \brief 构造函数
	    用信号处理函数构造SigAction对象.

	    @param handler_ 信号处理函数指针
	    @param sig_mask_ 当信号处理函数执行时阻塞的信号集
	    @param flags_ 控制信号处理函数行为的标志集(特定于操作系统, 参看manpages)
	*/
	SigAction (C_SIG_HANDLER handler_, 
			   SigSet*       sig_mask_ = 0, 
			   int           flags_ = 0);

	/*! \brief 构造函数
	  	 用信号处理函数指针构造SigAction对象并立即部署信号signum_.<p>
	    第一个参数是函数指针.可以是非静态的C++类成员函数。@see SigHandler

	    @param handler_ 信号处理函数指针
	    @param signum_  部署的信号
	    @param sig_mask_ 信号处理函数执行的时候所阻塞的信号集
	    @param flags_ 控制信号处理函数行为的标志集(特定于操作系统, 参看manpages)
	*/
	SigAction (C_SIG_HANDLER handler_,
			   int           signum_,
			   SigSet*       sig_mask_ = 0, 
			   int           flags_ = 0);

	/*! \brief 作为signum_当前的信号部署自注册, 旧的部署存放在oaction_。

	    @param signum_ 信号
	    @param oaction_ 旧部署的存放处
	    @return 成功是返回0, 否则返回-1,并设定errno值
	*/
	int register_action (int signum_, SigAction* oaction_ = 0);

	/*! \brief 把signum_的信号部署修改为oaction_,

	    @param signum_ 被修改的信号
	    @param oaction_ 信号部署
	    @return 成功是返回0, 否则返回-1,并设定errno值
	*/
	int restore_action (int signum_, SigAction& oaction_);

	/*! \brief 取得信号signum_当前的部署
	    @param signum_ 信号
	    @return 成功是返回0, 否则返回-1,并设定errno值
	*/
	int retrieve_action (int signum_);

	/*! \brief 设置sigaction
	    @param sa_ 新的sigaction值
	*/
	void action (SIGACTION * sa_);

	/*! \brief 返回当前的sigaction实例指针
	    @return 返回当前的sigaction实例指针
	*/
	SIGACTION * action ();

	/*! \brief 设置标志集(影响信号处理函数行为的标志集)
	    @param new_flags_ 新的标志集
	*/
	void flags (int new_flags_);

	/*! \brief 获取当前的标志集
	    @return  返回当前的标志集值
	*/
	int flags ();

	/*! \brief 设置新的信号掩码
	 */
	void mask (SigSet & mask_set_);

	/*! \brief 获取当前的信号掩码
	 */
	SigSet mask ();

	/*! \brief 设置新的信号处理函数
	 */
	void handler (C_SIG_HANDLER sha_);

	/*! \brief 获取当前的信号处理函数指针
	 */
	C_SIG_HANDLER handler ();

	/*! \brief 类型转换操作符
	  把SigAction对象转换为内部sigaction结构的函数指针
	*/
	operator SIGACTION *();
  
private: 
	SIGACTION m_sa;  /*!< sigaction结构实例  */
};

/** @} */

END_SUNRISE_NAMESPACE

#endif /* _SigAction_h */
