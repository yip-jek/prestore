#ifndef __TDBCONNECTION_H__
#define __TDBCONNECTION_H__


#include "Prelude.h"
#include "Logger.h"


/* 全局唯一实例处理 */
#ifndef USE_SIMPLE_SINGLETON  /* USING AIX */
	#include "Singleton.h"
	#define SR_SH_OPTION , CreateUsingNew
#else                         /* USING SUN */
	#include "SSingleton.h"
	#define SR_SH_OPTION
#endif

//USING_SUNRISE;
#define SR_FAKE(x) x
#define SR_DEFSIGOBJ(type) \
	typedef SR_SUNRISE_NAMESPACE SingletonHolder<type SR_SH_OPTION> \
		type##SingleObj
		//SR_FAKE(type)##SingleObj
#define SR_SIGOBJ(type) type##SingleObj::Instance()
//#define SR_SIGOBJ(type) SR_FAKE(type)##SingleObj::Instance()


#ifndef __ORACLE              /* USING INFORMIX */
	#include "TInformixEx.h"
	#include "TDBCrypt.h"
	#include "TQueryEx.h"
	//#define SR_DB_CRYPTER   TDBCrypt
	typedef TDBCrypt        SR_DB_CRYPTER;
	typedef TInformixEx     SR_DB_CONNECTOR;
	typedef TInformixEx		TDB_CONNECTION;		/*兼容以前的宏定义*/
	typedef TQueryEx        SR_DB_QUERY;
#else                         /* USING ORACLE */
	#include "ORAutil.h"
	#include "TOraCon.h"
	#include "TOraCrypt.h"
	#include "TQueryOra.h"
	//#define SR_DB_CRYPTER   TOraCrypt
	typedef TOraCrypt       SR_DB_CRYPTER;
	typedef TOraCon         SR_DB_CONNECTOR;
	typedef TOraCon			TDB_CONNECTION;		/*兼容以前的宏定义*/
	typedef TQueryOra       SR_DB_QUERY;
#endif


#define SR_CONNECT2DB(ConStr, pDBC) \
{ \
	pDBC = NULL; \
	pDBC = TDBConn::Con2DB(ConStr); \
	if (NULL != pDBC) \
	{ \
		DL((APP, "Connected to database OK using '%s'.\n", ConStr)); \
	} \
	else \
	{ \
		DL((ERROR, "Failed to connect to database using '%s'.\n", ConStr)); \
		return -1; \
	} \
}



class TDBConn
{
	private:

		TDBConn();
		~TDBConn();


	public:

		static SR_DB_CONNECTOR * Con2DB(const char *_ConStr)
		{
#ifdef __ORACLE
			try
			{
#endif
				SR_DEFSIGOBJ(SR_DB_CRYPTER);
				if (!SR_SIGOBJ(SR_DB_CRYPTER).init())
				{
					return NULL;
				}

				return (SR_SIGOBJ(SR_DB_CRYPTER).useDBConnection(_ConStr));
#ifdef __ORACLE
			}
			catch (OraError &e)
			{
				DL((ERROR, "Caught OraError exception, msg is following.\n"));
				DL((ERROR, "SQLCODE is %ld, ErrMsg is:\n%s", e.get_error(), e.get_info()));
			}
			catch (...)
			{
				DL((ERROR, "Caught unknown exception.\n"));
			}

			return NULL;
#endif
		}
};


class TDBConnection
{
	public:
		TDBConnection(){}
		~TDBConnection(){}

		SR_DB_CONNECTOR * connDB(const char *_ConStr)
		{
			return TDBConn::Con2DB(_ConStr);
		}
};



#endif  /* __TDBCONNECTION_H__ */
