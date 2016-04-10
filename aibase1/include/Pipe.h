//------------------------------------------------------------------------------
//                           Pipe.h
//------------------------------------------------------------------------------
#ifndef PIPE_H
#define PIPE_H

#include <stdio.h>
#include <string>

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "Logger.h"

BEGIN_SUNRISE_NAMESPACE

/** \example pipe_test.cpp
 * 管道类测试程序，创建子进程调用smoker, 并通过与smoker之间的管道(Pipe)通讯.
 */

/** \example smoker.cpp
 * 被用作子进程执行.
 */

/** \defgroup libsr 基础工具库(libsr)
 * \{*/
/** \file Pipe.h
 *  \brief UNIX系统调用popen()/pclose()的包裹类(wrapper class).
 */

/** \class Pipe Pipe.h
 * \brief UNIX系统调用popen()/pclose()的包裹类(wrapper class).
 */
/** \}*/
class Pipe 
{
public:
	/** \brief 缺省构造函数
	 */
	Pipe ();

	/** \brief 析构函数
	 * 调用close()方法关闭打开的管道.
	 */
	~Pipe ();

	/** \brief 启动一个子shell并执行命令字符串cmd_.
	 * 为子进程创建管道并因应参数type_指定的"r"(read)或"w"(write)把管道
	 * 粘附在子进程的标准输出和标准输入上.标准输入/输出的函数(fprintf(), fscanf()
	 * , fgets等)可以把信息在父子进程之间传递。
	 *
	 * @see Fork
	 * @param cmd_  执行的命令字符串
	 * @param type_ "w" for write pipe and "r" for read pipe
	 * @return 标准I/O流指针，如果错误，返回NULL并设置errno标明发生的错误.
	*/
	FILE* open (const string& cmd_, const string& type_);

	/** \brief 关闭管道.收集子进程状态确保子进程已结束
	 *
	 * @return 成功返回0; 否则返回-1.
	*/
	int close ();

	/** \brief 用SIGTERM信号kill子进程, 最好还是调用close()收集子进程状态
	 *
	 * @see close()
	 * @return 成功返回0, kill(2)失败返回-1.
	*/
	int kill ();

	/** \brief 取得子进程id. */
	pid_t pid () const;

	/** \brief 取得管道的标准I/O文件指针 */
	FILE* fp () const;

	/** \brief 取得管道的文件描述字 */
	int fd () const;

private:						
	Pipe (const Pipe&);
	Pipe& operator= (const Pipe&);

private:
	FILE*  m_fp; /**< I/O流指针 */
	pid_t  m_child_pid; /**< 子进程id */
};

inline pid_t 
Pipe::pid () const { return m_child_pid; }

inline int
Pipe::fd () const { return fileno (m_fp); }

inline FILE*
Pipe::fp () const { return m_fp; }

END_SUNRISE_NAMESPACE

#endif // PIPE_H

