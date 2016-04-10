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

/** \defgroup libdb   ���ݿ��̿�(libdb) */
/** \{*/
/** \file TQueryEx.h
 *  \brief ��װInformix������ͷ�ļ�*/

/** \class TQueryEx
 *  \brief ��װInformix������
 */
/** \}*/

/** \example exDB.cpp 
 *  \example exInf.ecpp
 *  \brief TDBInstance TQueryEx TQueryOra TInformixEx TOraCon ��ʾ������ 
 */

class TQueryEx
{
public:
	/** \brief ȱʡ���캯��
	 */
	TQueryEx();

	/** \brief ���캯��
	 */
	TQueryEx(const TInformixEx *pDatabase);

	/** \brief ��������
	 */
	virtual ~TQueryEx();

protected:
	const TInformixEx  *m_pDatabase;       /* informix database pointer    */

        /** \brief ��¼��Ϊ��������ĸ����ֶε���Ϣ
         */
	struct sqlda *m_sqlda_ptr_in;   /* sqlda struct pointer to Parameter */

        /** \brief ��¼SELECT����и������ֶε���Ϣ
         */
	struct sqlda *m_sqlda_ptr_out;  /* sqlda struct pointer to Field  */

        /** \brief ����ȡ��m_sqlda_ptr_in, m_sqlda_ptr_out�и����ֶε���Ϣ
         */
    	struct sqlvar_struct *col_ptr;  /* sqlvar struct point column   */
	//struct sqlvar_struct *m_sqlvar_ptr;  /* sqlvar struct pointer */

protected:
        /** \brief��¼Ҫ���д����SQL���
         */
	char          m_sql_statment[4096];     /* informix SQL statement       */

        /** \brief ��¼����ԭʼSQL�����α������
         */
	char          m_sql_cursor_name[64];    /* SQL cursor name              */

        /** \brief ��¼����ԭʼSQL�����α��ID
         */
	char          m_sql_cursor_var[64];     /* SQL cursor id we defined     */

        /** \brief ��־�Ƿ��SQL������Ԥ����
         */
	int	      m_PrepareFlag;

        /** \brief ��־�Ƿ������α�
         */
	int	      m_CursorFlag;

        /** \brief ��־�Ƿ�����α�
         */
	int	      m_CursorOpenFlag;

        /** \brief ��¼ԭʼSQL���ת����õ��Ĺ������������SELECT���
         */

	char	      m_sql_statment_in[2048];

        /** \brief ��¼�������������SELECT�����α��ID
         */
	char	      m_sql_cursor_var_in[64];

        /** \brief ��־�Ƿ�����������SELECT��������Ԥ����
         */
	int	      m_PrepareFlagIn;

	//---------------JUST FOR Select & Where & Var-----------------//
        /** \brief ��¼ԭʼSQL���ת����õ��ķ����ֶε�SELECT���
         */
	char          m_sql_statment_out[2048];  /* informix SQL statement       */

        /** \brief ��¼���������ֶε�SELECT�����α��ID
         */
	char          m_sql_cursor_var_out[64];  /* SQL cursor id we defined     */

        /** \brief ��־�Ƿ�Է����ֶε�SELECT��������Ԥ����
         */
	int	      m_PrepareFlagOut;

        /** \brief ��¼��ǰ��������,ÿ����һ������ͶԸ���Ž��м�1
         */
	long		m_lQueryExIndex;
	static long   m_lQueryExBase; 

        /** \brief ���ݻ��棬���ں͸������ֶν��а�
         */
	char         *m_sql_data_ptr_in;         /* SQL data ptr                 */

        /** \brief ���ݻ���Ĵ�С
         */
	long          m_sql_buf_size_in;         /* sqlvar struct buffer size    */

        /** \brief ���ݻ��棬���ں͸�����ֶν��а�
         */
	char         *m_sql_data_ptr_out;        /* SQL data ptr                 */

        /** \brief ���ݻ���Ĵ�С
         */
	long          m_sql_buf_size_out;        /* sqlvar struct buffer size    */

        /** \brief ��¼ԭʼSQL��������
         */
	long          m_nSQLTypes;               /* SQL statment Type detemine   */

        /** \brief ԭʼSQL�������������ĸ���
         */
	long          m_nInColumns;              /* informix database columns    */

        /** \brief ԭʼSQL���������ֶεĸ���
         */
	long          m_nOutColumns;             /* informix database columns    */

        /** \brief SQL�����Ӱ�������
         */
	long          m_nRowsAffected;           /* informix database rows       */

        /** \brief �������
         */
	long          m_nCode ;                  /* informix error code          */
        /** \brief ������Ϣ
         */
	char          m_sMsg[4096];              /* informix error message       */

	/* transaction DDL operation */
public:
	/** \brief ��ʼһ��Transaction 
	 */
	BOOL BeginTrans();
	/** \brief �ع�һ��Transaction
	 */
	BOOL RollbackTrans();
	/** \brief �ύһ������
	 */
	BOOL CommitTrans();
	/** \brief �ж�һ������
	 */
	BOOL BreakTrans();

	/* direct DDL operation */
public:
	/** \brief ������ݿ��Ƿ��Ծ
	 */
	BOOL CheckDBActive();
	/** \brief ֱ������һ����̬SQL���
	 */
	BOOL StaticSQL(char *sSQL);
	/** \brief ���ݺʹ�ӡ��SQL����ִ�����, ����debug
	 */
	void SQLTrace();

	/* DDL operation by UI Interface */
public:
	/** \brief �������ȴ�ʱ��Ϊfor ever
	 */
	void SetLockModeWait(void);
	/** \brief ִ�е�ǰ��SQL���
	 */
	BOOL SQLExecute(); 
	/** \brief Ԥ����һ��SQL���
	 */
	BOOL SQLPrepare(char *sSQL);
	/** \brief Ԥ����һ��SQL���
	 */
	BOOL SQLForcePrepare(char *sSQL);
	/** \brief Ԥ����һ��SQL���
	 */
	BOOL SQLPrepare(const TString& sSQL);
	/** \brief Ϊһ��Ԥ������˵�select SQL�����α�
	 */
	BOOL SQLOpenCursor(  );
	/** \brief �ر��α�
	 */
	BOOL SQLCloseCursor();

	/** \brief ��ȡ��¼
	 */
	long SQLFetch();
	/** \brief �ͷ��α꣬�ͷ�SQL��������
	 */
	BOOL SQLClose();        
	/** \brief ��ӡ�ֶ�����
	 */
	void DescribeColumns(); 
	/** \brief ��ӡ�ֶ���������
	 */
	void Describe();
	/** \brief ��ȡupdate, insert���Ӱ��ļ�¼��
	 */
	long GetRowsAffected();
	/** \breif ��ȡ���������
	 */
	long GetInColumns();
	/** \brief ��ȡ���������
	 */
	long GetOutColumns();

private:
	/** \brief ���������
	 */
	BOOL SqlAllocBindAera( long & m_sql_buf_size,char * m_sql_data_ptr,
                                struct sqlda * m_sqlda_ptr );
        /** \brief ���������ݴ�ŵ����ݿ�Ļ�����
         */
	BOOL SQLPut();                      /* put the data to the buffer   */

        /** \brief �����ݿ⻺�������������뵽���ݿ���
         */
	BOOL SQLFlush();                    /* put the data to the buffer   */

        /** \brief ��ȡ���ݿ����Ĵ�����Ϣ
          * \param nIndex:
	  * 0 - estimated number of rows returned 
	  * 1 - serial value after insert or  ISAM error code 
	  * 2 - number of rows processed 
  	  * 3 - estimated cost 
	  * 4 - offset of the error into the SQL statement 
          * 5 - rowid after insert 
         */
	long GetSqlerrd(int nIndex);

        /** \brief ��һ��SQL������ת���������
          * \param pcSQL ָ��ԭʼSQL���
          * \param pcResultSQL ָ������ֶζ�Ӧ��SELECT���
          * \param pcWhereSQL ָ�������ֶζ�Ӧ��SELECT���
          */
	BOOL ConvertSQL(char* pcSQL,char* pcResultSQL,char* pcWhereSQL);
	
	/* input params bind by UI Interface */
public:
	/** \brief ͨ���ֶ�λ���趨�������ֵ
	 */
	BOOL SetParamByPos(long nVarNo, TString& sBuf);
	/** \brief ͨ���ֶ�λ���趨�������ֵ
	 */
	BOOL SetParamByPos(long nVarNo, char *pchBuf);
	/** \brief ͨ���ֶ�λ���趨�������ֵ
	 */
	BOOL SetParamByPos(long nVarNo, int& iVarBuf);
	/** \brief ͨ���ֶ�λ���趨�������ֵ
	 */
	BOOL SetParamByPos(long nVarNo, char& chrVarBuf);
	/** \brief ͨ���ֶ�λ���趨�������ֵ
	 */
	BOOL SetParamByPos(long nVarNo, long& nVarBuf);
	/** \brief ͨ���ֶ�λ���趨�������ֵ
	 */
	BOOL SetParamByPos(long nVarNo, float& fVarBuf);
	/** \brief ͨ���ֶ�λ���趨�������ֵ
	 */
	BOOL SetParamByPos(long nVarNo, double& dfVarBuf);

public:
	//BOOL BindParamByPos(long nVarNo, TString& sBuf);
	/** \brief ͨ���ֶ�λ�ð��������, �Ժ����ֵ�ĸı�ͻ�ʹInformix�����
	 *  �������ֵ�ı䣬�뿴������
	 */
	BOOL BindParamByPos(long nVarNo, char*& pchBuf); 
	/** \brief ͨ���ֶ�λ�ð��������
	 */
	BOOL BindParamByPos(long nVarNo, int*& iVarBuf);
	/** \brief ͨ���ֶ�λ�ð��������
	 */
	BOOL BindParamByPos(long nVarNo, long*& nVarBuf);
	/** \brief ͨ���ֶ�λ�ð��������
	 */
	BOOL BindParamByPos(long nVarNo, float*& fVarBuf);
	/** \brief ͨ���ֶ�λ�ð��������
	 */
	BOOL BindParamByPos(long nVarNo, double*& dfVarBuf);

	/* DDL operation support */
protected:												 
        /** \brief ��ʼ����Ա����
         */
	void SQLInit();            /* SQL attribute initialize     */

        /** \brief ����SQL�����ʱû��ʵ��
         */
	BOOL SQLParse();           /* SQL attribute parse          */
        /** \brief �ͷ����ݿ���Դ��ʱû��ʵ��
         */
	void FreeResource();       /* free resource information    */

public:
	/** \brief ͨ���ֶ�λ�ð��������, �Ժ�fetch���������ݶ����ڱ�����
	 * �뿴����
	 */
	BOOL BindFieldByPos(long nVarNo, char *& strVarBuff);
	/** \brief ͨ���ֶ�λ�ð��������
	 */
	BOOL BindFieldByPos(long nVarNo, int*& iVarBuff);
	/** \brief ͨ���ֶ�λ�ð��������
	 */
	BOOL BindFieldByPos(long nVarNo, long*& nVarBuff);
	/** \brief ͨ���ֶ�λ�ð��������
	 */
	BOOL BindFieldByPos(long nVarNo, float*& fVarBuff);
	/** \brief ͨ���ֶ�λ�ð��������
	 */
	BOOL BindFieldByPos(long nVarNo, double*& dfVarBuff);
	
	/* output params bind by UI Interface */
public:
	/** \brief ͨ���ֶ�λ�û�ȡ���ֵ
	 */
	BOOL GetFieldByPos(long nVarNo, char* pchBuff); /* read field char * params */
	/** \brief ͨ���ֶ�λ�û�ȡ���ֵ
	 */
	BOOL GetFieldByPos(long nVarNo, int& iVarBuff); /* read field int params    */
	/** \brief ͨ���ֶ�λ�û�ȡ���ֵ
	 */
	BOOL GetFieldByPos(long nVarNo, char& chrVarBuff); /* read field int params */
	/** \brief ͨ���ֶ�λ�û�ȡ���ֵ
	 */
	BOOL GetFieldByPos(long nVarNo, long& nVarBuff);  /* read field long params */
	/** \brief ͨ���ֶ�λ�û�ȡ���ֵ
	 */
	BOOL GetFieldByPos(long nVarNo, float& fVarBuff);  /* read field float params */
	/** \brief ͨ���ֶ�λ�û�ȡ���ֵ
	 */
	BOOL GetFieldByPos(long nVarNo, double& dfVarBuff); /* read field double params*/
};

#ifndef __linux__
long TQueryEx::m_lQueryExBase = 0;
#endif

#endif
