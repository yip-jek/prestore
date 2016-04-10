#ifndef GENSERVER_H
#define GENSERVER_H

extern "C" {
#include <stdio.h>              /* printf, sprintf         */
#include <unistd.h>
#include <stdlib.h>				/* getopt()                */
#include <string.h>             /* strlen                  */
#include <errno.h>              /* errno                   */
#include <signal.h>             /* kill                    */
#include <sys/types.h>          /* open                    */
#include <sys/stat.h>           /* open                    */
#include <fcntl.h>              /* open                    */
#include <limits.h>             /* PATH_MAX                */
#include <sys/resource.h>		/* getrlimit               */
#include <syslog.h>
#include <assert.h>
}

#include <string>
#include <vector>

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif
#include "Assert.h"
#include "Handlers.h"
#include "SigHandlers.h"
#include "Fork.h"
#include "Reactor.h"
#include "CmdLineOpts.h"
#include "PidFileLock.h"

using std::string;
using std::vector;

BEGIN_SUNRISE_NAMESPACE
/*! \file GenServer.h
    通用服务器类(Service Configurator模式)
*/

/*! \example genserver_test.cpp
 *  通用服务器类测试程序
 */

/*! \class GenServer GenServer.h
 * \brief 通用服务器类
 * 为编写服务器提供通用框架
 */
class GenServer : 
	public virtual EventHandler, 
	public CmdLineOpts
{
public:
	/*! \enum LogFlag
	 */
	enum LogFlag { 
		KEEPLOG,				/**< By default, append new log records to 
								   the  existing log file. This is operational 
								   mode. */
		RMLOG					/**< Remove existing log file and start afresh.
								   Convenient during development phase. */
	};

public:
	/*! \brief 构造函数
	 * 服务器处于<tt>IDLE</tt>状态.
	 */
	GenServer ();

	/*! \brief 析构函数
	 */
	virtual ~GenServer() {}

	/*! \brief 执行初始化的接入点.
		处理标准命令行参数，打开日志文件. 以一致的方式处理下面这些信号:
	    SIGHUP, SIGPIPE, SIGCHLD, SIGCLD, SIGALRM, SIGINT, SIGPOLL,
	    SIGTERM. 

		 服务器设置为RUNNING状态.
	  
	    @param argc Pointer to number of command line arguments
	    @param argv  Command line arguments char* array
	    @param help_info Title that will be displayed with -h option
	 */

	virtual void init (int* argc, char* argv[], const char* help_info);

	/*! \brief 服务器回复IDLE状态.
	 */
	virtual int fini (void) { return 0; }
	
	/*! \brief 暂时挂起服务器, 进入SUSPENDED状态.
	 */
	virtual int suspend (void) { return 0; }

	/*! \brief 恢复执行, 状态恢复为RUNNING.
	 */
	virtual int resume (void) { return 0; }

	/*! \brief 为衍生类提供的初始化接口
	 */
	virtual void initServer () =0;

	/*! \brief 为衍生类提供的数据处理主接口，通常是主事件循环.
	 */
	virtual void processServer () =0;

	/*! \brief 当OS传递终止信号时,为衍生类提供的额外清理操作的钩子接口
	 */
	virtual void fatalSignalHook () { /*--- empty ---*/ }

	/*! \brief 信号处理函数
	 */
	int handle_signal (int signum_);

	/*! \brief 检测是否有退出标志.
	  
	    @return true when 'graceful quit' flag has been raised; 
	            false otherwise 
	 */
	bool stopServer() { return m_graceful_quit; }

	/*! \brief 设置退出标志. 通知服务器停止中心分派器的运作，停止数据处理并退出
	 */
	void setStopServerFlag ();

	/*! \brief 设置版本

	    @param release_  Release number.
	    @param revision_ Patch level.
	*/
	void set_version (const string& release_, int revision_);

	/*! \brief 获取版本信息
	 */
	string get_version ();

	/*! \brief 设置作者名
	 */
	void set_author (const string& author_);

	/** New debug information is added to the old log file.
	    To erase old log file, set flag to RMLOG.

	    @param logf_ Defaulted to KEEPLOG that adds log records to 
	                 the existing log file; RMLOG - remove existing log
	                 file and start afresh.
	*/
	void set_flags (LogFlag logf_) { m_log_flag = logf_; }

	/*! \brief 打印程序运行参数
	 */
	virtual void displayHelp ();

	/*! \brief 返回进程名字+实例号码
	 */
	string   getProcName() { return m_proc_name; }

	/*! \brief 取命令行
	 */
	string   getCmdLineName() { return m_cmdline_name; }

	/*! \brief 取缺省配置文件名$HOME/.srbc/<process_name>
	    如果要改变配置文件名，只要在衍生类里面修改成员m_std_config_name
	*/
	string getStdConfigName() { return m_std_config_name; }
	    
	/*! \brief 取命令行指定的配置文件名, 命令行用'-f'可以指定配置文件名
	 */
	string getAltConfigName() { return m_alt_config_name; }

	/*! \brief 返回监听端口的假定名
	 */
	string getPortName() { return m_port; }

	/*! \brief 设置监听端口的假定名.
	    @param port_ new listening port name
	 */
	void setPortName (string port_) { m_port = port_; }

	/*! \brief 改变进程名
	    @param proc_name_ new process name
	 */
	void setProcName (string proc_name_) { m_proc_name = proc_name_; }

	/*! \brief 获取信号管理器(SigHandlers实例)的引用.
	 */
	SigHandlers& get_sig_manager () { return m_sig_dispatcher; }
	
	/*! \brief 获取中心分派器的引用
	 */
    	Reactor* getReactor () { return &m_reactor; }

	/*! \brief 设置为守护进程
	 */
	static bool becomeDaemon ();

protected:
	string   m_proc_name;            /*!< 进程名外加实例号码  */
	string   m_cmdline_name;         /*!< 出现在命令行的进程名 */
	string   m_port;                 /*!< 监听端口的假定名 */
	string   m_std_config_name;     /*!< 标准配置文件名 */
	string   m_alt_config_name;     /*!< 命令行指定的配置文件名 */
	u_int    m_log_size;            /*!< 日志文件的大小 */ 
	int      m_instance;            /*!< 进程实例id */ 
	string   m_log_file;            /*!< 日志文件的全路径名 */
	long     m_debug_mask;          /*!< debug/error信息的掩码 */
	bool     m_graceful_quit;       /*!< 进程退出标志 */
	SigHandlers m_sig_dispatcher;   /*!< 信号处理分派器 */
	SIGPOLLHandler m_sig_poll;      /*!<  SIGPOLL信号处理器 */
	bool m_daemon;                  /*!< UNIX守护进程标识，如果为true，则进程会
												 变为守护进程 */
	Reactor m_reactor;              /*!< 中心分派器 */
	string m_version;               /*!< 版本信息 */
   int m_revision;                 /*!< 版本补丁信息 */
	string m_author;                /*!< 作者名 */
	const char* m_help_msg;         /*!< 帮助信息 */
	LogFlag m_log_flag;            /*!< 日志文件初始化标志，如果设为RM_LOG, 在
												服务器起动的时候，将删除旧日志文件 */
	bool m_log_stdout_flag;       /*!< 日志信息重定向到std::cerr的标志 */
	bool m_daemon_flag;           /*!< 守护进程命令行选项标志. */
	bool m_help_flag;            /*!< 帮助信息的命令行选项标志, 如果为true, 在命令行
											用[-h, --help]则打印帮助信息 */
	bool m_version_flag;         /*!< 版本命令行选项标志，如果为true, 在命令行用[-v, --help]可以
											 打印出版本信息 */
	bool m_ommit_pidflock_flag;  /*!< 如果设定为true, 则跳过PID文件锁定步骤. */

private:
	PidFileLock m_pidfile_lock;  /*!< pid文件锁 */
	string m_pidfile;            /*!< pid文件名 */

private:
	/// No cloning 
	GenServer (const GenServer&);
	GenServer& operator=(const GenServer&);

	/// Initialize internals
	void initInternals ();
};


inline void
GenServer::
setStopServerFlag ()
{
	m_graceful_quit = true; 
	m_reactor.deactivate ();
}

inline void
GenServer::
set_version (const string& release_, int revision_)
{
	m_version = release_;
	m_revision = revision_;
}

inline void
GenServer::
set_author (const string& author_)
{
	m_author = author_;
}

END_SUNRISE_NAMESPACE

#endif /* GENSERVER_H */
