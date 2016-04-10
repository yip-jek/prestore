#ifndef __OraCon_H__
#define __OraCon_H__


#include "devisdefs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>

#define USERNAME_LEN 64
#define PWD_LEN      64
#define DB_LEN       64
#define SERVER_LEN   64

#define MSG_LEN    4096

#define DB_ENV_LEN 100
#define DB_CRYPT "DB_CRYPT"
#define LINE_LEN 256



/** \defgroup libdb   数据库编程库(libdb) */
/** \{*/
/** \file TOraCon.h
 *  \brief oracle连接类的头文件  */

/** \class TOraCon
 *  \brief oracle连接类
 */

/** \example exDB.cpp 
 *  \example exInf.ecpp
 *  \brief TDBInstance TQueryEx TQueryOra TInformixEx TOraCon 类示例程序 
 */


/** \}*/

class TOraCon
{
public:

	TOraCon();

	virtual ~TOraCon();

public:
        /**\enum emStatus { NotInited,Connected,Disconnected }
         *  \brief 用于标记当前数据库状态的联合体
	 */
	enum emStatus { NotInited,Connected,Disconnected };  /* connection mode status  */

        /**\ enum emTransMode { Auto, Manual };
         *  \brief 用于标记事务处理的方式是用户定义,还是自动
	 */
	enum emTransMode { Auto, Manual };                   /* transaction mode status */

        /**\ emsavepoint {Point_A,Point_B,Point_C,Point_D,Point_E}
         *  \brief 用于标记事务的五个保留点
	 */
	enum emsavepoint {Point_A,Point_B,Point_C,Point_D,Point_E,Point_ALL} ;   /* savepoint step  */

private:
	char m_sUserName[USERNAME_LEN+1]; 
	char m_sPassword[PWD_LEN+1];     
	char m_sServer[SERVER_LEN+1];  
	char m_sLinkName[SERVER_LEN+1];

public:
                /** \brief 设置登录数据库用户名
                 *  \param sUserName - 用户名 [输入]
                 */
	void SetUserName(char *sUserName);     
                /** \brief 设置登录数据库密码
                 *  \param sPassword - 密码 [输入]
                 */
	void SetPassword(char *sPassword);    
                /** \brief 设置登录数据库服务器名
                 *  \param sDatabase - 数据库服务器名 [输入]
                 */
	void SetServer(char *sServer);      
                /** \brief 设置连接数据库标识名
                 *  \param sLinkName - 数据库标识名 [输入]
                 */
	void SetLinkName(char *sLinkName);      

private:
	long m_nCode;                      
	char m_sMsg[MSG_LEN];                

	enum emStatus      m_emStatus;   
	enum emTransMode   m_emTransMode;

public:

        /** \brief 数据库连接,
	 *  \param sUserName  登录用户;NULL则通过服务器名sUserName名查加密文件后连接　
	 *  \param sPassword  登录密码;NULL则通过服务器名sUserName查加密文件后连接　
	 *  \param sUserName  登录数据库的服务器,一般用于远程登录，缺省为本地('@'后部分);　
	 *  \param linkname   连接名，非空值时，SQL对这个数据库的操作SQL要加 AT :linkname;
         *  \return 成功返回true，失败返回false;
         */
	BOOL Connect(char *sUserName,char *sPassword,char *sServer,char *linkname);


        /** \brief 数据库连接,通过读配置文件$devis_home/devis/restrict/$ORACLE_SID实现本地登录
         *  \return 成功返回true，失败返回false;
         */
	BOOL oraconnect();
       

        /** \brief 断开连接
         *\return 成功返回true，失败返回false;
         */
        BOOL Disconnect();        

        /** \brief 检查数据库是否活跃
        * \return 活跃状态返回true，否则返回false;
         */
	BOOL CheckDBActive() const ;          

protected:
 	BOOL IsConnected() const; 

	bool CheckError(); 

	void FreeResource();

public:

	  /**\brief 根据服务器名到加密文件DB_CRYPT查找对应的帐户密码
            * \param sServer 要连接的数据库服务器名,由Oracle的tnsnames.ora定义
            * \return 出错为false;否则为true;
            */
	BOOL ScanDesfile(char * sServer);
             /**\brief 分析输入串的各字段信息
              * \param env 要分析的字符串
              * \param sServer 帐户
              * \param sServer 密码
              * \param sServer 数据库，对于ORACLE无效
              * \param sServer 要连接的数据库服务器名
              * \return 出错为false;否则为true;
              */
	BOOL SplitEnv(const char* env, char* user, char* pwd, char* db, char* host);

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

        /** \brief 声明事务开始,没有实现
	 *  \return true
         */
	BOOL BeginTrans();

        /** \brief 设置事务保留点,最大可以保留5个
         *  \param step -要保留的事务标识 取值为0-4,缺省为0 [输入]        
         *  \return 如果超过事务标识值范围则返回false,否则为true;
         */
	BOOL SaveTrans(emsavepoint step=Point_A);


        /** \brief 设置事务回滚点,对应于事务保留
         *  \param step -要回滚的事务标识 取值为0-4,缺省为0,其它为对所有事务作回滚[输入]        
         *  \return 返回true;
         */
	BOOL RollbackTrans(emsavepoint step=Point_ALL);

        /** \brief 提交所有事务
        * \return  成功返回true，否则返回false;
         */
	BOOL CommitTrans();

        /** \brief 中断一个事务,oracle对此无实现
        * \return 成功返回true，否则返回false;
         */
	BOOL BreakTrans();
};

/** \brief 检查是否发生了SQL错误,返回错误信息
 *  \param outstr 错误信息缓冲
 *  \param outstrlen 错误信息缓冲长度
 *  \return 返回SQLCODE,0表示没有错误.
 */
int ChkError(char* outstr, int outstrlen);

#endif
