//------------------------------------------------------------------------------
//                          TimeVal.h
//------------------------------------------------------------------------------
#ifndef TIME_VAL_H
#define TIME_VAL_H

#include <sys/time.h>		// gettimeofday(3)
#include <unistd.h>		// gettimeofday(3)

#include <string> 

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif

/** \example timeval_test.cpp
 * \brief TimeVal类测试程序
 */

using std::string;

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr 基础工具库(libsr)
 * \{*/
/** \file TimeVal.h
 *  \brief UNIX系统timeval结构包裹类
 */
/** \class TimeVal
 *  \brief UNIX系统的timeval结构包裹类
 */
/** \}*/
class TimeVal : public timeval
{
public:
	enum { 
		gmt,					/**< 格林威治时区 */
		loc						/**< 本地时区 */
	};

	/** \brief 缺省构造函数
	 sec被设置为0, usecs被设置为0.
	 要取得当前时间，这样调用 - TimeVal(gettimeofday());
	 */
	TimeVal ();

	/** \brief 以seconds/milliseconds对构造TimeVal实例.
	 *  \param sec_ 秒钟数
	 *  \param msec_ 微秒数
	 */
	TimeVal (long sec_, long msec_);

	/** \brief 构造函数
	 *  \param d_ 整数部分作为秒钟数,小数点后6尾作为微秒数初始化
	 */
	TimeVal (double d_);

	/** \brief 以<TT> struct timeval</TT>结构体构造TimeVal实例.
	 *  \param tv_ 用一个timeval来初始化
	 */
	TimeVal (const timeval& tv_);

	/** \brief 拷贝构造函数
	 */
	TimeVal (const TimeVal& tv_); 

	/** \brief 类型转换操作符
	 */
	operator double () const;

	/** \brief 设置timeval结构体sec
	 */
	void sec (long sec_);

	/** \brief 获取timeval结构体sec
	 */
	long sec (void) const;

	/** \brief 设置timeval结构体的milliseconds.
	 */
	void msec (long msec_);

	/** \brief 获取timeval结构体的milliseconds.
	 */
	long msec (void) const;

	/** \brief 转换tv_usec (microseconds) 为 milliseconds (1 / 1,000秒)
	 */
	long millisec () const;

	/** \brief 设置时区
	 */
	void tz (int tz_);

	/** \brief 获取时区
	 */
	int tz (void) const;

	TimeVal& operator= (const TimeVal& tv_);

	/// Addition.
	TimeVal& operator+= (const TimeVal& rhs_);

	/// Substraction.
	TimeVal& operator-= (const TimeVal& rhs_);

	/// Addition.
	friend TimeVal operator+(const TimeVal& lhs_, const TimeVal& rhs_);

	/// Substraction.
	friend TimeVal operator-(const TimeVal& lhs_, const TimeVal& rhs_);

	/// Comparison.
	bool operator< (const TimeVal& rhs_) const;

	/// Equality.
	bool operator==(const TimeVal& rhs_) const;

	/// Comparison.
	friend bool operator> (const TimeVal& lhs_, const TimeVal& rhs_);

	/// Comparison.
	friend bool operator!=(const TimeVal& lhs_, const TimeVal& rhs_);

	/// Comparison.
	friend bool operator<=(const TimeVal& lhs_, const TimeVal& rhs_);

	/// Comparison.
	friend bool operator>=(const TimeVal& lhs_, const TimeVal& rhs_);

	/** \brief 格式化timeval结构体, 缺省格式为MM/DD/YYYY HH:MM:SS.MMM
	    如果要其他的格式，可参看man 3 strftime. 通过参数fmt_传递
		 格式字符串
	    \param fmt_ strftime(3)定义的格式字符串
	    \return 返回日期字符串.
	*/
	string fmtString (const char* fmt_ = NULL) const;

	/** \brief 格式化timeval结构体为HH:MM:SS
	 */
	string fmt_hh_mm_ss () const;

	/** \brief 格式化timeval结构体为HH:MM:SS.MLS
	 */
	string fmt_hh_mm_ss_mls () const;

	/** \brief 格式化timeval结构体为MM:SS
	 */
	string fmt_mm_ss () const;

	/** \brief 格式化timeval结构体为MM:SS.MLS
	 */
	string fmt_mm_ss_mls () const;

	/** \brief 格式化timeval结构体为SS.MLS
	 */
	string fmt_ss_mls () const;

	/** \brief 把timeval结构写入日志文件
	*/
	void dump_to_log (const string& name_ = "") const;

	/** Static that returns zero timeval: {0,0}
	 */
	static TimeVal zeroTime ();

	/** \brief 取得当前时间(调用系统调用gettimeofday())
	    \return time of the day as timeval
	*/
	static TimeVal gettimeofday ();

protected:
	/// Internal initialization common to most constructors.
	void init (long, long, int);

//private:
public:
	/// Normalization after arithmetic operation.
	void normalize ();

private:
	/// 时区
	int m_tz;

	/// Zero time value
	static TimeVal m_zero;
};
//TimeVal operator+(const TimeVal& lhs_, const TimeVal& rhs_);

END_SUNRISE_NAMESPACE

#endif /* TIME_VAL_H */  
