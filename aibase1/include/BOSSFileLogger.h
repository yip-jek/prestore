
#ifndef BOSSFILE_LOGGER_H
#define BOSSFILE_LOGGER_H

#define USING_OLD_IOSTREAM

#include <fstream>
#include <string>

#include "Prelude.h"
#include "Logger_Impl.h"


BEGIN_SUNRISE_NAMESPACE

#define LOG_SEQ_DELIMER '_'

/** @defgroup libsr 日志输出模块(libsr) 
@{
*/

/*! \file BOSSFileLogger.h
    \brief BOSS系统的文件日志记录器
*/
/*! \class BOSSFileLogger
 *\brief BOSS文件日志记录器实现类
 *
 * 打开日志的时候指定CCM_ID，日至记录器自动按照
 * BOSS系统的规则产生日志文件名，并且在日至文件大于某个指定的尺寸时自动
 * 产生新的日志序号产生日志文件名。
 */
class BOSSFileLogger : public Logger_Impl
{
public:

	/*! \brief 功能说明：构造函数 */
	BOSSFileLogger ();

    /*!  \brief 功能说明:打开日志文件. \n
	 * 
	 *  通过提供输出目录，以进程ID，日期和顺序号产生文件名，确保在输出目录不存在相同的日志文件名,并打开文件
	 *  @param[in]  base_dir_ 日志输出目录
	 *  @param[in]  ccm_id_   进程编号
	 *  @param[in]  groups_   日志掩码组
	 *  @param[in]  maxsize_   日志文件最大大小
	 *  @param[in]  seq_delimer_   日志文件名分隔符
	 * @return 0表示成功，-1表示失败
	*/
	virtual int open (const char*	base_dir_,
			  int		 ccm_id_,
			  u_long      groups_,
			  u_long      maxsize_ = 10 * 1024 * 1024,  // 10 Mb = 1024x1024x10
			  char 		  seq_delimer_ = LOG_SEQ_DELIMER,
			  int 		switchday = 5); 
   
	/*!  \brief 功能说明:关闭日志文件. \n
	*
	*    刷新输出流数据缓冲，关闭输出日志文件，释放占用资源
	*  @return 0表示成功
    */
	virtual int  close  (void);

    /*!  \brief 功能说明:刷新输出流数据缓冲到日志文件 */
	virtual void resync (void);

    /*!  \brief 功能说明:在输出流中输出日志信息,
	 *
	 *   根据日志掩码位在日志文件输出类型标志,若系统时间输出标志为TRUE,则输出系统时间，若函数名不为空输出函数名,
	 *   再输出日志信息，若系统时间满足切换条件或日志文件大于最大限制值，进行日志文件名切换, 
	 *   关闭原日志文件，打开新日志文件
	 *   @param[in]  groups_      日志掩码位
	 *   @param[in]  indent_level 缩进层次
	 *   @param[in]  func_name_   函数名称
	 *   @param[in]  fmt_         输出格式
	 *   @param[in]  va_list      输出值列表
	 * @return 0表示成功
	 * @pre 日志文件成功打开后
	*/
	virtual int log_msg (Group g_, 
						 size_t indent_level_,
						 const string& func_name_,
						 const char* fmt_, va_list);

   /*!  \brief 功能说明：在输出流中输出函数名 
     *
     *   根据日志掩码在日志文件输出类型标志,若系统时间输出标志为TRUE,则输出系统时间，若函数名不为空输出函数名,
	 *   若系统时间满足切换条件或日志文件大于最大限制值，进行日志文件名切换,关闭原日志文件，打开新日志文件
	 *   @param[in]  groups_      日志掩码位
	 *   @param[in]  indent_level 缩进层次
	 *   @param[in]  func_name_   函数名称
	 * @param[in] type_  字符串输出标志位，取值如下 \n           
	 * - FUNC_ENTRY 输出为'/'
     * - FUNC_EXIT  输出为'\'
	 * - FUNC_EXIT  输出为'[]'
	 * @return 0表示成功
 	 * @pre 日志文件成功打开后
   */
	virtual int log_func (Group g_, 
						  size_t indent_level_,
						  const string& func_name_,
						  marker_t type_);

   /*!  \brief 功能说明：输出类成员变量中的值，现在未启用     */
	void dump (void);

private:
	enum state_t { opened, closed };
   /*!  \brief 功能说明：拷贝函数。 */
	BOSSFileLogger (const BOSSFileLogger&);            // copy constructor
   /*!  \brief 功能说明：赋值函数。 */	
	BOSSFileLogger& operator= (const BOSSFileLogger&); // copy assignment
   /*!  \brief 功能说明：根据系统时间和成员变量保存先前年月日确定是否存在日志文件名在切换。
   * @return true表示要进行日志文件名切换，flase不作任何处理。
   */
	bool switch_dt(void);
  /*!  \brief 功能说明：根据系统时间和日志文件大小切换日志文件名。 
   * @return 0表示成功，-1表示失败
   */
	int handle_rollover ( void );

private:
	/*! \brief 输出流 */
	std::ofstream  m_sink;
	/*! \brief 进程号*/
	int	  m_ccm_id;
	/*! \brief 分隔符,缺省为"_" */
	char      m_seqdelimer;
	/*! \brief 日志文件名年份 */
	int 	  m_year;
	/*! \brief 日志文件名月份 */
	int	  m_month;
	/*! \brief 日志文件名日份 */
	int 	  m_day;
	/*! \brief 目志文件输出目录 */
	char*	  m_basedir;
	/*! \brief 应用程序名 */
	char*   m_app_name;
	/*! \brief 日志文件最大限制,缺省为10M */
	u_long    m_maxsize;
	/*! \brief 日志文件打开关闭状态 */
	state_t   m_state;
	/*! \brief 日志文件当前大小 */
	u_long    m_bytecount;
	/*! \brief 日志切换天数,默认5天一次*/
	int m_switchday;
};

inline BOSSFileLogger::
BOSSFileLogger () 
    : m_maxsize   (10 * 1024 * 1024), 
      m_state     (closed),
      m_bytecount (0),
	  m_switchday (5)
{
	/*--- empty ---*/
}

inline void 
BOSSFileLogger::
resync (void)
{
	m_sink << std::flush;
}

END_SUNRISE_NAMESPACE

/** @} */

#endif /* FILE_LOGGER_H */
