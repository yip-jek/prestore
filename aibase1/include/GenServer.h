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
    ͨ�÷�������(Service Configuratorģʽ)
*/

/*! \example genserver_test.cpp
 *  ͨ�÷���������Գ���
 */

/*! \class GenServer GenServer.h
 * \brief ͨ�÷�������
 * Ϊ��д�������ṩͨ�ÿ��
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
	/*! \brief ���캯��
	 * ����������<tt>IDLE</tt>״̬.
	 */
	GenServer ();

	/*! \brief ��������
	 */
	virtual ~GenServer() {}

	/*! \brief ִ�г�ʼ���Ľ����.
		�����׼�����в���������־�ļ�. ��һ�µķ�ʽ����������Щ�ź�:
	    SIGHUP, SIGPIPE, SIGCHLD, SIGCLD, SIGALRM, SIGINT, SIGPOLL,
	    SIGTERM. 

		 ����������ΪRUNNING״̬.
	  
	    @param argc Pointer to number of command line arguments
	    @param argv  Command line arguments char* array
	    @param help_info Title that will be displayed with -h option
	 */

	virtual void init (int* argc, char* argv[], const char* help_info);

	/*! \brief �������ظ�IDLE״̬.
	 */
	virtual int fini (void) { return 0; }
	
	/*! \brief ��ʱ���������, ����SUSPENDED״̬.
	 */
	virtual int suspend (void) { return 0; }

	/*! \brief �ָ�ִ��, ״̬�ָ�ΪRUNNING.
	 */
	virtual int resume (void) { return 0; }

	/*! \brief Ϊ�������ṩ�ĳ�ʼ���ӿ�
	 */
	virtual void initServer () =0;

	/*! \brief Ϊ�������ṩ�����ݴ������ӿڣ�ͨ�������¼�ѭ��.
	 */
	virtual void processServer () =0;

	/*! \brief ��OS������ֹ�ź�ʱ,Ϊ�������ṩ�Ķ�����������Ĺ��ӽӿ�
	 */
	virtual void fatalSignalHook () { /*--- empty ---*/ }

	/*! \brief �źŴ�����
	 */
	int handle_signal (int signum_);

	/*! \brief ����Ƿ����˳���־.
	  
	    @return true when 'graceful quit' flag has been raised; 
	            false otherwise 
	 */
	bool stopServer() { return m_graceful_quit; }

	/*! \brief �����˳���־. ֪ͨ������ֹͣ���ķ�������������ֹͣ���ݴ����˳�
	 */
	void setStopServerFlag ();

	/*! \brief ���ð汾

	    @param release_  Release number.
	    @param revision_ Patch level.
	*/
	void set_version (const string& release_, int revision_);

	/*! \brief ��ȡ�汾��Ϣ
	 */
	string get_version ();

	/*! \brief ����������
	 */
	void set_author (const string& author_);

	/** New debug information is added to the old log file.
	    To erase old log file, set flag to RMLOG.

	    @param logf_ Defaulted to KEEPLOG that adds log records to 
	                 the existing log file; RMLOG - remove existing log
	                 file and start afresh.
	*/
	void set_flags (LogFlag logf_) { m_log_flag = logf_; }

	/*! \brief ��ӡ�������в���
	 */
	virtual void displayHelp ();

	/*! \brief ���ؽ�������+ʵ������
	 */
	string   getProcName() { return m_proc_name; }

	/*! \brief ȡ������
	 */
	string   getCmdLineName() { return m_cmdline_name; }

	/*! \brief ȡȱʡ�����ļ���$HOME/.srbc/<process_name>
	    ���Ҫ�ı������ļ�����ֻҪ�������������޸ĳ�Աm_std_config_name
	*/
	string getStdConfigName() { return m_std_config_name; }
	    
	/*! \brief ȡ������ָ���������ļ���, ��������'-f'����ָ�������ļ���
	 */
	string getAltConfigName() { return m_alt_config_name; }

	/*! \brief ���ؼ����˿ڵļٶ���
	 */
	string getPortName() { return m_port; }

	/*! \brief ���ü����˿ڵļٶ���.
	    @param port_ new listening port name
	 */
	void setPortName (string port_) { m_port = port_; }

	/*! \brief �ı������
	    @param proc_name_ new process name
	 */
	void setProcName (string proc_name_) { m_proc_name = proc_name_; }

	/*! \brief ��ȡ�źŹ�����(SigHandlersʵ��)������.
	 */
	SigHandlers& get_sig_manager () { return m_sig_dispatcher; }
	
	/*! \brief ��ȡ���ķ�����������
	 */
    	Reactor* getReactor () { return &m_reactor; }

	/*! \brief ����Ϊ�ػ�����
	 */
	static bool becomeDaemon ();

protected:
	string   m_proc_name;            /*!< ���������ʵ������  */
	string   m_cmdline_name;         /*!< �����������еĽ����� */
	string   m_port;                 /*!< �����˿ڵļٶ��� */
	string   m_std_config_name;     /*!< ��׼�����ļ��� */
	string   m_alt_config_name;     /*!< ������ָ���������ļ��� */
	u_int    m_log_size;            /*!< ��־�ļ��Ĵ�С */ 
	int      m_instance;            /*!< ����ʵ��id */ 
	string   m_log_file;            /*!< ��־�ļ���ȫ·���� */
	long     m_debug_mask;          /*!< debug/error��Ϣ������ */
	bool     m_graceful_quit;       /*!< �����˳���־ */
	SigHandlers m_sig_dispatcher;   /*!< �źŴ�������� */
	SIGPOLLHandler m_sig_poll;      /*!<  SIGPOLL�źŴ����� */
	bool m_daemon;                  /*!< UNIX�ػ����̱�ʶ�����Ϊtrue������̻�
												 ��Ϊ�ػ����� */
	Reactor m_reactor;              /*!< ���ķ����� */
	string m_version;               /*!< �汾��Ϣ */
   int m_revision;                 /*!< �汾������Ϣ */
	string m_author;                /*!< ������ */
	const char* m_help_msg;         /*!< ������Ϣ */
	LogFlag m_log_flag;            /*!< ��־�ļ���ʼ����־�������ΪRM_LOG, ��
												�������𶯵�ʱ�򣬽�ɾ������־�ļ� */
	bool m_log_stdout_flag;       /*!< ��־��Ϣ�ض���std::cerr�ı�־ */
	bool m_daemon_flag;           /*!< �ػ�����������ѡ���־. */
	bool m_help_flag;            /*!< ������Ϣ��������ѡ���־, ���Ϊtrue, ��������
											��[-h, --help]���ӡ������Ϣ */
	bool m_version_flag;         /*!< �汾������ѡ���־�����Ϊtrue, ����������[-v, --help]����
											 ��ӡ���汾��Ϣ */
	bool m_ommit_pidflock_flag;  /*!< ����趨Ϊtrue, ������PID�ļ���������. */

private:
	PidFileLock m_pidfile_lock;  /*!< pid�ļ��� */
	string m_pidfile;            /*!< pid�ļ��� */

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
