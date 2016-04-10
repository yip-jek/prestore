/*--------------------------
�ͦ 
2005-3-24

�ṩOracle�����»�ȡSelect����Ľӿڣ�
ʵ��BOSS TQueryEx�๦�ܵ�һ���Ӽ���
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
    TQueryLmt(TOraCon *pOraCon);          /*���øù��캯�������Զ�ȷ��SQLDA��һЩ��ʼ��СΪĬ��ֵ40��
                                               ���Ҫ�ֹ����壬����ò��������Ĺ��캯����Ȼ�����Set��Init*/
    TQueryLmt();
    ~TQueryLmt();
	
	BOOL Init(TOraCon *pOraCon);         /*��������ָ�룬��ʹ�õڶ������캯����ʱ���õ�*/
	void Set(int item,int vname,int iname);  /*�ֹ�����SQLDA��һЩ��ʼ��С*/
	
	BOOL CheckDBActive();                    /*�Ƿ��Ѿ��������ݿ� */
	int  GetLastError(char *errstr,int errlen);
	BOOL SQLExecute(char* sSQL);
	BOOL SQLPrepare(const char* sSql);            /*Prepareһ��SQL*/
	BOOL SQLOpenCursor();
	BOOL SQLClose(); 
	int SQLFetch();    
	
	
    
    /* ���º�������ͨ���ֶ�λ�ð�������� */
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
	
    typedef struct  /*�˽ṹ���ڰ����������ʵ�ʴ��*/
    {
        short siType;   /*����*/
        char* vpBuff;  /*ʵ���ڴ�����*/
        int nLen;      /*��󳤶ȣ���������򱨾� */
    }_VAR;
    _VAR *m_pField;          

	int m_nItem,m_nVname,m_nIname;
	int m_nFlag;       /* ���ݿ�ָ�붨���־*/

};

#endif
