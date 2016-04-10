#ifndef _TQueryOra_H_
#define _TQueryOra_H_

#include "devisdefs.h"
#include "TOraCon.h"
#include <string.h>
using namespace std;

#define SQL_MAX_LEN 3072

#define DEFAULT_ITEM 50
#define DEFAULT_VNAME 40
#define DEFAULT_INAME 40

/*默认输入变量的最大长度*/
#define MAX_PARAMSIZE 100 

/*默认输入变量不为空值*/
#define DF_vNULL 0 

#ifndef SQLDA_
#define SQLDA_ 1

#ifdef T
# undef T
#endif
#ifdef F
# undef F
#endif

#ifdef S
# undef S
#endif
#ifdef L
# undef L
#endif

struct SQLDA {
  /* ub4    */ int        N; /* Descriptor size in number of entries        */
  /* text** */ char     **V; /* Ptr to Arr of addresses of main variables   */
  /* ub4*   */ int       *L; /* Ptr to Arr of lengths of buffers            */
  /* sb2*   */ short     *T; /* Ptr to Arr of types of buffers              */
  /* sb2**  */ short    **I; /* Ptr to Arr of addresses of indicator vars   */
  /* sb4    */ int        F; /* Number of variables found by DESCRIBE       */
  /* text** */ char     **S; /* Ptr to Arr of variable name pointers        */
  /* ub2*   */ short     *M; /* Ptr to Arr of max lengths of var. names     */
  /* ub2*   */ short     *C; /* Ptr to Arr of current lengths of var. names */
  /* text** */ char     **X; /* Ptr to Arr of ind. var. name pointers       */
  /* ub2*   */ short     *Y; /* Ptr to Arr of max lengths of ind. var. names */
  /* ub2*   */ short     *Z; /* Ptr to Arr of cur lengths of ind. var. names */
  };

typedef struct SQLDA SQLDA;

#endif 

/** \defgroup libdb   数据库编程库(libdb) */
/** \{*/
/** \file TQueryOra.h
 *  \brief 封装oracle操作类头文件*/

/** \class TQueryOra
 *  \brief 封装oracle操作类
 */
/** \}*/

/** \example exDB.cpp 
 *  \example exInf.ecpp
 *  \brief TDBInstance TQueryEx TQueryOra TInformixEx TOraCon 类示例程序 
 */


class TQueryOra
{
public:
        /**\enum emSQLStatus { FLAG_CLOSE,FLAG_DEFINED,FLAG_OPENED }
                 *  \brief 用于标记当前SQL操作状态的联合体 */
        enum emSQLStatus { FLAG_CLOSE,FLAG_DEFINED,FLAG_OPENED };  /* sql step status  */
public:
	TQueryOra(const TOraCon *pOraCon);          /*调用该构造函数，会自动确定SQLDA的一些初始大小为默认值40，
                                               如果要手工定义，请调用不带参数的构造函数，然后调用Set和Init*/
	~TQueryOra();
	
	/** \brief 初始化数据成员
	 */
	void Init();         

	/** \brief 手工设置SQLDA的初始值
	 *  \param item 变量个数
	 *  \param vname 变量名称长度
	 *  \param iname 指示变量名称长度
	 */
	void SetDespSize(int item,int vname,int iname);
	
	/** brief 返回sql执行行数
	 */
	long GetRow() { return m_nRowsAffected; }
	
	/** \brief 是否已经连接数据库
	 *
	 */
	BOOL CheckDBActive();  


	/** \brief 利用动态一执行无未知输入变量的非select语句　
	 *
	 */
	BOOL SQLExecute(char* sSQL);


	/** \brief分析SQL
	 */
	BOOL SQLPrepare(const char* sSql);            /*Prepare一条SQL*/

	/** \brief 打开游标，对于非select相当于执行语句
	 */
	BOOL SQLOpenCursor();


	/** \brief 获取游标数据，针对select语句
	 *  \return 出错为-1;其它返回sqlcode,也就量当取游标结束时返回1403,否则为0;
	 */
	int  SQLFetch();    

	/** \brief 释放SQLDA空间,关闭游标
	 */
	int  SQLClose(); 

	/** \brief 关闭游标
	 */
	//BOOL CloseCorser(); 
	BOOL SQLCloseCursor(); 
	
protected:
	/** \brief 初始化SQLDA结构的变量个数，变量名称长度，指示变量名称长度
	 *
	 */
   	int Alloc_Descriptors();

	/** \brief处理输出sqlda缓冲区
	 */
	int SetSelect_Desp();

	/** \brief处理输入sqlda缓冲区
	 */
	int SetBind_Desp();
	
    
    
public:
        /** \brief 通过字段位置绑定输出变量,调用一次后从绑定变量的值得到输入值　
         *  \param nVarNo 为输出字段的位置,从0开始; 
         *  \param ??VarBuf 要绑定变量引用的指针; 
         */
        BOOL BindFieldByPos(long nVarNo, char *& strVarBuff);
        BOOL BindFieldByPos(long nVarNo, int*& iVarBuff);
        BOOL BindFieldByPos(long nVarNo, long*& nVarBuff);
        BOOL BindFieldByPos(long nVarNo, float*& fVarBuff);
        BOOL BindFieldByPos(long nVarNo, double*& dfVarBuff);


        /** \brief 通过字段位置设定输入变量,对每一行输出都要调用一次
         *  \param nVarNo 为输入字段的位置,从0开始; 
         *  \param ??VarBuf 要绑定变量引用; 
         */
        BOOL GetFieldByPos(long nVarNo, char* pchBuff);
        BOOL GetFieldByPos(long nVarNo, int& iVarBuff);
        BOOL GetFieldByPos(long nVarNo, char& chrVarBuff);
        BOOL GetFieldByPos(long nVarNo, long& nVarBuff);  
        BOOL GetFieldByPos(long nVarNo, float& fVarBuff);  
        BOOL GetFieldByPos(long nVarNo, double& dfVarBuff); 
public:
        /** \brief 通过字段位置绑定输入变量,调用一次后通过改变绑定变量的值便可改变输入变量　
         *  \param nVarNo 为输入字段的位置,从0开始; 
         *  \param ??VarBuf 要绑定变量引用的指针; 
         *  \param MaxSize 为对于字符类型的长度,应取数据表定义的长度; 
         *  \param Ifnull 空值标致，1为空，默认为非空0; 
         */
        BOOL BindParamByPos(long nVarNo, char*& pchBuf,int MaxSize=MAX_PARAMSIZE,int Ifnull=DF_vNULL); 
        BOOL BindParamByPos(long nVarNo, int*& iVarBuf, int Ifnull=DF_vNULL);
        BOOL BindParamByPos(long nVarNo, long*& nVarBuf, int Ifnull=DF_vNULL);
        BOOL BindParamByPos(long nVarNo, float*& fVarBuf, int Ifnull=DF_vNULL);
        BOOL BindParamByPos(long nVarNo, double*& dfVarBuf, int Ifnull=DF_vNULL);


        /** \brief 通过字段位置设定输入变量,每次输入变量要调用一次　
         *  \param nVarNo 为输入字段的位置,从0开始; 
         *  \param ??VarBuf 要设定变量的引用; 
         *  \param MaxSize 为对于字符类型的长度,应取strlen(pchBuf); 
         *  \param Ifnull 空值标致，1为空，默认为非空0; 
         */
        BOOL SetParamByPos(long nVarNo, char *pchBuf, int MaxSize=MAX_PARAMSIZE, int Ifnull=DF_vNULL);
        BOOL SetParamByPos(long nVarNo, int& iVarBuf, int Ifnull=DF_vNULL);
        BOOL SetParamByPos(long nVarNo, char& chrVarBuf, int Ifnull=DF_vNULL);
        BOOL SetParamByPos(long nVarNo, long& nVarBuf, int Ifnull=DF_vNULL);
        BOOL SetParamByPos(long nVarNo, float& fVarBuf, int Ifnull=DF_vNULL);
        BOOL SetParamByPos(long nVarNo, double& dfVarBuf, int Ifnull=DF_vNULL);
protected:
    
		const 	TOraCon *m_pOraCon;

        /** \brief 输出SQLDA 
         */
	SQLDA *m_pbind;

        /** \brief 输入SQLDA 
         */
	SQLDA *m_pselect;    
	
        /** \brief 保存sql语句文本 
         */
	char m_sSql[SQL_MAX_LEN+1];	

        /** \brief 标识sql语句是否为查询语句　 
         */
	int  m_ifselect;
	
        /** \brief 原始SQL语句中输入参数的个数
         */
        long          m_nInColumns;

        /** \brief 原始SQL语句中输出字段的个数
         */
        long          m_nOutColumns;

        /** \brief SQL语句执行行数
         */
        long          m_nRowsAffected;

        /** \brief 用于初始化SQLDA结构的变量个数，变量名称长度，指示变量名称长度 
         */
	int m_nItem,m_nVname,m_nIname;

        /** \brief 数据库指针定义标志
         */
	enum emSQLStatus m_nFlag;

};

#endif
