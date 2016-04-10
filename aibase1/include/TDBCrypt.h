#ifndef __TDBCRYPT_H__
#define __TDBCRYPT_H__

#define USING_OLD_IOSTREAM
#include <map>
#include <string>

#include "TInformixEx.h"

using std::map;
using std::string;

#define DB_CRYPT "DB_CRYPT"
#define LINE_LEN 256
#define DB_ENV_LEN 100
#define DB_ID 200


/** \example TDBC_Test.cpp
 * 环境类测试程序
 */

/** \defgroup libdb   数据库编程库(libdb) */
/** \{*/
/** \file TDBCrypt.h
 *  \brief TDBCrypt类头文件.*/

/** \class TDBCrypt
 * \brief 环境类 - 从环境变量DB_CRYPT所指定的文件中解密出连接数据库的用户名/密码,数据库/主机.
 *     配套的还有adddb命令.
 */

/** \}*/


class TDBCrypt {
	public:
		typedef struct DBEnv_ {
			char user[DB_ENV_LEN+1];
			char pwd[DB_ENV_LEN+1];
			char db[DB_ENV_LEN+1];
			char host[DB_ENV_LEN+1];
			TInformixEx* con;
		}DBEnv;

	public:
		typedef map<string, DBEnv*> DBENV_MAP;
		typedef map<string, DBEnv*>::iterator DBENV_MAP_IT;

	public:
		/** \brief 构造函数
		 */
		TDBCrypt();

		/** \brief 析构函数
		 */
		virtual ~TDBCrypt();

		/** \breif 设置自动释放
		 */
		void setAutoDelete(bool enable = true);

		/** \brief 初始化
		 * 从环境变量DB_CRYPT读入密文文件，分析出数据库相关的用户名／密码，数据库名／主机名对。
		 */
		virtual bool init(bool connect = false);

		/** \brief 获取某数据库instance的连接资料(包括用户名, 密码，数据库名，主机名
		 * @param id - 数据库instance标识. 格式是 database@host(例如gmcc_boss@boss)
		 * @param user - 用户名 [输出]
		 * @param pwd - 密码 [输出]
		 * @param db - 数据库名 [输出]
		 * @param host - 主机名 [输出]
		 *
		 * @return 成功取得记录返回true, 并在user,pwd,db,host等输出参数里面有返回值，否则
		 *            返回false, user,pwd,db,host都不能用。
		 *            注： user,pwd,db,host必需在用完后释放掉.delete [] user;...
		 */
		bool fetchUserPwd(const char * id, char** user, char** pwd, char** db, char** host);

		/** \brief 获取当前连接池内第一个数据库连接
		 */
		TInformixEx* getCurrentConnection(void);

		/** \brief 获取数据库连接句柄
		 *
		 * @param id - 数据库instance标识. 格式是 database@host(例如gmcc_boss@boss)
		 */
		TInformixEx* useDBConnection(const char * id);


		/** \brief 获取数据库连接句柄
		 *
		 */
		const TInformixEx* getDBConnection();


		/** \brief 释放资源 - 如果设置autodelete为false，要手工释放资源.
		 */
		void Destroy(void);

	private: //不允许赋值语义 和 拷贝语义*
		TDBCrypt(const TDBCrypt& e);
		TDBCrypt& operator=(const TDBCrypt& e); 
		const TDBCrypt& operator=(const TDBCrypt& e) const; 

	private:
		bool SplitEnv(const char* env, char* user, char* pwd, char* db, char* host);
	
	private:
		DBENV_MAP m_DBMap;

	protected:
		TInformixEx*  m_dbconnection;
		bool m_AutoDel;
		bool m_SetEnv; /**< 标识环境是否设置好 */
		bool m_FOK;  /**< 标识加密文件是否能读 */
		bool m_Connected; /**< 标识是否已经连接数据库 */
};

#else
class TDBCrypt;
#endif
