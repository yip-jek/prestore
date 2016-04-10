#ifndef IGETMQ_H
#define IGETMQ_H

#include <vector>

using std::vector;

/** @defgroup libjk MQ�ӿ���(libjk)
@{
*/

/*! \file IGetMQ.h
 *  \brief MQ�ӿ���
 */

/*! \brief MQ�����Ϣ�ṹ
 */
struct SMQINFO
{
	/*! \brief ���й�������
	 */
	char qm_name[60+1];
	/*! \brief ������
	 */
	char q_name[60+1];
	/*! \brief ���з���0��ʾ������У�1��ʾ�������
 	 */
	int  readwrite;
	/*! \brief ȫ�ֵ�MQ�ţ�Ҳ���Ǳ�Monp0_Mq��CMQ_ID
	 */
	int  mq_id;
};

/*! \brief MQ��Ϣ����
 */
typedef vector<SMQINFO> ArrMqInfo;

/*! \class IGetMQ
 *  \brief ��ȡMQ����Ϣ�ӿ���
 */
class IGetMQ
{
public:
	/*! \brief ���캯��
 	 */
	IGetMQ();
	/*! \brief ��ȡMQ��Ϣ
	 * @param cmdmq_id ���̺ţ���Ӧ�����еĵ�3����������Monp0_Command��CCM_ID 
	 * @return ���SMQINFO�ṹ��vectorָ��
	 */
	ArrMqInfo* getMqInfo(int cmdmq_id);
	/*! \brief ��ȡ��������Ϣ
	 * @return ������Ϣ
	 */
	const char* getLastError();
private:
	/*! \brief ������Ϣ������
	 */
	char m_errbuf[100+1];
};

/** @} */

#endif
