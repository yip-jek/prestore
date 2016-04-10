
/*
 *
 *    Informix ESQL Emmable SQL C/C++ API Interface. DML Operation
 * 
 * \Author  : devis
 * \Date    : 2002.11.25
 * \Version : 01.01.001
 * \History : 
 */

#ifndef __ESQL_INFORMIX_EX_H__
#define __ESQL_INFORMIX_EX_H__

#include "devisdefs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>


#define USERNAME_LEN 64
#define PWD_LEN      64
#define DB_LEN       64
#define SERVER_LEN   64

/** \defgroup libdb   数据库编程库(libdb) */
/** \{*/
/** \file TInformixEx.h  
 *  \brief TInformixEx连接类的头文件  */

/** \class TInformixEx
 *  \brief TInformixEx连接类 */

/** \example exDB.cpp 
 *  \example exInf.ecpp
 *  \brief TDBInstance TQueryEx TQueryOra TInformixEx TOraCon 类示例程序 
 */


/** \}*/

////////////////////////////////////////////////////////////////////////////////////////
class TInformixEx
{
public:
	/** \brief 构造函数
	 */
	TInformixEx();
	/** \brief 析构函数
	 */
	virtual ~TInformixEx();

public:
	/**\enum emStatus { NotInited,Connected,Disconnected }
		 *  \brief 用于标记当前数据库状态的联合体 */
	enum emStatus { NotInited,Connected,Disconnected };  /* connection mode status  */
	/**\ enum emTransMode { Auto, Manual };
		 *  \brief 用于标记事务处理的方式是用户定义,还是自动*/
	enum emTransMode { Auto, Manual };                   /* transaction mode status */

private:
	char m_sUserName[USERNAME_LEN+1]; 
	char m_sPassword[PWD_LEN+1];     
	char m_sDatabase[DB_LEN+1];     
	char m_sServer[SERVER_LEN+1];  
	char m_sDBID[DB_LEN+SERVER_LEN+1];

public:
                /** \brief 设置登录数据库用户名
		 *  \param sUserName - 用户名 [输入]			
                 */
	void SetUserName(char *sUserName);     
                /** \brief 设置登录数据库密码
		 *  \param sPassword - 密码 [输入]			
                 */
	void SetPassword(char *sPassword);    
                /** \brief 设置登录数据库名
		 *  \param sDatabase - 数据库名 [输入]			
                 */
	void SetDatabase(char *sDatabase);   
                /** \brief 设置登录数据库服务器名
		 *  \param sDatabase - 数据库服务器名 [输入]			
                 */
	void SetServer(char *sServer);      

private:
	long m_nCode;                      
	char m_sMsg[4096];                

	enum emStatus      m_emStatus;   
	enum emTransMode   m_emTransMode;

public:
	/** \brief 数据库连接
	* \return 成功返回true，失败返回false;
	 */
	BOOL Connect(char *sUserName,char *sPassword,char *sDatabase,char *sServer);
	/** \brief 断开连接
	* \return 成功返回true，失败返回false;
	 */
	BOOL Disconnect();              
	/** \brief 检查数据库是否活跃
	* \return 活跃状态返回true，否则返回false;
	 */
	BOOL CheckDBActive() const;          

protected:
	/** \brief 检查连接状态
	* \return 连接成功返回true，否则返回false;
	 */
 	BOOL IsConnected() const; 
	/** \brief 检查错误状态
	 */
	void CheckError(); 
	/** \brief 释放资源
	 */
	void FreeResource();

public:
	/** \brief 获取错误信息
	 *  \param nErrorCode - 用于保存数据库错误代码的变量 [输出]			
	 *  \param sErrorMsg - 用于保存数据库错误信息的缓存 [输出]			
	 */
	void GetLastError(long &nErrorCode,char *sErrorMsg);
	/** \brief 设置错误信息
	 *  \param nErrorCode - 错误代码的值 [输入]			
	 *  \param sErrorMsg - 错误信息 [输入]			
	 */
	void SetLastError(long nErrorCode = 0,char *sErrorMsg = NULL); 
	/** \brief 开始一个事务
	* \return 成功返回true，否则返回false;
	 */
	BOOL BeginTrans();
	/** \brief 回滚一个事务
	* \return 成功返回true，否则返回false;
	 */
	BOOL RollbackTrans();
	/** \brief 提交一个事务
	* \return 成功返回true，否则返回false;
	 */
	BOOL CommitTrans();
	/** \brief 中断一个事务
	* \return 成功返回true，否则返回false;
	 */
	BOOL BreakTrans();
};

/** \brief 检查是否发生了SQL错误,返回错误信息
 *  \param outstr 错误信息缓冲
 *  \param outstrlen 错误信息缓冲长度
 *  \return 返回SQLCODE,0表示没有错误.
 */
int ChkError(char* outstr,int outstrlen);

/** \brief 取得Informix一个错误代码对应的具体错误信息
 *  \param sqlcode 需要查找的错误代码
 *  \param isamcode isamcode
 *  \return 返回具体错误信息,是在函数里面new出来的,调用者要负责释放
 */
char* getInformixErrMsg(long int sqlcode, long int isamcode);

#endif
