//------------------------------------------------------------------------------
//                            Logger.h
//------------------------------------------------------------------------------
#ifndef LOGGER_H
#define LOGGER_H

//System headers
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

#include <string>
#include <stack>

using std::string;
using std::stack;

#ifdef sun
#ifndef USE_SIMPLE_SINGLETON
#define USE_SIMPLE_SINGLETON 1
#endif
#endif

//sunrise headers
#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#ifdef USE_SIMPLE_SINGLETON
#include "SSingleton.h"
#else
#include "Singleton.h"
#endif
#include "Logger_Impl.h"
#include "MemDump.h"

BEGIN_SUNRISE_NAMESPACE

#define LOCAL_TIME 0 
#define GMT_TIME 1 

/** @addtogroup libsr 
@{
*/


/*! \example logger_test.cpp
 *  Logger类的测试程序
 */

/*! \file Logger.h 
 * \brief 日志信息接口类
 */
/******************************************************************************
  Class Logger
******************************************************************************/ 
/*! \class Logger
 * \brief 日志类
 */
class Logger
{
public:
    Logger () : m_impl (NULL) { /*empty*/ }
    ~Logger () { this->close (); }

public:
	/*! \brief 打开日志枚举组中某日志项的日志开关.
	 * Group是一个枚举, 定义各种级别的日志信息. @see Group
	 *  @param g_ 组实例.
	 */
    void enable_group  (Group g_);

	/*! \brief 关闭日志枚举组中某日志项的日志开关.
	 * Group是一个枚举, 定义各种级别的日志信息. @see Group
	 *	@param g_ 组实例
	 */
    void disable_group (Group g_);

	/*! \brief 打开日志枚举组中多个日志项的日志开关.
	 *  @param groups_ 多个日志项的按位与结果
	 */
    void enable_groups  (u_long groups_);

	/*! \brief 关闭日志枚举组中多个日志项的日志开关.
	 *  @param groups_ 多个日志项的按位与结果
	 */
    void disable_groups (u_long groups_);

	 /*! \brief 打开所有的日志项
	  */
    void enable_all_groups  (void);

	 /*! \brief 关闭所有的日志项
	  */
    void disable_all_groups (void);

	 /*! \brief 检查某日志项是否被打开
	  * @return 如果是打开，返回true。否则返回false。
	  */
    bool group_enabled (Group g_) const;

	 /*! \brief 在日志信息中加入时间戳
	  */
    void enable_timestamp (void);

	 /*! \brief 关闭时间戳
	  */
    void disable_timestamp (void);

	 /*! \brief 检查日志信息中是否加入了时间戳
	  * @return 如果是，返回true。否则返回false。
	  */
    bool timestamp_enabled (void) const;

	 /*! \brief 设置时区
	  * 0 - GMT(格林威治), 1 - LOCAL(本地)
	  */
    void set_timezone (int zone);

	 /*! \brief 登记函数名的开始
	  *
	  */
    void sign_on (const string& func_name_);

	 /*! \brief 登记函数名的结束
	  */
    void sign_off (void);

    /*! \brief 写日志信息到标准输出
	  *  @return  成功返回0, 否则返回-1或者全局errno值.
	  */
    int  open (u_long groups_);

    /* \brief 写日志信息到文件.
	 *  @return 成功返回0, 否则返回-1或者全局的errno值.
	 */
    int  open (const char* logfname_,
	       u_long groups_, u_long maxsize_);

    /* \brief 写日志信息到文件.
	 *  @return 成功返回0, 否则返回-1或者全局的errno值.
	 */
    int  open (const char* base_dir_, int ccm_id_, u_long groups_, u_long maxsize_, char seq_delimer_, int switchday);

#if 0
    /*! \brief 写日志信息到日志服务器.
	  *  @return 成功返回0, 否则返回-1或者全局的errno值.
	  */
    int  open (const char* logfname_, const INETAddress& logsvr_,
    			   u_long groups_);
#endif

	 /*! \brief 创建新进程后重新同步日志流.
	  */
    void resync (void);

	 /*! \brief 关闭日志流
	  */
    int  close (void);

	 /*! \brief 写日志
	  */
    int log_msg  (u_long g_, const char* fmt_, ...);

	int log_func (u_long g_, marker_t type_);


private:
    Logger_Impl*  m_impl;	      /*!<  日志记录器的实现类实例。 */
    stack<string> m_context;     /*!< 保留所有上下文的堆栈。 */
};

/*! \def LOGGER
 *  Logger单件实例取用宏
 */

#ifndef USE_SIMPLE_SINGLETON
#ifdef NEED_NAMESPACE
typedef sunrise::SingletonHolder<Logger, CreateUsingNew> SingleLogger;
#define LOGGER sunrise::SingleLogger::Instance()
#else
typedef SingletonHolder<Logger, CreateUsingNew> SingleLogger;
#define LOGGER SingleLogger::Instance()
#endif
#else
typedef SingletonHolder<Logger> SingleLogger;
#define LOGGER SingleLogger::Instance()
#endif

/*******************************************************************************
  Inline functions
*******************************************************************************/

inline void 
Logger::
enable_group (Group g_) 
{ 
    if (m_impl) {
		m_impl->enable_group (g_);
    }
}

inline void 
Logger::
disable_group (Group g_)
{
    if (m_impl) {
		m_impl->disable_group (g_);
    }
}

inline void 
Logger::
enable_groups (u_long g_) 
{ 
    if (m_impl) {
		m_impl->enable_groups (g_);
    }
}

inline void 
Logger::
disable_groups (u_long g_)
{
    if (m_impl) {
		m_impl->disable_groups (g_);
    }
}

inline bool 
Logger::
group_enabled (Group g_) const
{
    return (m_impl) ? m_impl->group_enabled (g_) : false;
}

inline void 
Logger::
enable_all_groups  (void)
{
    if (m_impl) {
		m_impl->enable_all_groups ();
    }
}

inline void 
Logger::
disable_all_groups (void)
{
    if (m_impl) {
		m_impl->disable_all_groups ();
    }
}

inline void 
Logger::
enable_timestamp (void)
{
    if (m_impl) {
		m_impl->enable_timestamp ();
    }
}

inline void 
Logger::
disable_timestamp (void)
{
    if (m_impl) {
		m_impl->disable_timestamp ();
    }
}

inline bool 
Logger::
timestamp_enabled (void) const
{
    return (m_impl) ? m_impl->timestamp_enabled () : false;
}

inline void 
Logger::
set_timezone (int zone_)
{
    if (m_impl) {
		m_impl->set_timezone (zone_);
    }
}

inline void 
Logger::
resync (void) 
{
    if (m_impl) {
		m_impl->resync ();
    }
}


inline void 
Logger::
sign_on (const string& func_name_)
{
    m_context.push (func_name_);
}

inline void 
Logger::
sign_off (void)
{
    if (!m_context.empty ()) {
		m_context.pop ();
    }
}

/*******************************************************************************
  Macro definition shortcuts
*******************************************************************************/
/** @def DL(X)
	写debug信息到日志记录器的宏.
*/
#if defined (SUNRISE_NLOGGING)
#  define DL(X) do {} while (0)
#else
#  define DL(X) \
     do { \
       LOGGER.log_msg X; \
     } while (0)
#endif

/** @def EL(X)
	写error信息到日志记录器的宏.
*/
#if defined (SUNRISE_NLOGGING)
#  define EL(X) do {} while (0)
#else
#  define EL(X) \
     do { \
       LOGGER.log_msg X; \
       LOGGER.log_msg(ERROR,"errno: %d \"%s\"\n", errno, strerror(errno)); \
     } while (0)
#endif

/*******************************************************************************
  Namespace Log provides only the most plausable interface.
  Other public functions can be reached via LOGGER
*******************************************************************************/

/*! \namespace Log
 *  \brief Log namespace
 */

namespace Log {					// @start namespace Log

    /**
	 * 打开log文件.
	 *
	 * @param logfname_ 日志文件名
	 * @param groups_ LogMask.h文件里定义的日志项，缺省用上所有的日志项.
	 * @param maxsize_ 日志文件大小的最大值，缺省是10M。达到了这个最大限制
	 * 						后，日志文件名被改为logfname_0.
	 * @return 成功返回0, 否则返回-1.
	 */
    inline int open_log_file (const char* logfname_, 
				  u_long groups_ = ALL, 
				  u_long maxsize_ = 10 * 1024 * 1024) 
    {
		return LOGGER.open (logfname_, groups_, maxsize_);
    }

    /**
	 * 打开BOSS项目专用log文件.
	 *
	 * @param ccm_id_ CCM_ID
	 * @param base_dir_ 日志文件存放的地方。
	 * @param groups_ LogMask.h文件里定义的日志项，缺省用上所有的日志项.
	 * @param maxsize_ 日志文件大小的最大值，缺省是10M。达到了这个最大限制
	 * 						后，日志文件名从原来的logfname_<序号>被改为
	 * 						logfname_<序号+1>.
	 * @param seq_delimer_ 日志文件名序号前的分隔符，默认"_"
	 * @param switchday 日志文件名切换天数，默认5天切换一次
	 * @return 成功返回0, 否则返回-1.
	 */
	inline int open_log_file ( int ccm_id_, 
								const char* base_dir_,
								u_long groups_ = ALL,
								u_long maxsize_ = 10 * 1024 * 1024,
								char seq_delimer_ = '_',
								int switchday = 5 ) 
	{
		return LOGGER.open ( base_dir_, ccm_id_, groups_, maxsize_, seq_delimer_, switchday);
	}

    /** 写日志信息到标准输出.
	  */
    inline int open_log_stdout (u_long groups_ = ALL)
    {
		return LOGGER.open (groups_);
    }

#if 0
    /** 写日志信息到日志服务器.
	  */
    inline int open_log_logserver (const char* logfname_, 
                      const INETAddress& logsvr_,
     		   u_long groups_)
     {
     	return LOGGER.open (logfname_, logsvr_, groups_);
     }
#endif

	 /** 创建新进程后重新同步日志流.
	  */
    inline void resync (void) {	LOGGER.resync (); }
    
	 /** 关闭日志流
	  */
    inline int close (void) { return LOGGER.close (); }

	 /** 设定时区
	  */
    inline void set_gmt_timezone (void) { LOGGER.set_timezone (0); }

	/** 加入时间戳
	 */
    inline void enable_timestamp (void) { LOGGER.enable_timestamp (); }

	/** 去掉时间戳
	 */
    inline void disable_timestamp (void) { LOGGER.disable_timestamp (); }	

} // @end namespace Log

//------------------------------------------------------------------------------
// 诊断上下文.
//------------------------------------------------------------------------------
/** 跟踪函数调用堆栈的诊断上下文类。
 */
class DiagnosticContext
{
public:
	/** 构造函数
	 */
	DiagnosticContext (const char* fn_, u_long mask_ = TRACE);
	/** 析构函数
	 */
	~DiagnosticContext ();

private:
	/** 复制构造函数
	 */
	DiagnosticContext (const DiagnosticContext&);
	/** 赋值构造函数
	 */
	DiagnosticContext& operator= (const DiagnosticContext&);

private:
	const char*  m_fname; //<! 日志文件名
	u_long       m_mask; //<! 日志项组合
};

inline
DiagnosticContext::
DiagnosticContext (const char* fn_, u_long mask_)
	: m_fname (fn_), m_mask (mask_)
{
	LOGGER.sign_on (m_fname);
	LOGGER.log_func (m_mask, FUNC_ENTRY);
}

inline 
DiagnosticContext::
~DiagnosticContext ()
{
	LOGGER.log_func (m_mask, FUNC_EXIT);
	LOGGER.sign_off ();
}

/** @def trace(s)

    trace()用于跟踪函数调用栈.

    trace()登记格式如:[function_name] --v-- 的级别为TRACE的日志信息。
    当到达函数的退出处，记录格式如：[function_name] --^--的日志信息
    
    应用程序通过命令行{'-m','--mask'}提供不同的debug掩码可以方便地打开
	 或者关闭记录各种TRACE日志信息.
*/
#if defined (SUNRISE_NTRACE)
//#define trace(s) do {} while(0)
//#define TR(s) do {} while(0)
#define trace(s)
#define TR(s)
#else
#ifdef NEED_NAMESPACE
#define trace(s) sunrise::DiagnosticContext tRaCeR(s);
#define TR(s) sunrise::DiagnosticContext tRaCeR(s);
#else
#define trace(s) DiagnosticContext tRaCeR(s);
#define TR(s) DiagnosticContext tRaCeR(s);
#endif
#endif

/** @def trace_with_mask(s, m)

    trace_with_mask()用于跟踪函数调用栈. 参数m提供了日志掩码。
*/
#ifdef NEED_NAMESPACE
#define trace_with_mask(s, m) sunrise::DiagnosticContext tRaCeR(s, m);
#else
#define trace_with_mask(s, m) DiagnosticContext tRaCeR(s, m);
#endif
/** @} */
END_SUNRISE_NAMESPACE
#endif /* LOGGER_H */
