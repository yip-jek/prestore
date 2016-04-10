#ifndef IGETMQ_H
#define IGETMQ_H

#include <vector>

using std::vector;

/** @defgroup libjk MQ接口类(libjk)
@{
*/

/*! \file IGetMQ.h
 *  \brief MQ接口类
 */

/*! \brief MQ相关信息结构
 */
struct SMQINFO
{
	/*! \brief 队列管理器名
	 */
	char qm_name[60+1];
	/*! \brief 队列名
	 */
	char q_name[60+1];
	/*! \brief 队列方向，0表示输入队列，1表示输出队列
 	 */
	int  readwrite;
	/*! \brief 全局的MQ号，也就是表Monp0_Mq的CMQ_ID
	 */
	int  mq_id;
};

/*! \brief MQ信息数组
 */
typedef vector<SMQINFO> ArrMqInfo;

/*! \class IGetMQ
 *  \brief 获取MQ的信息接口类
 */
class IGetMQ
{
public:
	/*! \brief 构造函数
 	 */
	IGetMQ();
	/*! \brief 获取MQ信息
	 * @param cmdmq_id 进程号，对应命令行的第3个参数，表Monp0_Command的CCM_ID 
	 * @return 存放SMQINFO结构的vector指针
	 */
	ArrMqInfo* getMqInfo(int cmdmq_id);
	/*! \brief 获取最后错误信息
	 * @return 错误信息
	 */
	const char* getLastError();
private:
	/*! \brief 错误信息缓存区
	 */
	char m_errbuf[100+1];
};

/** @} */

#endif
