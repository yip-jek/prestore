#ifndef I_TASK_MONITOR_H
#define I_TASK_MONITOR_H

#include <list>

using std::list;

/** @defgroup libjk ��ؿ��ƵĹ����ӿ���(libjk)
@{
*/

/*! \file ITask.h
 *  \brief ��ؿ��ƵĹ����ӿ���
 */

/*! \class ITask
 *  \brief ��ؿ��ƵĹ����ӿ���
 */

/** \example exITask.cpp
 *  \brief ITask��ʾ������ 
 */

#include "THeartBeat.h"

class ITask
{
public:
	/*! ����˵��: ���캯�� */
	ITask();

	/*! ����˵��: �������� */
	~ITask();

	/*! ����˵��: ��������ID. \n
	 *
	 *  ����ID���ɽ��̵ĵ�һ���������������
	 *  @param[in] iTaskID ����ID
	 *  @return ��
	 */
	void setTaskID(int iTaskID);

	/*! ����˵��: ���ý���UNIX ID.\n
	 *
	 *  �÷����������ݿ���Ӧ��¼��UnixID�ֶ�
	 *  ����������HBENABLE==1ʱ����ʼ��THeartBeat
	 *  @param[in] iUnixID Unix ID��
	 *  @return  ���� 0 ��ʾ�ɹ�, -1 ��ʾʧ��
	 */
	int setProcUnixID(int iUnixID);
	
	/*! ����˵��: ��ȡ��������.����THeartBeat��Ϣ
	 *
	 *  @param[in] pszCmdParam 
	 *  @param[in] iParamBufLen
	 *  @return �����ʶ��,0Ϊ������1Ϊ������2Ϊֹͣ��3Ϊ���� 
	 *  	     ע��getTask���᷵��1���������������ɿ��ƴ������
	 *           ����-1��ʾ getTask����
	 */
	int getTask(char* pszCmdParam,int iParamBufLen);

	/*! ����˵��: ��������ִ�н��. \n
	 *
	 *
	 *  @param bSuccess �����Ƿ�ɹ�
	 *  @return �ɹ����·���0��ʧ�ܷ���-1
	 */
	int setTaskResult(bool bSuccess);

	/*! ����˵��: ���ñ�����ֹͣ��־.\n
	 *
	 *
	 *  @param bNormal �Ƿ������˳�
	 *  @return �ɹ�����0��ʧ�ܷ���-1
	 */
	int setProcQuit(bool bNormal);

	/*! ����˵��: ��ȡ���һ�δ�����Ϣ.\n
	 *
	 *
	 * @return ������Ϣ�ַ�����ַ,�û�����Ҫ�����ͷ��ڴ涯�� 
	 */
	const char* getLastError();

private:
	
	/* ����������HBENABLE==1ʱ����ʼ��THeartBeat , �ɹ�����0*/
	int init_hb();

private:
	/*! �������������ݽṹ */
	struct STASKDETAIL
	{
		/*! ��ϸ����ļ�¼ID */
		int cid_id;
		/*! �������� */
		int cct_id;
		/*! ���� */
		char cmdparam[2048+1];
	};

	/*! ������cid_id */
	int  m_cid_id;

	/*! Ŀ��cid_id */
	int  m_cue_target;

	/*! ������Ϣ������ */
	char m_errbuf[512+1];

	/*! ���������� */
	list<STASKDETAIL> m_listTaskGrp;

	/*! �Ƿ������������α� */
	bool m_bTaskDecl;

	/*! �Ƿ�������setProcUnixId's Prepare */
	bool m_bUnixIdDecl;

	/*! �Ƿ�������setTaskResult's Prepare */
	bool m_bResultDecl;

	/*! �Ƿ�������setProcQuit's Prepare */
	bool m_bQuitDecl;

	THeartBeat * m_hb;
	bool m_hbenable;
};

/** @} */

#endif
