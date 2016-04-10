/**
 * File name:    writelogtab.h
 * @author:      周明亮
 * @version:     V01.01.100   2003/04/09
 * Description:  定义写日志表类
 *				 使用前要先连接数据库
 * Log:

 */
#ifndef __WRITELOGTAB_H__
#define __WRITELOGTAB_H__

/** @defgroup libjk 定义处理层进程初始化类(libjk)
@{
 */

/*! \file writelogtab.h
 *  \brief 定义处理层进程初始化类
 */

/*! \class CWriteLogTab
 *  \brief 定义处理层进程初始化类
 */

class CWriteLogTab {
	public:
		/*! \brief 构造函数
		 */
		CWriteLogTab();
		/*! \brief 析构函数
		 */
		//	~CWriteLogTab();
		/*! \brief 设置时间
		 * @param str 14位时间
		 */
		int settime(const char* str);
		/*! \brief 设置kpi标识(14位)
		 * @param str kpi表示
		 */
		int setkpiid(const char* str);
		/*! \brief 设置kpi来源
		 * @param str kpi来源
		 */
		int setkpisrc(const char* str);
		/*! \brief 设置网元id
		 * @param id 网元id
		 */
		int setunid(const int id);

		/*! \brief 设置kpi值
		 * @param str kpi值，具体含义要根据kpiid而定
		 */
		int setkpivalue(const char* str);
		/*! \brief 设置kpi参数1
		 * @param str kpi参数1，具体含义要根据kpiid而定
		 */
		int setpara1(const char* str);
		/*! \brief 设置kpi参数2
		 * @param str kpi参数2，具体含义要根据kpiid而定
		 */
		int setpara2(const char* str);
		/*! \brief 设置kpi参数3
		 * @param str kpi参数3，具体含义要根据kpiid而定
		 */
		int setpara3(const char* str);
		/*! \brief 设置kpi参数4
		 * @param str kpi参数4，具体含义要根据kpiid而定
		 */
		int setpara4(const char* str);
		/*! \brief 设置kpi参数5
		 * @param str kpi参数5，具体含义要根据kpiid而定
		 */
		int setpara5(const char* str);
		/*! \brief 获取数据库操作失败的信息
		 * @Param：			无
		 * @Return：			数据库操作失败的信息
		 */
		virtual char* getOptDBErrs();
		/*! \brief 写入日志
		 * 每次写入之后，会把时间字段清空,如果下次调用该方法前没有设置时间,
		 * 则该方法会把当前时间写入日志 
		 */
		int writelog();
		/*! \brief 清除各字段内容
		 */
		int clear();

	protected:

		/*! \brief 存放数据库操作出错信息
		 */
		char m_OptDBErrs[100];
	private:
		char m_kpiid[14+1];	//!< KPI标识
		char m_kpisrc[30+1];	//!< kpi来源
		char m_time[14+1]; 	//!< 生成时间
		int  m_un_id;   	//!< 网元
		char m_kpivalue[50+1];	//!< kpi值
		char m_para1[50+1];	//!< kpi参数1
		char m_para2[50+1];	//!< kpi参数2
		char m_para3[50+1];	//!< kpi参数3
		char m_para4[50+1];	//!< kpi参数4
		char m_para5[50+1];	//!< kpi参数5
		//		char m_unname[50+1];	//!< 网元名称	


};

/** @} */

#endif
