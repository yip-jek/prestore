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



/** \defgroup libdb   ���ݿ��̿�(libdb) */
/** \{*/
/** \file TOraCon.h
 *  \brief oracle�������ͷ�ļ�  */

/** \class TOraCon
 *  \brief oracle������
 */

/** \example exDB.cpp 
 *  \example exInf.ecpp
 *  \brief TDBInstance TQueryEx TQueryOra TInformixEx TOraCon ��ʾ������ 
 */


/** \}*/

class TOraCon
{
public:

	TOraCon();

	virtual ~TOraCon();

public:
        /**\enum emStatus { NotInited,Connected,Disconnected }
         *  \brief ���ڱ�ǵ�ǰ���ݿ�״̬��������
	 */
	enum emStatus { NotInited,Connected,Disconnected };  /* connection mode status  */

        /**\ enum emTransMode { Auto, Manual };
         *  \brief ���ڱ��������ķ�ʽ���û�����,�����Զ�
	 */
	enum emTransMode { Auto, Manual };                   /* transaction mode status */

        /**\ emsavepoint {Point_A,Point_B,Point_C,Point_D,Point_E}
         *  \brief ���ڱ����������������
	 */
	enum emsavepoint {Point_A,Point_B,Point_C,Point_D,Point_E,Point_ALL} ;   /* savepoint step  */

private:
	char m_sUserName[USERNAME_LEN+1]; 
	char m_sPassword[PWD_LEN+1];     
	char m_sServer[SERVER_LEN+1];  
	char m_sLinkName[SERVER_LEN+1];

public:
                /** \brief ���õ�¼���ݿ��û���
                 *  \param sUserName - �û��� [����]
                 */
	void SetUserName(char *sUserName);     
                /** \brief ���õ�¼���ݿ�����
                 *  \param sPassword - ���� [����]
                 */
	void SetPassword(char *sPassword);    
                /** \brief ���õ�¼���ݿ��������
                 *  \param sDatabase - ���ݿ�������� [����]
                 */
	void SetServer(char *sServer);      
                /** \brief �����������ݿ��ʶ��
                 *  \param sLinkName - ���ݿ��ʶ�� [����]
                 */
	void SetLinkName(char *sLinkName);      

private:
	long m_nCode;                      
	char m_sMsg[MSG_LEN];                

	enum emStatus      m_emStatus;   
	enum emTransMode   m_emTransMode;

public:

        /** \brief ���ݿ�����,
	 *  \param sUserName  ��¼�û�;NULL��ͨ����������sUserName��������ļ������ӡ�
	 *  \param sPassword  ��¼����;NULL��ͨ����������sUserName������ļ������ӡ�
	 *  \param sUserName  ��¼���ݿ�ķ�����,һ������Զ�̵�¼��ȱʡΪ����('@'�󲿷�);��
	 *  \param linkname   ���������ǿ�ֵʱ��SQL��������ݿ�Ĳ���SQLҪ�� AT :linkname;
         *  \return �ɹ�����true��ʧ�ܷ���false;
         */
	BOOL Connect(char *sUserName,char *sPassword,char *sServer,char *linkname);


        /** \brief ���ݿ�����,ͨ���������ļ�$devis_home/devis/restrict/$ORACLE_SIDʵ�ֱ��ص�¼
         *  \return �ɹ�����true��ʧ�ܷ���false;
         */
	BOOL oraconnect();
       

        /** \brief �Ͽ�����
         *\return �ɹ�����true��ʧ�ܷ���false;
         */
        BOOL Disconnect();        

        /** \brief ������ݿ��Ƿ��Ծ
        * \return ��Ծ״̬����true�����򷵻�false;
         */
	BOOL CheckDBActive() const ;          

protected:
 	BOOL IsConnected() const; 

	bool CheckError(); 

	void FreeResource();

public:

	  /**\brief ���ݷ��������������ļ�DB_CRYPT���Ҷ�Ӧ���ʻ�����
            * \param sServer Ҫ���ӵ����ݿ��������,��Oracle��tnsnames.ora����
            * \return ����Ϊfalse;����Ϊtrue;
            */
	BOOL ScanDesfile(char * sServer);
             /**\brief �������봮�ĸ��ֶ���Ϣ
              * \param env Ҫ�������ַ���
              * \param sServer �ʻ�
              * \param sServer ����
              * \param sServer ���ݿ⣬����ORACLE��Ч
              * \param sServer Ҫ���ӵ����ݿ��������
              * \return ����Ϊfalse;����Ϊtrue;
              */
	BOOL SplitEnv(const char* env, char* user, char* pwd, char* db, char* host);

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

        /** \brief ��������ʼ,û��ʵ��
	 *  \return true
         */
	BOOL BeginTrans();

        /** \brief ������������,�����Ա���5��
         *  \param step -Ҫ�����������ʶ ȡֵΪ0-4,ȱʡΪ0 [����]        
         *  \return ������������ʶֵ��Χ�򷵻�false,����Ϊtrue;
         */
	BOOL SaveTrans(emsavepoint step=Point_A);


        /** \brief ��������ع���,��Ӧ��������
         *  \param step -Ҫ�ع��������ʶ ȡֵΪ0-4,ȱʡΪ0,����Ϊ�������������ع�[����]        
         *  \return ����true;
         */
	BOOL RollbackTrans(emsavepoint step=Point_ALL);

        /** \brief �ύ��������
        * \return  �ɹ�����true�����򷵻�false;
         */
	BOOL CommitTrans();

        /** \brief �ж�һ������,oracle�Դ���ʵ��
        * \return �ɹ�����true�����򷵻�false;
         */
	BOOL BreakTrans();
};

/** \brief ����Ƿ�����SQL����,���ش�����Ϣ
 *  \param outstr ������Ϣ����
 *  \param outstrlen ������Ϣ���峤��
 *  \return ����SQLCODE,0��ʾû�д���.
 */
int ChkError(char* outstr, int outstrlen);

#endif
