//------------------------------------------------------------------------------
//                             Fork.h
//------------------------------------------------------------------------------
#ifndef IS_FORK_H
#define IS_FORK_H


#include <unistd.h>		// fork
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif
#include "Assert.h"

#ifdef sun
#ifndef USE_SIMPLE_SINGLETON
#define USE_SIMPLE_SINGLETON 1
#endif
#endif

#ifdef USE_SIMPLE_SINGLETON
#include "SSingleton.h"
#else
#include "Singleton.h"
#endif

#include <list>
using std::list;

/*! \example fork_test.cpp
 * Fork类的测试程序
 */

/* Sun Solaris 2.6 has wait4(3C) function definition missing
 * from its header files. The function, however, is in the 
 * standard library. Testing this scenario would require
 * writing custom m4 macro. 
 */
#if defined(__sun) 
#include <sys/time.h>
#include <sys/resource.h>
extern "C" pid_t 
wait4(pid_t pid, int *statusp, int options, struct rusage *rusage);
#endif

BEGIN_SUNRISE_NAMESPACE

/*! \file Fork.h
    \brief 系统调用fork()的包裹类(wrapper class)的定义
    这个包裹类内部直接使用了系统调用fork，并提供子进程的终止方式.
*/

/*! \class Fork Fork.h 
   \brief 系统调用fork()的包裹类
    这个包裹类内部直接使用了系统调用fork，并提供了子进程的终止方式.
 */
class Fork {
public:
	/*! \enum state_t
	   子进程完成状态
	*/
	enum state_t { 
		KILL_ON_EXIT,			/*!< 退出前杀死所有子进程 */
		WAIT_ON_EXIT,			/*!< 退出前要等待所有的子进程先退出 */
		LEAVE_ALONE				/*!< 退出前忽略所有正在运行的子进程 */
	};

	/*! \brief 构造函数. 
	  	创建子进程
	    @param s_ 指定是否等待子进程退出。
	              (缺省是WAIT_ON_EXIT), 或者通过发送SIGTERM信号杀死子进程.
	*/
	Fork (state_t s_ = WAIT_ON_EXIT);
	
	/*! \brief 析构函数
	*/
	~Fork() { trace_with_mask("Fork::~Fork",FORK); }

	/*! \brief 检测当前的代码段是否属于父进程的.
	    @return 如果是父进程代码段返回true,否则返回false.
	*/
	bool isParent() const { return m_pid ? true : false; }

	/*! \brief 检测当前代码段是否属于子进程的.
	    @return 如果是子进程代码段返回true,否则返回false.
	*/
	bool isChild() const { return !m_pid ? true : false; }

	/*! \brief 取得子进程id
	    @return 返回子进程id
	*/
	pid_t getChildPID() const {
		trace_with_mask("Fork::getChildPID",FORK);
		return m_pid;
	}

	/*! 
	 *  \brief 执行外部命令
	 *  包裹系统调用fork()/execvp()/wait4().
	 *
	 *  @param cmd_ 外部命令.
	 *  @param args_ 外部命令参数.
	 *  @param wait_for_completion_ 
	 *                如果设定为true, 则进程将被阻塞知道外部命令执行完毕.
	 *
	 *  @return 如果wait_for_completion_设定为false, 返回子进程id.
	 *          如果wait_for_completion_设定为true, 返回子进程退出状态.
	 */
	static int fork_exec (const string& cmd_, 
						  const string& args_,  
						  bool wait_for_completion_);
	
private:
	pid_t m_pid; 
};

/*! \class fnode_t 
 * \brief fork节点
 */
class fnode_t {
public:
	/*! \brief 构造函数.
	 */
	fnode_t (pid_t pid_, Fork::state_t state_)
		: m_pid(pid_), m_state(state_)
		{
			trace_with_mask("fnode_t::fnode_t",FORK);
		}

	/*! \brief 取得子进程id
	 */
	pid_t getPID() const
		{
			trace_with_mask("fnode_t::getPID",FORK);
			return m_pid;
		}

	/*! \brief 取得杀子进程标志.
	 */
	bool needKill() const
		{
			trace_with_mask("fnode_t::needKill",FORK);
			return  m_state == Fork::KILL_ON_EXIT ? true : false;
		}
private:
	/*! \brief 子进程id
	 */
	pid_t m_pid;

	/*! \brief 子进程完成状态 {kill, wait}
	 */
	Fork::state_t m_state;
};

/*! \class ForkList Fork.h
 *  \brief ForkList维护一个由数个fork出来子进程所组成的列表. 
 *  主进程要么等待每个子进程结束，要么通过SIGTERM信号杀死所有的子进程才会退出。避免
 *  出现僵尸进程。
 */
class ForkList
{
public:
	/*! \brief 构造函数
	 */
	ForkList () { trace_with_mask("ForkList::ForkList",FORK); }

	/*! \brief 析构函数
	 */
	~ForkList();

	/*! \brief 子进程列表.
	 */
	list< fnode_t* > m_list;
};

#ifndef USE_SIMPLE_SINGLETON

#ifdef NEED_NAMESPACE
typedef sunrise::SingletonHolder<ForkList, CreateUsingNew> SingleForkList;
#define FORKLIST sunrise::SingleForkList::Instance()
#else
typedef SingletonHolder<ForkList, CreateUsingNew> SingleForkList;
#define FORKLIST SingleForkList::Instance()
#endif

#else

typedef SingletonHolder<ForkList> SingleForkList;
#define FORKLIST SingleForkList::Instance()

#endif

END_SUNRISE_NAMESPACE

#endif // IS_FORK_H





