#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

#define USING_OLD_IOSTREAM

#include <fstream>
#include <string>

#include "Prelude.h"
#include "Logger_Impl.h"

BEGIN_SUNRISE_NAMESPACE

/** @addtogroup libsr 
@{
*/

/*! \file FileLogger.h
    \brief 文件日志记录器
*/
/*! \class FileLogger
 * \brief 文件日志记录器
 *
 * 文件日志记录器实现类, 日志信息记录在指定的文件.
 */
class FileLogger : public Logger_Impl
{
public:
	/*! \brief 功能说明：缺省构造函数 */
	FileLogger ();

	/*! \brief 功能说明:打开日志文件. \n
	 * 
	 *  @param[in]  logfname_ 日志文件名
	 *  @param[in]  groups_   日志掩码组
	 *  @param[in]  maxsize__   日志文件最大大小
	 * @return 0表示成功，-1表示失败
	*/
	virtual int open (const char* logfname_, 
			  u_long      groups_,
			  u_long      maxsize_ = 10485760); // 10 Mb = 1024x1024x10

	/*! \brief 功能说明:关闭日志文件. \n
	*
	*    刷新输出流数据缓冲，关闭输出日志文件
	*  @return 0表示成功
    */
	virtual int  close  (void);

	/*! \brief 功能说明:刷新输出流数据缓冲到日志文件 */
	virtual void resync (void);

    /*!  \brief 功能说明:在输出流中输出日志信息,
	 *
	 *   根据日志掩码组在日志文件输出类型标志,若系统时间输出标志为TRUE,则输出系统时间，若函数名不为空输出函数名,
	 *   再输出日志信息，若日志文件大于最大限制值，并团日志文件名，更改日志文件名，重打开原日志文件. 
	 *   @param[in]  groups_      日志掩码
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
     *   根据日志掩码组在日志文件输出类型标志,若系统时间输出标志为TRUE,则输出系统时间，若函数名不为空输出函数名,
	 *   若日志文件大于最大限制值，并团日志文件名，更改日志文件名，重打开原日志文件
	 *   @param[in]  groups_      日志掩码
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
	
   /*! \brief 文件打开状态值
     * - opened 文件已条开
	 * - closed 文件已关闭
	*/
	enum state_t { opened, closed };
    /*!  \brief 功能说明：拷贝函数。 */
	FileLogger (const FileLogger&);            // copy constructor
	/*!  \brief 功能说明：赋值函数。 */	
	FileLogger& operator= (const FileLogger&); // copy assignment

	int handle_rollover (void);

private:
	/*! \brief 输出流 */
	std::ofstream  m_sink;
	/*! \brief 日志文件最大限制,缺省为10M */
	u_long    m_maxsize;
	/*! \brief 日志文件打开关闭状态 */
	state_t   m_state;
	/*! \brief 日志文件当前大小 */
	u_long    m_bytecount;
};

inline FileLogger::
FileLogger () 
    : m_maxsize   (1048576), 
      m_state     (closed),
      m_bytecount (0)
{
	/*--- empty ---*/
}

inline void 
FileLogger::
resync (void)
{
	m_sink << std::flush;
}

/** @} */

END_SUNRISE_NAMESPACE

#endif /* FILE_LOGGER_H */
