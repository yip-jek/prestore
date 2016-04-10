#ifndef I_TASK_MONITOR_H
#define I_TASK_MONITOR_H

#include <list>

using std::list;

/** @defgroup libjk 监控控制的公共接口类(libjk)
@{
*/

/*! \file ITask.h
 *  \brief 监控控制的公共接口类
 */

/*! \class ITask
 *  \brief 监控控制的公共接口类
 */

/** \example exITask.cpp
 *  \brief ITask类示例程序 
 */

#include "THeartBeat.h"

class ITask
{
public:
	/*! 功能说明: 构造函数 */
	ITask();

	/*! 功能说明: 析构函数 */
	~ITask();

	/*! 功能说明: 设置任务ID. \n
	 *
	 *  任务ID是由进程的第一个参数穿入的整数
	 *  @param[in] iTaskID 任务ID
	 *  @return 无
	 */
	void setTaskID(int iTaskID);

	/*! 功能说明: 设置进程UNIX ID.\n
	 *
	 *  该方法更新数据库相应记录的UnixID字段
	 *  当环境变量HBENABLE==1时，初始化THeartBeat
	 *  @param[in] iUnixID Unix ID号
	 *  @return  返回 0 表示成功, -1 表示失败
	 */
	int setProcUnixID(int iUnixID);
	
	/*! 功能说明: 获取调度任务.发送THeartBeat消息
	 *
	 *  @param[in] pszCmdParam 
	 *  @param[in] iParamBufLen
	 *  @return 任务标识号,0为无任务、1为启动、2为停止、3为重置 
	 *  	     注：getTask不会返回1（启动），启动由控制代理完成
	 *           返回-1表示 getTask出错
	 */
	int getTask(char* pszCmdParam,int iParamBufLen);

	/*! 功能说明: 设置任务执行结果. \n
	 *
	 *
	 *  @param bSuccess 任务是否成功
	 *  @return 成功更新返回0，失败返回-1
	 */
	int setTaskResult(bool bSuccess);

	/*! 功能说明: 设置本进程停止标志.\n
	 *
	 *
	 *  @param bNormal 是否正常退出
	 *  @return 成功返回0，失败返回-1
	 */
	int setProcQuit(bool bNormal);

	/*! 功能说明: 获取最近一次错误信息.\n
	 *
	 *
	 * @return 错误信息字符串地址,用户不需要进行释放内存动作 
	 */
	const char* getLastError();

private:
	
	/* 当环境变量HBENABLE==1时，初始化THeartBeat , 成功返回0*/
	int init_hb();

private:
	/*! 任务组链表数据结构 */
	struct STASKDETAIL
	{
		/*! 详细任务的记录ID */
		int cid_id;
		/*! 命令类型 */
		int cct_id;
		/*! 参数 */
		char cmdparam[2048+1];
	};

	/*! 本任务cid_id */
	int  m_cid_id;

	/*! 目标cid_id */
	int  m_cue_target;

	/*! 错误信息缓冲区 */
	char m_errbuf[512+1];

	/*! 任务组链表 */
	list<STASKDETAIL> m_listTaskGrp;

	/*! 是否已声明任务游标 */
	bool m_bTaskDecl;

	/*! 是否已声明setProcUnixId's Prepare */
	bool m_bUnixIdDecl;

	/*! 是否已声明setTaskResult's Prepare */
	bool m_bResultDecl;

	/*! 是否已声明setProcQuit's Prepare */
	bool m_bQuitDecl;

	THeartBeat * m_hb;
	bool m_hbenable;
};

/** @} */

#endif
