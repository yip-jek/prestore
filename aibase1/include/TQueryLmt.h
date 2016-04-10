/*--------------------------
李浚挺 
2005-3-24

提供Oracle环境下获取Select结果的接口，
实现BOSS TQueryEx类功能的一个子集。
---------------------------*/
#ifndef _TQueryLmt_H_
#define _TQueryLmt_H_

#include "devisdefs.h"
#include "TOraCon.h"

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

#define SQL_MAX_LEN 4096

class TQueryLmt
{
public:
    TQueryLmt(TOraCon *pOraCon);          /*调用该构造函数，会自动确定SQLDA的一些初始大小为默认值40，
                                               如果要手工定义，请调用不带参数的构造函数，然后调用Set和Init*/
    TQueryLmt();
    ~TQueryLmt();
	
	BOOL Init(TOraCon *pOraCon);         /*传入连接指针，当使用第二个构造函数的时候用到*/
	void Set(int item,int vname,int iname);  /*手工设置SQLDA的一些初始大小*/
	
	BOOL CheckDBActive();                    /*是否已经连接数据库 */
	int  GetLastError(char *errstr,int errlen);
	BOOL SQLExecute(char* sSQL);
	BOOL SQLPrepare(const char* sSql);            /*Prepare一条SQL*/
	BOOL SQLOpenCursor();
	BOOL SQLClose(); 
	int SQLFetch();    
	
	
    
    /* 以下函数用于通过字段位置绑定输出变量 */
    BOOL BindFieldByPos(long nVarNo,char *&sVarBuff);
    BOOL BindFieldByPos(long nVarNo,int *&nVarBuff);
    BOOL BindFieldByPos(long nVarNo,long *&lVarBuff);
    BOOL BindFieldByPos(long nVarNo,float *&fVarBuff);
    BOOL BindFieldByPos(long nVarNo,double *&dVarBuff); 
    
   	int Alloc();
	int Select_list();
	int Free(); 
protected:
    
    TOraCon *m_pOraCon;

	/*SQLDA *m_pbind;*/
	SQLDA *m_pselect;    
	
	char m_sSql[SQL_MAX_LEN+1];	
	char m_sErrstr[4096];
	int  m_nSqlCode;
	
    typedef struct  /*此结构用于绑定输出变量的实际存放*/
    {
        short siType;   /*类型*/
        char* vpBuff;  /*实际内存区域*/
        int nLen;      /*最大长度，如果超过则报警 */
    }_VAR;
    _VAR *m_pField;          

	int m_nItem,m_nVname,m_nIname;
	int m_nFlag;       /* 数据库指针定义标志*/

};

#endif
