/*
 */

#ifndef __THEARTBEAT_H__
#define __THEARTBEAT_H__

/** @defgroup libtrap 
@{ 
 */

/**
 * @class THeartBeat
 * @brief 心跳包类
 * 功能点
 * 1、  信息队列的key值为宏HB_MSG_KEY，可以通过环境EXHB_MSG_KEY 改变
 *      a、当环境EXHB_MSG_KEY < 0时，关闭整个心跳包功能。
 *		b、当环境EXHB_MSG_KEY >= 0时，key值为：HB_MSG_KEY+EXHB_MSG_KEY
 *
 * 2、  发送心跳包功能，当msgbuf为空串""时，为简单的心跳消息.受环境变量TRAPTIMES影响。
 * 		A、TRAPTIMES配置为0时，不发送；
 *		B、TRAPTIMES为正整数时，标示每TRAPTIMES发送一次给监控前台。
 *		C、TRAPTIMES没有配置时或配置为负数时被重置为1。
 *
 * 3、  发送指标数据功能，当msgbuf不为空时，应为"指标:指标值"，监控前台要求。
 *
 * 4、  ITask类的getTask增加：当环境变量HBENABLE==1时，发送心跳包功能。
 *
 *
 *
 * @file THeartBeat.h
 * @brief 心跳包类头文件
 *
 * @example exTHeartBeat.cpp
 */
/** @cond */
/*
 * 接口组用gcc编译，取了这个源码
 *Gcc编译为执行文件时加“-lpthread  -lstdc++”即可，如果是编译.o或打包 .a 时不用特别处理。
 */
/** @endcond */

#include <unistd.h>

#define HB_MSG_KEY 0x20001000	/* 信息队列的key值，可以通过环境EXHB_MSG_KEY偏移,但不建议使用以免冲突 */

class THeartBeat {
private :
	int m_key;
	int m_msgid;
	int m_nosend;

	int m_calltimes;
	int m_traptimes;	
	pid_t m_pid;   /* 记录初始化是进程id */

	int m_secid;
	int m_ccmid;
	
	char m_errbuf[1024];
public :
	THeartBeat();
	~THeartBeat();

	/**
	 * @brief 初始化消息队列
	 *		1、TRAPTIMES没有配置时或配置为负数时，被重置为1，
	 *		2、信息队列的key值为宏HB_MSG_KEY，可以通过环境EXHB_MSG_KEY 改变
	 *			a、当环境EXHB_MSG_KEY < 0时，关闭整个心跳包功能。
	 *			b、当环境EXHB_MSG_KEY >= 0时，key值为：HB_MSG_KEY+EXHB_MSG_KEY（不建议使用）。
	 * @param secid 即监控前台的se_cid_id
	 * @param ccmid 己监控前台的int_ccm_id 
	 * @return 成功返回0， 失败返回-1,可通过hb_error（）得到原因
	 */
	int hb_init(int secid, int ccmid);

	/**
	 * @brief 发送trap消息到信息队列
	 * @param msgbuf 要发送的消息
	 * 		1、当msgbuf为空串""时，为简单的心跳消息.受环境变量TRAPTIMES影响.
	 * 				TRAPTIMES配置为0时，不发送；
	 * 				TRAPTIMES为正整数时，标示每TRAPTIMES发送一次给监控前台。
	 * 		2、当msgbuf不为空时，应为"指标:指标值"，监控前台要求
	 * @return 成功返回0， 失败返回-1,可通过hb_error（）得到原因, 返回1时标识通过EXHB_MSG_KEY关闭了发生功能。
	 */
	int hb_send(const char * msgbuf);

	/**
	 * @brief 接收trap消息
	 * @param msgbuf 用于存放接收的组合后的消息
	 * @return 成功返回实际msgbuf长度，失败返回-1,没有消息返回0
	 */
	int hb_receive(char * msgbuf);

	/**
	 * @brief 返回最后的失败原因
	 */
	const char * hb_error();
};

/**
  @}   
 */
#endif
