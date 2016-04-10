
//------------------------------------------------------------------------------
//                              Logger_Impl.h
//------------------------------------------------------------------------------
#ifndef LOGGER_IMPL_H
#define LOGGER_IMPL_H

#define USING_OLD_IOSTREAM
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>



#include <string>

using std::string;
using std::ostream;


#include "Prelude.h"
#include "LogMask.h"

BEGIN_SUNRISE_NAMESPACE
/** @addtogroup libsr 
@{
*/

/*! \file Logger_Impl.h
    \brief 日志记录器接口类
*/


/// Maximum length of the formatted message
//#define LOGGER_MAXLINE 256
#define LOGGER_MAXLINE 1024

/*! \class Logger_Impl Logger_Impl.h
 * \brief 日志记录器实现类
 *
 *日志记录器的基类，为继承类提供公共函数，功能包括：\n
 * - 设置时间输出标志 \n
 * - 设置时间输出标志 \n
 * - 设置缩进格式 \n
 * - 提供打开日志、输出日志、刷新日志、并闭日志通用接口
 */
class Logger_Impl {
public:
	/*！\brief 构造函数 */
    Logger_Impl ();
	/*！\brief 析构函数 */
    virtual ~Logger_Impl () { /* empty */ }
    
	/*! \brief 功能说明：设置单个日志输出类型 
	 * @param[in]  g_特定日志掩码 
	*/
    void enable_group  (Group g_)  { m_groups |= g_;  }
    /*! \brief 功能说明：删除单个日志输出类型 
	 * @param[in]  g_特定日志掩码 
	 */
	void disable_group (Group g_)  { m_groups &= ~g_; }
   
	/*! \brief 功能说明：设置多种日志输出类型
	 * @param[in]  g_包括多个日志掩码 
	 */
    void enable_groups  (u_long g_)  { m_groups |= g_;  }
    /*! \brief 功能说明：删除多种日志输出类型 
	 * @param[in]  g_包括多个日志掩码
	 */
	void disable_groups (u_long g_)  { m_groups &= ~g_; }

    /*! \brief 功能说明：设置日志输出类型缺省值 */
    void enable_all_groups  (void) { 
#ifdef NEED_SPACENAME 
		 m_groups = sunrise::ALL;  
#else
		 m_groups = ALL;  
#endif
	 }
    /*! \brief 功能说明：设置日志输出失效 */
    void disable_all_groups (void) { m_groups = 0;    }

	/*! \brief 功能说明：增加输出日志掩码 
	 * @param[in] g_ 特殊日志掩码
	 * @return 已设置日志掩码
	 */
    bool group_enabled (Group g_) const { return (m_groups & g_); }
    
	/*! \brief 功能说明：设置系统时间输出生效标志 */
    void enable_timestamp  (void)       { m_tmflg = true;  }
     /*! 功能说明：设置系统时间输出失效标志 */
	void disable_timestamp (void)       { m_tmflg = false; }
    /*! \brief 功能说明：获取系统时间输出标志 
	 * @return 系统时间输出标志
	 */
	bool timestamp_enabled (void) const { return m_tmflg;  }
     /*! \brief 功能说明：设置时区 
	  * @param[in] zone_ 时间类型
	 */
	void set_timezone      (int zone_)  { m_tz = zone_;    }
    /*! \brief 功能说明：设置缩进长度
	* @param[in] step_ 缩进长度
	*/
    void    set_indent_step (u_short step_) { m_indent_step = step_; }
	/*! \brief 功能说明：获取缩进长度 
	 * @return 系统时间输出标志
	*/
    u_short get_indent_step (void) const    { return m_indent_step;  }

    /*! \brief 功能说明：为以标准输出作为日志输出提供统一接口*/
    virtual int open (u_long groups_);

    /*! \brief 功能说明：为打开日志文件名提供统一接口*/
    virtual int open (const char* logfname_, u_long groups_, u_long maxsize_);

	/*! \brief 功能说明：日志文件名在输出目录上自动产生,为打开日志文件提供统一接口*/
    virtual int open (const char* base_dir_, int ccm_id_, u_long groups_, u_long maxsize_, char seq_delimer_, int switchday);

#if 0
    /// Open LogServer
// 	virtual int open (const char* logfname_,
// 			  const INETAddress& logsvr_,
// 			  u_long groups_);
#endif
    /*! \brief 功能说明：为关闭日志文件提供统一接口 */
    virtual int  close  (void) = 0;
	/*! \brief 功能说明：为刷新输出缓冲区提供统一接口 */
    virtual void resync (void) { /* empty */ }
    
	/*! \brief 功能说明：为输出日志记录提供统一接口 */
    virtual int log_msg (Group g_, size_t indent_level_, 
						 const string& func_name_, 
						 const char* fmt_, va_list) = 0;

    /*! \brief 功能说明：为按缩进层次输出函数名称提供统一接口 */
    virtual int log_func (Group g_, size_t indent_level_, 
						  const string& func_name_, 
						  marker_t type_) = 0;
protected:
	
/*!  \brief 功能说明：若系统时间输出标志为true,则在输出流中输出系统时间，时间精确到毫秒 
      * @param[in] sink_ 日志记录输出流
      * @return  若系统时间输出标志为true返回23,否则为0
 	  * @pre 日志文件成功打开后
*/
    virtual u_short add_timestamp    (ostream& sink_);

/*!  \brief 功能说明：按缩进层次输出函数名称，主要用于跟踪函数的进入和退出。 
      * @param[in] sink_ 日志记录输出流
	  * @param[in] funcname_ 函数名称
	  * @param[in] indent_level_ 缩进层次
	  * @param[in] type_  字符串输出标志位，取值如下 \n           
	  * - FUNC_ENTRY 输出为'/'
      * - FUNC_EXIT  输出为'\'
	  * - FUNC_EXIT  输出为'[]'
	  * @return  返回输出子符串长度
 	  * @pre 日志文件成功打开后
*/
    virtual u_short indent_func_name (ostream& sink_, const string& funcname_,
					size_t indent_level_,
				  marker_t type_);

protected:
    /*! \brief 功能说明：日志输出缩进位数 */
    u_short m_indent_step;

   /*! \brief 功能说明：分类日志输出过滤标志 */
    u_long m_groups;

    /*! \brief 功能说明：日志文件名 */
    string m_logfname;

     /*! \brief 功能说明：在日志记录中输出系统时间标志位 */
    bool   m_tmflg;

    /*! \brief 功能说明：时区输出标志位 Timezone: 0-GMT, 1-Local  */
    int    m_tz;
};

inline
Logger_Impl::
Logger_Impl () 
    : m_indent_step (3), m_groups (0), m_tmflg (false), m_tz (0)
{
    // empty
}

inline int 
Logger_Impl::
open (u_long /* groups_ */)
{
    errno = ENOSYS;
    return -1;
}

inline int 
Logger_Impl::
open (const char* /* logfname_ */, u_long /* groups_ */, u_long /* maxsize_ */)
{
    errno = ENOSYS;
    return -1;
}

inline int 
Logger_Impl::
open (const char* /* base_dir_ */, int /* ccm_id_ */, u_long /* groups_ */, 
		u_long /* maxsize_ */, char /* seq_delimer */, int /* switchday */)
{
    errno = ENOSYS;
    return -1;
}

#if 0
// inline int 
// Logger_Impl::open (const char* logfname_, const INETAddress& logsvr_,
// 		   u_long groups_)
// {
// 	errno = ENOSYS;
// 	return -1;
// }
#endif

/*! 日志输出
@}
*/

END_SUNRISE_NAMESPACE

#endif /* LOGGER_IMPL_H */
