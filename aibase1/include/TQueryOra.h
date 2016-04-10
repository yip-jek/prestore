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

/*Ĭ�������������󳤶�*/
#define MAX_PARAMSIZE 100 

/*Ĭ�����������Ϊ��ֵ*/
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

/** \defgroup libdb   ���ݿ��̿�(libdb) */
/** \{*/
/** \file TQueryOra.h
 *  \brief ��װoracle������ͷ�ļ�*/

/** \class TQueryOra
 *  \brief ��װoracle������
 */
/** \}*/

/** \example exDB.cpp 
 *  \example exInf.ecpp
 *  \brief TDBInstance TQueryEx TQueryOra TInformixEx TOraCon ��ʾ������ 
 */


class TQueryOra
{
public:
        /**\enum emSQLStatus { FLAG_CLOSE,FLAG_DEFINED,FLAG_OPENED }
                 *  \brief ���ڱ�ǵ�ǰSQL����״̬�������� */
        enum emSQLStatus { FLAG_CLOSE,FLAG_DEFINED,FLAG_OPENED };  /* sql step status  */
public:
	TQueryOra(const TOraCon *pOraCon);          /*���øù��캯�������Զ�ȷ��SQLDA��һЩ��ʼ��СΪĬ��ֵ40��
                                               ���Ҫ�ֹ����壬����ò��������Ĺ��캯����Ȼ�����Set��Init*/
	~TQueryOra();
	
	/** \brief ��ʼ�����ݳ�Ա
	 */
	void Init();         

	/** \brief �ֹ�����SQLDA�ĳ�ʼֵ
	 *  \param item ��������
	 *  \param vname �������Ƴ���
	 *  \param iname ָʾ�������Ƴ���
	 */
	void SetDespSize(int item,int vname,int iname);
	
	/** brief ����sqlִ������
	 */
	long GetRow() { return m_nRowsAffected; }
	
	/** \brief �Ƿ��Ѿ��������ݿ�
	 *
	 */
	BOOL CheckDBActive();  


	/** \brief ���ö�̬һִ����δ֪��������ķ�select��䡡
	 *
	 */
	BOOL SQLExecute(char* sSQL);


	/** \brief����SQL
	 */
	BOOL SQLPrepare(const char* sSql);            /*Prepareһ��SQL*/

	/** \brief ���α꣬���ڷ�select�൱��ִ�����
	 */
	BOOL SQLOpenCursor();


	/** \brief ��ȡ�α����ݣ����select���
	 *  \return ����Ϊ-1;��������sqlcode,Ҳ������ȡ�α����ʱ����1403,����Ϊ0;
	 */
	int  SQLFetch();    

	/** \brief �ͷ�SQLDA�ռ�,�ر��α�
	 */
	int  SQLClose(); 

	/** \brief �ر��α�
	 */
	//BOOL CloseCorser(); 
	BOOL SQLCloseCursor(); 
	
protected:
	/** \brief ��ʼ��SQLDA�ṹ�ı����������������Ƴ��ȣ�ָʾ�������Ƴ���
	 *
	 */
   	int Alloc_Descriptors();

	/** \brief�������sqlda������
	 */
	int SetSelect_Desp();

	/** \brief��������sqlda������
	 */
	int SetBind_Desp();
	
    
    
public:
        /** \brief ͨ���ֶ�λ�ð��������,����һ�κ�Ӱ󶨱�����ֵ�õ�����ֵ��
         *  \param nVarNo Ϊ����ֶε�λ��,��0��ʼ; 
         *  \param ??VarBuf Ҫ�󶨱������õ�ָ��; 
         */
        BOOL BindFieldByPos(long nVarNo, char *& strVarBuff);
        BOOL BindFieldByPos(long nVarNo, int*& iVarBuff);
        BOOL BindFieldByPos(long nVarNo, long*& nVarBuff);
        BOOL BindFieldByPos(long nVarNo, float*& fVarBuff);
        BOOL BindFieldByPos(long nVarNo, double*& dfVarBuff);


        /** \brief ͨ���ֶ�λ���趨�������,��ÿһ�������Ҫ����һ��
         *  \param nVarNo Ϊ�����ֶε�λ��,��0��ʼ; 
         *  \param ??VarBuf Ҫ�󶨱�������; 
         */
        BOOL GetFieldByPos(long nVarNo, char* pchBuff);
        BOOL GetFieldByPos(long nVarNo, int& iVarBuff);
        BOOL GetFieldByPos(long nVarNo, char& chrVarBuff);
        BOOL GetFieldByPos(long nVarNo, long& nVarBuff);  
        BOOL GetFieldByPos(long nVarNo, float& fVarBuff);  
        BOOL GetFieldByPos(long nVarNo, double& dfVarBuff); 
public:
        /** \brief ͨ���ֶ�λ�ð��������,����һ�κ�ͨ���ı�󶨱�����ֵ��ɸı����������
         *  \param nVarNo Ϊ�����ֶε�λ��,��0��ʼ; 
         *  \param ??VarBuf Ҫ�󶨱������õ�ָ��; 
         *  \param MaxSize Ϊ�����ַ����͵ĳ���,Ӧȡ���ݱ���ĳ���; 
         *  \param Ifnull ��ֵ���£�1Ϊ�գ�Ĭ��Ϊ�ǿ�0; 
         */
        BOOL BindParamByPos(long nVarNo, char*& pchBuf,int MaxSize=MAX_PARAMSIZE,int Ifnull=DF_vNULL); 
        BOOL BindParamByPos(long nVarNo, int*& iVarBuf, int Ifnull=DF_vNULL);
        BOOL BindParamByPos(long nVarNo, long*& nVarBuf, int Ifnull=DF_vNULL);
        BOOL BindParamByPos(long nVarNo, float*& fVarBuf, int Ifnull=DF_vNULL);
        BOOL BindParamByPos(long nVarNo, double*& dfVarBuf, int Ifnull=DF_vNULL);


        /** \brief ͨ���ֶ�λ���趨�������,ÿ���������Ҫ����һ�Ρ�
         *  \param nVarNo Ϊ�����ֶε�λ��,��0��ʼ; 
         *  \param ??VarBuf Ҫ�趨����������; 
         *  \param MaxSize Ϊ�����ַ����͵ĳ���,Ӧȡstrlen(pchBuf); 
         *  \param Ifnull ��ֵ���£�1Ϊ�գ�Ĭ��Ϊ�ǿ�0; 
         */
        BOOL SetParamByPos(long nVarNo, char *pchBuf, int MaxSize=MAX_PARAMSIZE, int Ifnull=DF_vNULL);
        BOOL SetParamByPos(long nVarNo, int& iVarBuf, int Ifnull=DF_vNULL);
        BOOL SetParamByPos(long nVarNo, char& chrVarBuf, int Ifnull=DF_vNULL);
        BOOL SetParamByPos(long nVarNo, long& nVarBuf, int Ifnull=DF_vNULL);
        BOOL SetParamByPos(long nVarNo, float& fVarBuf, int Ifnull=DF_vNULL);
        BOOL SetParamByPos(long nVarNo, double& dfVarBuf, int Ifnull=DF_vNULL);
protected:
    
		const 	TOraCon *m_pOraCon;

        /** \brief ���SQLDA 
         */
	SQLDA *m_pbind;

        /** \brief ����SQLDA 
         */
	SQLDA *m_pselect;    
	
        /** \brief ����sql����ı� 
         */
	char m_sSql[SQL_MAX_LEN+1];	

        /** \brief ��ʶsql����Ƿ�Ϊ��ѯ��䡡 
         */
	int  m_ifselect;
	
        /** \brief ԭʼSQL�������������ĸ���
         */
        long          m_nInColumns;

        /** \brief ԭʼSQL���������ֶεĸ���
         */
        long          m_nOutColumns;

        /** \brief SQL���ִ������
         */
        long          m_nRowsAffected;

        /** \brief ���ڳ�ʼ��SQLDA�ṹ�ı����������������Ƴ��ȣ�ָʾ�������Ƴ��� 
         */
	int m_nItem,m_nVname,m_nIname;

        /** \brief ���ݿ�ָ�붨���־
         */
	enum emSQLStatus m_nFlag;

};

#endif
