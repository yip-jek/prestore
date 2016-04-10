
//------------------------------------------------------------------------------
//                            StdOutLogger.h
//------------------------------------------------------------------------------
#ifndef STDOUT_LOGGER_H
#define STDOUT_LOGGER_H

#define USING_OLD_IOSTREAM
#include <fstream>
#include <string>

#include "Prelude.h"
#include "Logger_Impl.h"

BEGIN_SUNRISE_NAMESPACE

/** @addtogroup libsr 
@{
*/

/*! \file StdOutLogger.h 
 *
 * \brief 标准输出日志记录器
 */

/*! \class StdOutLogger StdOutLogger.h
 * \brief 标准输出日志记录器
 *
 *重载Logger_Impl的接口函数，在标准输出流中输出日志信息和函数名，刷新输出buffer.
 */
class StdOutLogger : public Logger_Impl
{
public:
	StdOutLogger () { /*empty*/ }
    
	/*!  \brief 功能说明:设置日志输出类型
	 *   @param[in]  groups_ 日志类型
	 * @return 0表示成功
	*/
	virtual int  open   (u_long  groups_);
	
	/*!  \brief 功能说明:只实现关闭输出流接口，函数不作任何处理 
	 * @return 0表示成功
	*/
	virtual int  close  (void);

    /*!  \brief 功能说明:在标准输出流中输出日志信息,若
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

   /*!  \brief 功能说明：在标准输出流中输出函数名,若系统时间输出标志为TRUE则输出系统时间。 
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

 /*!  \brief 功能说明：刷新标准输出流输出缓冲区。 */
	virtual void resync ();

private:
  /*!  \brief 功能说明：拷贝函数。 */
	StdOutLogger (const StdOutLogger&);            /// no cloning
  /*!  \brief 功能说明：赋值函数。 */
	StdOutLogger& operator= (const StdOutLogger&); /// no cloning
};

/*******************************************************************************
 Inline member functions
*******************************************************************************/
inline int
StdOutLogger::
open (u_long groups_)
{
    m_groups   = groups_;
    return 0;
}

inline int
StdOutLogger::
close (void)
{
    return 0;
}

inline void
StdOutLogger::
resync ()
{
	COUT << FLUSH;
}

/** @} */

END_SUNRISE_NAMESPACE

#endif /* STDERR_LOGGER_H */
