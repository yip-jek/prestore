/*
 */

#ifndef __THEARTBEAT_H__
#define __THEARTBEAT_H__

/** @defgroup libtrap 
@{ 
 */

/**
 * @class THeartBeat
 * @brief ��������
 * ���ܵ�
 * 1��  ��Ϣ���е�keyֵΪ��HB_MSG_KEY������ͨ������EXHB_MSG_KEY �ı�
 *      a��������EXHB_MSG_KEY < 0ʱ���ر��������������ܡ�
 *		b��������EXHB_MSG_KEY >= 0ʱ��keyֵΪ��HB_MSG_KEY+EXHB_MSG_KEY
 *
 * 2��  �������������ܣ���msgbufΪ�մ�""ʱ��Ϊ�򵥵�������Ϣ.�ܻ�������TRAPTIMESӰ�졣
 * 		A��TRAPTIMES����Ϊ0ʱ�������ͣ�
 *		B��TRAPTIMESΪ������ʱ����ʾÿTRAPTIMES����һ�θ����ǰ̨��
 *		C��TRAPTIMESû������ʱ������Ϊ����ʱ������Ϊ1��
 *
 * 3��  ����ָ�����ݹ��ܣ���msgbuf��Ϊ��ʱ��ӦΪ"ָ��:ָ��ֵ"�����ǰ̨Ҫ��
 *
 * 4��  ITask���getTask���ӣ�����������HBENABLE==1ʱ���������������ܡ�
 *
 *
 *
 * @file THeartBeat.h
 * @brief ��������ͷ�ļ�
 *
 * @example exTHeartBeat.cpp
 */
/** @cond */
/*
 * �ӿ�����gcc���룬ȡ�����Դ��
 *Gcc����Ϊִ���ļ�ʱ�ӡ�-lpthread  -lstdc++�����ɣ�����Ǳ���.o���� .a ʱ�����ر���
 */
/** @endcond */

#include <unistd.h>

#define HB_MSG_KEY 0x20001000	/* ��Ϣ���е�keyֵ������ͨ������EXHB_MSG_KEYƫ��,��������ʹ�������ͻ */

class THeartBeat {
private :
	int m_key;
	int m_msgid;
	int m_nosend;

	int m_calltimes;
	int m_traptimes;	
	pid_t m_pid;   /* ��¼��ʼ���ǽ���id */

	int m_secid;
	int m_ccmid;
	
	char m_errbuf[1024];
public :
	THeartBeat();
	~THeartBeat();

	/**
	 * @brief ��ʼ����Ϣ����
	 *		1��TRAPTIMESû������ʱ������Ϊ����ʱ��������Ϊ1��
	 *		2����Ϣ���е�keyֵΪ��HB_MSG_KEY������ͨ������EXHB_MSG_KEY �ı�
	 *			a��������EXHB_MSG_KEY < 0ʱ���ر��������������ܡ�
	 *			b��������EXHB_MSG_KEY >= 0ʱ��keyֵΪ��HB_MSG_KEY+EXHB_MSG_KEY��������ʹ�ã���
	 * @param secid �����ǰ̨��se_cid_id
	 * @param ccmid �����ǰ̨��int_ccm_id 
	 * @return �ɹ�����0�� ʧ�ܷ���-1,��ͨ��hb_error�����õ�ԭ��
	 */
	int hb_init(int secid, int ccmid);

	/**
	 * @brief ����trap��Ϣ����Ϣ����
	 * @param msgbuf Ҫ���͵���Ϣ
	 * 		1����msgbufΪ�մ�""ʱ��Ϊ�򵥵�������Ϣ.�ܻ�������TRAPTIMESӰ��.
	 * 				TRAPTIMES����Ϊ0ʱ�������ͣ�
	 * 				TRAPTIMESΪ������ʱ����ʾÿTRAPTIMES����һ�θ����ǰ̨��
	 * 		2����msgbuf��Ϊ��ʱ��ӦΪ"ָ��:ָ��ֵ"�����ǰ̨Ҫ��
	 * @return �ɹ�����0�� ʧ�ܷ���-1,��ͨ��hb_error�����õ�ԭ��, ����1ʱ��ʶͨ��EXHB_MSG_KEY�ر��˷������ܡ�
	 */
	int hb_send(const char * msgbuf);

	/**
	 * @brief ����trap��Ϣ
	 * @param msgbuf ���ڴ�Ž��յ���Ϻ����Ϣ
	 * @return �ɹ�����ʵ��msgbuf���ȣ�ʧ�ܷ���-1,û����Ϣ����0
	 */
	int hb_receive(char * msgbuf);

	/**
	 * @brief ��������ʧ��ԭ��
	 */
	const char * hb_error();
};

/**
  @}   
 */
#endif
