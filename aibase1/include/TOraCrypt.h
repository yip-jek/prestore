#if !defined(__TORACRYPT_H__)
#define __TORACRYPT_H__

#define USING_OLD_IOSTREAM
#include <map>
#include <string>

#include "TOraCon.h"

using std::map;
using std::string;

#define DB_CRYPT "DB_CRYPT"
#define LINE_LEN 256
#define DB_ENV_LEN 100
#define DB_ID 200

/*! \example TDBC_Test.cpp
 * ��������Գ���
 */

/*! \class TOraCrypt
 *
 * \brief ������ - �ӻ�������DB_CRYPT��ָ�����ļ��н��ܳ��������ݿ���û���/����,���ݿ�/����.
 *     ���׵Ļ���adddb����.
 * 
 */
class TOraCrypt {
	public:
		typedef struct DBEnv_ {
			char user[DB_ENV_LEN+1];
			char pwd[DB_ENV_LEN+1];
			char db[DB_ENV_LEN+1];
			char host[DB_ENV_LEN+1];
			TOraCon* con;
		}DBEnv;

	public:
		typedef map<string, DBEnv*> DBENV_MAP;
		typedef map<string, DBEnv*>::iterator DBENV_MAP_IT;

	public:
		/*! \brief ���캯��
		 */
		TOraCrypt();

		/*! \brief ��������
		 */
		virtual ~TOraCrypt();

		/*! \breif �����Զ��ͷ�
		 */
		void setAutoDelete(bool enable = true);

		/*! \brief ��ʼ��
		 * �ӻ�������DB_CRYPT���������ļ������������ݿ���ص��û��������룬���ݿ������������ԡ�
		 */
		virtual bool init(bool connect = false);

		/*! \brief ��ȡĳ���ݿ�instance����������(�����û���, ���룬���ݿ�����������
		 * @param id - ���ݿ�instance��ʶ. ��ʽ�� database@host(����gmcc_boss@boss)
		 * @param user - �û��� [���]
		 * @param pwd - ���� [���]
		 * @param db - ���ݿ��� [���]
		 * @param host - ������ [���]
		 *
		 * @return �ɹ�ȡ�ü�¼����true, ����user,pwd,db,host��������������з���ֵ������
		 *            ����false, user,pwd,db,host�������á�
		 *            ע�� user,pwd,db,host������������ͷŵ�.delete [] user;...
		 */
		bool fetchUserPwd(const char * id, char** user, char** pwd, char** db, char** host);

		/*! \brief ��ȡ��ǰ���ӳ��ڵ�һ�����ݿ�����
		 */
		TOraCon* getCurrentConnection(void);

		/*! \brief ��ȡ���ݿ����Ӿ��
		 *
		 * @param id - ���ݿ�instance��ʶ. ��ʽ�� database@host(����gmcc_boss@boss)
		 */
		TOraCon* useDBConnection(const char * );

		/*! \brief ��ȡ���ݿ����Ӿ��
		*/
		const TOraCon* getDBConnection();

		/*! \brief �ͷ���Դ - �������autodeleteΪfalse��Ҫ�ֹ��ͷ���Դ.
		 */
		void Destroy(void);

	private: //������ֵ���� �� ��������!
		TOraCrypt(const TOraCrypt& e);
		TOraCrypt& operator=(const TOraCrypt& e); 
		const TOraCrypt& operator=(const TOraCrypt& e) const; 

	private:
		bool SplitEnv(const char* env, char* user, char* pwd, char* db, char* host);
	
	private:
		DBENV_MAP m_DBMap;

	protected:
		TOraCon* m_dbconnection;
		bool m_AutoDel;
		bool m_SetEnv; /*!< ��ʶ�����Ƿ����ú� */
		bool m_FOK;  /*!< ��ʶ�����ļ��Ƿ��ܶ� */
		bool m_Connected; /*!< ��ʶ�Ƿ��Ѿ��������ݿ� */
};

#else
class TOraCrypt;
#endif
