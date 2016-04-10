
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

/** \defgroup libdb   ���ݿ��̿�(libdb) */
/** \{*/
/** \file TInformixEx.h  
 *  \brief TInformixEx�������ͷ�ļ�  */

/** \class TInformixEx
 *  \brief TInformixEx������ */

/** \example exDB.cpp 
 *  \example exInf.ecpp
 *  \brief TDBInstance TQueryEx TQueryOra TInformixEx TOraCon ��ʾ������ 
 */


/** \}*/

////////////////////////////////////////////////////////////////////////////////////////
class TInformixEx
{
public:
	/** \brief ���캯��
	 */
	TInformixEx();
	/** \brief ��������
	 */
	virtual ~TInformixEx();

public:
	/**\enum emStatus { NotInited,Connected,Disconnected }
		 *  \brief ���ڱ�ǵ�ǰ���ݿ�״̬�������� */
	enum emStatus { NotInited,Connected,Disconnected };  /* connection mode status  */
	/**\ enum emTransMode { Auto, Manual };
		 *  \brief ���ڱ��������ķ�ʽ���û�����,�����Զ�*/
	enum emTransMode { Auto, Manual };                   /* transaction mode status */

private:
	char m_sUserName[USERNAME_LEN+1]; 
	char m_sPassword[PWD_LEN+1];     
	char m_sDatabase[DB_LEN+1];     
	char m_sServer[SERVER_LEN+1];  
	char m_sDBID[DB_LEN+SERVER_LEN+1];

public:
                /** \brief ���õ�¼���ݿ��û���
		 *  \param sUserName - �û��� [����]			
                 */
	void SetUserName(char *sUserName);     
                /** \brief ���õ�¼���ݿ�����
		 *  \param sPassword - ���� [����]			
                 */
	void SetPassword(char *sPassword);    
                /** \brief ���õ�¼���ݿ���
		 *  \param sDatabase - ���ݿ��� [����]			
                 */
	void SetDatabase(char *sDatabase);   
                /** \brief ���õ�¼���ݿ��������
		 *  \param sDatabase - ���ݿ�������� [����]			
                 */
	void SetServer(char *sServer);      

private:
	long m_nCode;                      
	char m_sMsg[4096];                

	enum emStatus      m_emStatus;   
	enum emTransMode   m_emTransMode;

public:
	/** \brief ���ݿ�����
	* \return �ɹ�����true��ʧ�ܷ���false;
	 */
	BOOL Connect(char *sUserName,char *sPassword,char *sDatabase,char *sServer);
	/** \brief �Ͽ�����
	* \return �ɹ�����true��ʧ�ܷ���false;
	 */
	BOOL Disconnect();              
	/** \brief ������ݿ��Ƿ��Ծ
	* \return ��Ծ״̬����true�����򷵻�false;
	 */
	BOOL CheckDBActive() const;          

protected:
	/** \brief �������״̬
	* \return ���ӳɹ�����true�����򷵻�false;
	 */
 	BOOL IsConnected() const; 
	/** \brief ������״̬
	 */
	void CheckError(); 
	/** \brief �ͷ���Դ
	 */
	void FreeResource();

public:
	/** \brief ��ȡ������Ϣ
	 *  \param nErrorCode - ���ڱ������ݿ�������ı��� [���]			
	 *  \param sErrorMsg - ���ڱ������ݿ������Ϣ�Ļ��� [���]			
	 */
	void GetLastError(long &nErrorCode,char *sErrorMsg);
	/** \brief ���ô�����Ϣ
	 *  \param nErrorCode - ��������ֵ [����]			
	 *  \param sErrorMsg - ������Ϣ [����]			
	 */
	void SetLastError(long nErrorCode = 0,char *sErrorMsg = NULL); 
	/** \brief ��ʼһ������
	* \return �ɹ�����true�����򷵻�false;
	 */
	BOOL BeginTrans();
	/** \brief �ع�һ������
	* \return �ɹ�����true�����򷵻�false;
	 */
	BOOL RollbackTrans();
	/** \brief �ύһ������
	* \return �ɹ�����true�����򷵻�false;
	 */
	BOOL CommitTrans();
	/** \brief �ж�һ������
	* \return �ɹ�����true�����򷵻�false;
	 */
	BOOL BreakTrans();
};

/** \brief ����Ƿ�����SQL����,���ش�����Ϣ
 *  \param outstr ������Ϣ����
 *  \param outstrlen ������Ϣ���峤��
 *  \return ����SQLCODE,0��ʾû�д���.
 */
int ChkError(char* outstr,int outstrlen);

/** \brief ȡ��Informixһ����������Ӧ�ľ��������Ϣ
 *  \param sqlcode ��Ҫ���ҵĴ������
 *  \param isamcode isamcode
 *  \return ���ؾ��������Ϣ,���ں�������new������,������Ҫ�����ͷ�
 */
char* getInformixErrMsg(long int sqlcode, long int isamcode);

#endif
