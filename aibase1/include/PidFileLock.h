//------------------------------------------------------------------------------
//                           PidFileLock.h
//------------------------------------------------------------------------------
#ifndef FILE_LOCK_H
#define FILE_LOCK_H

//System Includes
#include <sys/types.h>

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif
#include "Assert.h"

//STL
#include <string>
using std::string;

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr 基础工具库(libsr)
 * \{*/
/** \file PidFileLock.h
 *  \brief 创建和管理进程PID锁文件
*/

/** \class PidFileLock PidFileLock.h
 * \brief 进程PID锁文件管理类
 */
/** \}*/
class PidFileLock   : public flock
{
public:
	/** \brief 构造函数
	 */
	PidFileLock ();

	/** \brief 析构函数
	 * 如果进程拥有进程锁文件，在析构函数里将删除这个文件
	*/
	~PidFileLock ();

	/** \brief 锁文件
	    \return 如果成功，返回true.否则返回false.
	*/
	bool lock (const string& filename_);

	/** \brief 返回最后那次系统调用所产生的<TT>errno</TT>值. 
	    \return 无错误返回0, 否则返回<TT>errno</TT>.
	*/
	int get_error () const;
	
	/** \brief 返回<TT>errno</TT>所标识的错误信息
	*/
	const char* get_error_msg () const;

	/** \brief 把lock的状态信息写入debug文件
	 * <TT>m_fd = -1</TT>标识lock文件没有打开.
	*/
	void dump ();

private:
	/** \brief 对文件上锁
	    \return 成功上锁，返回0, 出现错误返回-1,并且在文件被其他的进程锁定了
		 时，errno设定为EAGAIN或者EACCES.
	*/
	int lock_region ();

	/** \brief 测试文件是否能被进程上锁.
	    \return 成功(文件可以被上锁或者已经被本进程锁定)时返回0, 失败时返回锁定
		 了这个文件的那个进程的id.
	*/
	pid_t test_region ();

	/** \brief 记录错误信息并设置errno.
	*/
	void log_error   (const char* msg_);

private:
	string m_filename; /**< 锁文件名 */
	int    m_fd; /**< 锁文件描述字 */
	int    m_error; /**< 最后一次系统调用产生的errno */
	string m_error_msg; /**< 错误信息 */
};

END_SUNRISE_NAMESPACE
#endif /* FILE_LOCK_H */  
