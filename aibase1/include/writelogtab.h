/**
 * File name:    writelogtab.h
 * @author:      ������
 * @version:     V01.01.100   2003/04/09
 * Description:  ����д��־����
 *				 ʹ��ǰҪ���������ݿ�
 * Log:

 */
#ifndef __WRITELOGTAB_H__
#define __WRITELOGTAB_H__

/** @defgroup libjk ���崦�����̳�ʼ����(libjk)
@{
 */

/*! \file writelogtab.h
 *  \brief ���崦�����̳�ʼ����
 */

/*! \class CWriteLogTab
 *  \brief ���崦�����̳�ʼ����
 */

class CWriteLogTab {
	public:
		/*! \brief ���캯��
		 */
		CWriteLogTab();
		/*! \brief ��������
		 */
		//	~CWriteLogTab();
		/*! \brief ����ʱ��
		 * @param str 14λʱ��
		 */
		int settime(const char* str);
		/*! \brief ����kpi��ʶ(14λ)
		 * @param str kpi��ʾ
		 */
		int setkpiid(const char* str);
		/*! \brief ����kpi��Դ
		 * @param str kpi��Դ
		 */
		int setkpisrc(const char* str);
		/*! \brief ������Ԫid
		 * @param id ��Ԫid
		 */
		int setunid(const int id);

		/*! \brief ����kpiֵ
		 * @param str kpiֵ�����庬��Ҫ����kpiid����
		 */
		int setkpivalue(const char* str);
		/*! \brief ����kpi����1
		 * @param str kpi����1�����庬��Ҫ����kpiid����
		 */
		int setpara1(const char* str);
		/*! \brief ����kpi����2
		 * @param str kpi����2�����庬��Ҫ����kpiid����
		 */
		int setpara2(const char* str);
		/*! \brief ����kpi����3
		 * @param str kpi����3�����庬��Ҫ����kpiid����
		 */
		int setpara3(const char* str);
		/*! \brief ����kpi����4
		 * @param str kpi����4�����庬��Ҫ����kpiid����
		 */
		int setpara4(const char* str);
		/*! \brief ����kpi����5
		 * @param str kpi����5�����庬��Ҫ����kpiid����
		 */
		int setpara5(const char* str);
		/*! \brief ��ȡ���ݿ����ʧ�ܵ���Ϣ
		 * @Param��			��
		 * @Return��			���ݿ����ʧ�ܵ���Ϣ
		 */
		virtual char* getOptDBErrs();
		/*! \brief д����־
		 * ÿ��д��֮�󣬻��ʱ���ֶ����,����´ε��ø÷���ǰû������ʱ��,
		 * ��÷�����ѵ�ǰʱ��д����־ 
		 */
		int writelog();
		/*! \brief ������ֶ�����
		 */
		int clear();

	protected:

		/*! \brief ������ݿ����������Ϣ
		 */
		char m_OptDBErrs[100];
	private:
		char m_kpiid[14+1];	//!< KPI��ʶ
		char m_kpisrc[30+1];	//!< kpi��Դ
		char m_time[14+1]; 	//!< ����ʱ��
		int  m_un_id;   	//!< ��Ԫ
		char m_kpivalue[50+1];	//!< kpiֵ
		char m_para1[50+1];	//!< kpi����1
		char m_para2[50+1];	//!< kpi����2
		char m_para3[50+1];	//!< kpi����3
		char m_para4[50+1];	//!< kpi����4
		char m_para5[50+1];	//!< kpi����5
		//		char m_unname[50+1];	//!< ��Ԫ����	


};

/** @} */

#endif
