/*
 * @All rights reserved
 *
 *   Informix ESQL Emmable SQL C/C++ Interface , DDL Operation
 * 
 * @Author  : devis
 * @Date    : 2002.11.26
 * @Version : 01.01.001
 * @History : 
 */


#ifndef __ESQL_TQUERY_EX_H__
#define __ESQL_TQUERY_EX_H__

#include "devisdefs.h"
#include "TInformixEx.h"
#include "TString.h"

////////////////////////////////////////////////////////////////////////////////////////
// VARCHAR defined 
#define MAXVCLEN		(255)
#define VCMIN(size)		(((size) >> 8) & 0xFF)
#define VCMAX(size)		((size) & 0xFF)
#define VCLENGTH(len)	(VCMAX(len) + 1)
#define VCSIZ(max, min)	((((min) & 0xFF) << 8) | ((max) & 0xFF))
////////////////////////////////////////////////////////////////////////////////////////

/** \defgroup libdb   数据库编程库(libdb) */
/** \{*/
/** \file TQueryEx.h
 *  \brief 封装Informix操作类头文件*/

/** \class TQueryEx
 *  \brief 封装Informix操作类
 */
/** \}*/

/** \example exDB.cpp 
 *  \example exInf.ecpp
 *  \brief TDBInstance TQueryEx TQueryOra TInformixEx TOraCon 类示例程序 
 */

class TQueryEx
{
public:
	/** \brief 缺省构造函数
	 */
	TQueryEx();

	/** \brief 构造函数
	 */
	TQueryEx(const TInformixEx *pDatabase);

	/** \brief 析构函数
	 */
	virtual ~TQueryEx();

protected:
	const TInformixEx  *m_pDatabase;       /* informix database pointer    */

        /** \brief 记录作为输入参数的各个字段的信息
         */
	struct sqlda *m_sqlda_ptr_in;   /* sqlda struct pointer to Parameter */

        /** \brief 记录SELECT语句中各返回字段的信息
         */
	struct sqlda *m_sqlda_ptr_out;  /* sqlda struct pointer to Field  */

        /** \brief 用来取出m_sqlda_ptr_in, m_sqlda_ptr_out中各个字段的信息
         */
    	struct sqlvar_struct *col_ptr;  /* sqlvar struct point column   */
	//struct sqlvar_struct *m_sqlvar_ptr;  /* sqlvar struct pointer */

protected:
        /** \brief记录要进行处理的SQL语句
         */
	char          m_sql_statment[4096];     /* informix SQL statement       */

        /** \brief 记录分析原始SQL语句的游标的名称
         */
	char          m_sql_cursor_name[64];    /* SQL cursor name              */

        /** \brief 记录分析原始SQL语句的游标的ID
         */
	char          m_sql_cursor_var[64];     /* SQL cursor id we defined     */

        /** \brief 标志是否对SQL语句进行预处理
         */
	int	      m_PrepareFlag;

        /** \brief 标志是否定义了游标
         */
	int	      m_CursorFlag;

        /** \brief 标志是否打开了游标
         */
	int	      m_CursorOpenFlag;

        /** \brief 记录原始SQL语句转换后得到的关于输入参数的SELECT语句
         */

	char	      m_sql_statment_in[2048];

        /** \brief 记录分析输入参数的SELECT语句的游标的ID
         */
	char	      m_sql_cursor_var_in[64];

        /** \brief 标志是否对输入参数的SELECT语句进行了预处理
         */
	int	      m_PrepareFlagIn;

	//---------------JUST FOR Select & Where & Var-----------------//
        /** \brief 记录原始SQL语句转换后得到的返回字段的SELECT语句
         */
	char          m_sql_statment_out[2048];  /* informix SQL statement       */

        /** \brief 记录分析返回字段的SELECT语句的游标的ID
         */
	char          m_sql_cursor_var_out[64];  /* SQL cursor id we defined     */

        /** \brief 标志是否对返回字段的SELECT语句进行了预处理
         */
	int	      m_PrepareFlagOut;

        /** \brief 记录当前对象的序号,每创建一个对象就对该序号进行加1
         */
	long		m_lQueryExIndex;
	static long   m_lQueryExBase; 

        /** \brief 数据缓存，用于和各输入字段进行绑定
         */
	char         *m_sql_data_ptr_in;         /* SQL data ptr                 */

        /** \brief 数据缓存的大小
         */
	long          m_sql_buf_size_in;         /* sqlvar struct buffer size    */

        /** \brief 数据缓存，用于和各输出字段进行绑定
         */
	char         *m_sql_data_ptr_out;        /* SQL data ptr                 */

        /** \brief 数据缓存的大小
         */
	long          m_sql_buf_size_out;        /* sqlvar struct buffer size    */

        /** \brief 记录原始SQL语句的类型
         */
	long          m_nSQLTypes;               /* SQL statment Type detemine   */

        /** \brief 原始SQL语句中输入参数的个数
         */
	long          m_nInColumns;              /* informix database columns    */

        /** \brief 原始SQL语句中输出字段的个数
         */
	long          m_nOutColumns;             /* informix database columns    */

        /** \brief SQL语句所影响的行数
         */
	long          m_nRowsAffected;           /* informix database rows       */

        /** \brief 错误代码
         */
	long          m_nCode ;                  /* informix error code          */
        /** \brief 错误信息
         */
	char          m_sMsg[4096];              /* informix error message       */

	/* transaction DDL operation */
public:
	/** \brief 开始一个Transaction 
	 */
	BOOL BeginTrans();
	/** \brief 回滚一个Transaction
	 */
	BOOL RollbackTrans();
	/** \brief 提交一个事务
	 */
	BOOL CommitTrans();
	/** \brief 中断一个事务
	 */
	BOOL BreakTrans();

	/* direct DDL operation */
public:
	/** \brief 检测数据库是否活跃
	 */
	BOOL CheckDBActive();
	/** \brief 直接运行一个静态SQL语句
	 */
	BOOL StaticSQL(char *sSQL);
	/** \brief 回溯和打印出SQL语句的执行情况, 用于debug
	 */
	void SQLTrace();

	/* DDL operation by UI Interface */
public:
	/** \brief 设置锁等待时间为for ever
	 */
	void SetLockModeWait(void);
	/** \brief 执行当前的SQL语句
	 */
	BOOL SQLExecute(); 
	/** \brief 预编译一个SQL语句
	 */
	BOOL SQLPrepare(char *sSQL);
	/** \brief 预编译一个SQL语句
	 */
	BOOL SQLForcePrepare(char *sSQL);
	/** \brief 预编译一个SQL语句
	 */
	BOOL SQLPrepare(const TString& sSQL);
	/** \brief 为一个预编译好了的select SQL语句打开游标
	 */
	BOOL SQLOpenCursor(  );
	/** \brief 关闭游标
	 */
	BOOL SQLCloseCursor();

	/** \brief 获取记录
	 */
	long SQLFetch();
	/** \brief 释放游标，释放SQL变量区域
	 */
	BOOL SQLClose();        
	/** \brief 打印字段描述
	 */
	void DescribeColumns(); 
	/** \brief 打印字段数据类型
	 */
	void Describe();
	/** \brief 获取update, insert语句影响的记录数
	 */
	long GetRowsAffected();
	/** \breif 获取输入变量数
	 */
	long GetInColumns();
	/** \brief 获取输出变量数
	 */
	long GetOutColumns();

private:
	/** \brief 绑定输出区域
	 */
	BOOL SqlAllocBindAera( long & m_sql_buf_size,char * m_sql_data_ptr,
                                struct sqlda * m_sqlda_ptr );
        /** \brief 将输入数据存放到数据库的缓冲区
         */
	BOOL SQLPut();                      /* put the data to the buffer   */

        /** \brief 将数据库缓冲区的数据输入到数据库中
         */
	BOOL SQLFlush();                    /* put the data to the buffer   */

        /** \brief 获取数据库出错的错误信息
          * \param nIndex:
	  * 0 - estimated number of rows returned 
	  * 1 - serial value after insert or  ISAM error code 
	  * 2 - number of rows processed 
  	  * 3 - estimated cost 
	  * 4 - offset of the error into the SQL statement 
          * 5 - rowid after insert 
         */
	long GetSqlerrd(int nIndex);

        /** \brief 将一个SQL语句进行转换输入参数
          * \param pcSQL 指向原始SQL语句
          * \param pcResultSQL 指向输出字段对应的SELECT语句
          * \param pcWhereSQL 指向输入字段对应的SELECT语句
          */
	BOOL ConvertSQL(char* pcSQL,char* pcResultSQL,char* pcWhereSQL);
	
	/* input params bind by UI Interface */
public:
	/** \brief 通过字段位置设定输入参数值
	 */
	BOOL SetParamByPos(long nVarNo, TString& sBuf);
	/** \brief 通过字段位置设定输入参数值
	 */
	BOOL SetParamByPos(long nVarNo, char *pchBuf);
	/** \brief 通过字段位置设定输入参数值
	 */
	BOOL SetParamByPos(long nVarNo, int& iVarBuf);
	/** \brief 通过字段位置设定输入参数值
	 */
	BOOL SetParamByPos(long nVarNo, char& chrVarBuf);
	/** \brief 通过字段位置设定输入参数值
	 */
	BOOL SetParamByPos(long nVarNo, long& nVarBuf);
	/** \brief 通过字段位置设定输入参数值
	 */
	BOOL SetParamByPos(long nVarNo, float& fVarBuf);
	/** \brief 通过字段位置设定输入参数值
	 */
	BOOL SetParamByPos(long nVarNo, double& dfVarBuf);

public:
	//BOOL BindParamByPos(long nVarNo, TString& sBuf);
	/** \brief 通过字段位置绑定输入变量, 以后变量值的改变就会使Informix管理的
	 *  输入参数值改变，请看范例。
	 */
	BOOL BindParamByPos(long nVarNo, char*& pchBuf); 
	/** \brief 通过字段位置绑定输入变量
	 */
	BOOL BindParamByPos(long nVarNo, int*& iVarBuf);
	/** \brief 通过字段位置绑定输入变量
	 */
	BOOL BindParamByPos(long nVarNo, long*& nVarBuf);
	/** \brief 通过字段位置绑定输入变量
	 */
	BOOL BindParamByPos(long nVarNo, float*& fVarBuf);
	/** \brief 通过字段位置绑定输入变量
	 */
	BOOL BindParamByPos(long nVarNo, double*& dfVarBuf);

	/* DDL operation support */
protected:												 
        /** \brief 初始化成员变量
         */
	void SQLInit();            /* SQL attribute initialize     */

        /** \brief 解析SQL语句暂时没有实现
         */
	BOOL SQLParse();           /* SQL attribute parse          */
        /** \brief 释放数据库资源暂时没有实现
         */
	void FreeResource();       /* free resource information    */

public:
	/** \brief 通过字段位置绑定输出变量, 以后fetch出来的数据都会在变量里
	 * 请看范例
	 */
	BOOL BindFieldByPos(long nVarNo, char *& strVarBuff);
	/** \brief 通过字段位置绑定输出变量
	 */
	BOOL BindFieldByPos(long nVarNo, int*& iVarBuff);
	/** \brief 通过字段位置绑定输出变量
	 */
	BOOL BindFieldByPos(long nVarNo, long*& nVarBuff);
	/** \brief 通过字段位置绑定输出变量
	 */
	BOOL BindFieldByPos(long nVarNo, float*& fVarBuff);
	/** \brief 通过字段位置绑定输出变量
	 */
	BOOL BindFieldByPos(long nVarNo, double*& dfVarBuff);
	
	/* output params bind by UI Interface */
public:
	/** \brief 通过字段位置获取输出值
	 */
	BOOL GetFieldByPos(long nVarNo, char* pchBuff); /* read field char * params */
	/** \brief 通过字段位置获取输出值
	 */
	BOOL GetFieldByPos(long nVarNo, int& iVarBuff); /* read field int params    */
	/** \brief 通过字段位置获取输出值
	 */
	BOOL GetFieldByPos(long nVarNo, char& chrVarBuff); /* read field int params */
	/** \brief 通过字段位置获取输出值
	 */
	BOOL GetFieldByPos(long nVarNo, long& nVarBuff);  /* read field long params */
	/** \brief 通过字段位置获取输出值
	 */
	BOOL GetFieldByPos(long nVarNo, float& fVarBuff);  /* read field float params */
	/** \brief 通过字段位置获取输出值
	 */
	BOOL GetFieldByPos(long nVarNo, double& dfVarBuff); /* read field double params*/
};

#ifndef __linux__
long TQueryEx::m_lQueryExBase = 0;
#endif

#endif
