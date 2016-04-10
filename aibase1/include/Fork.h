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
 * Fork��Ĳ��Գ���
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
    \brief ϵͳ����fork()�İ�����(wrapper class)�Ķ���
    ����������ڲ�ֱ��ʹ����ϵͳ����fork�����ṩ�ӽ��̵���ֹ��ʽ.
*/

/*! \class Fork Fork.h 
   \brief ϵͳ����fork()�İ�����
    ����������ڲ�ֱ��ʹ����ϵͳ����fork�����ṩ���ӽ��̵���ֹ��ʽ.
 */
class Fork {
public:
	/*! \enum state_t
	   �ӽ������״̬
	*/
	enum state_t { 
		KILL_ON_EXIT,			/*!< �˳�ǰɱ�������ӽ��� */
		WAIT_ON_EXIT,			/*!< �˳�ǰҪ�ȴ����е��ӽ������˳� */
		LEAVE_ALONE				/*!< �˳�ǰ���������������е��ӽ��� */
	};

	/*! \brief ���캯��. 
	  	�����ӽ���
	    @param s_ ָ���Ƿ�ȴ��ӽ����˳���
	              (ȱʡ��WAIT_ON_EXIT), ����ͨ������SIGTERM�ź�ɱ���ӽ���.
	*/
	Fork (state_t s_ = WAIT_ON_EXIT);
	
	/*! \brief ��������
	*/
	~Fork() { trace_with_mask("Fork::~Fork",FORK); }

	/*! \brief ��⵱ǰ�Ĵ�����Ƿ����ڸ����̵�.
	    @return ����Ǹ����̴���η���true,���򷵻�false.
	*/
	bool isParent() const { return m_pid ? true : false; }

	/*! \brief ��⵱ǰ������Ƿ������ӽ��̵�.
	    @return ������ӽ��̴���η���true,���򷵻�false.
	*/
	bool isChild() const { return !m_pid ? true : false; }

	/*! \brief ȡ���ӽ���id
	    @return �����ӽ���id
	*/
	pid_t getChildPID() const {
		trace_with_mask("Fork::getChildPID",FORK);
		return m_pid;
	}

	/*! 
	 *  \brief ִ���ⲿ����
	 *  ����ϵͳ����fork()/execvp()/wait4().
	 *
	 *  @param cmd_ �ⲿ����.
	 *  @param args_ �ⲿ�������.
	 *  @param wait_for_completion_ 
	 *                ����趨Ϊtrue, ����̽�������֪���ⲿ����ִ�����.
	 *
	 *  @return ���wait_for_completion_�趨Ϊfalse, �����ӽ���id.
	 *          ���wait_for_completion_�趨Ϊtrue, �����ӽ����˳�״̬.
	 */
	static int fork_exec (const string& cmd_, 
						  const string& args_,  
						  bool wait_for_completion_);
	
private:
	pid_t m_pid; 
};

/*! \class fnode_t 
 * \brief fork�ڵ�
 */
class fnode_t {
public:
	/*! \brief ���캯��.
	 */
	fnode_t (pid_t pid_, Fork::state_t state_)
		: m_pid(pid_), m_state(state_)
		{
			trace_with_mask("fnode_t::fnode_t",FORK);
		}

	/*! \brief ȡ���ӽ���id
	 */
	pid_t getPID() const
		{
			trace_with_mask("fnode_t::getPID",FORK);
			return m_pid;
		}

	/*! \brief ȡ��ɱ�ӽ��̱�־.
	 */
	bool needKill() const
		{
			trace_with_mask("fnode_t::needKill",FORK);
			return  m_state == Fork::KILL_ON_EXIT ? true : false;
		}
private:
	/*! \brief �ӽ���id
	 */
	pid_t m_pid;

	/*! \brief �ӽ������״̬ {kill, wait}
	 */
	Fork::state_t m_state;
};

/*! \class ForkList Fork.h
 *  \brief ForkListά��һ��������fork�����ӽ�������ɵ��б�. 
 *  ������Ҫô�ȴ�ÿ���ӽ��̽�����Ҫôͨ��SIGTERM�ź�ɱ�����е��ӽ��̲Ż��˳�������
 *  ���ֽ�ʬ���̡�
 */
class ForkList
{
public:
	/*! \brief ���캯��
	 */
	ForkList () { trace_with_mask("ForkList::ForkList",FORK); }

	/*! \brief ��������
	 */
	~ForkList();

	/*! \brief �ӽ����б�.
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





