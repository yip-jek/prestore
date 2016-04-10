
#ifndef CMD_LINE_OPTS_H
#define CMD_LINE_OPTS_H

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif
#include "Assert.h"

#include <string> 
#include <vector>
using std::string;
using std::vector;

/** \example cmdlineopt_test.cpp
 * \brief 命令行参数测试程序
 */

/** \defgroup libsr  基础功能模块(libsr) 
 */
/** \{*/

/** \file CmdLineOpts.h
 *       \brief 命令行选项处理类
 *       解释命令行选项
 */


/** \class Option CmdLineOpts.h "include\CmdLineOpts.h"
 * \brief 命令行选项处理类的协助类(Helper).
 * 这个类只给命令行选项类使用，不能被实例化。
 */
/** \}*/

BEGIN_SUNRISE_NAMESPACE

class CmdLineOpts; 


class Option {
public:
	friend class CmdLineOpts; 

	/** \enum type_t
	 *  \brief选项类型枚举
	 */
	enum type_t { 
		string_t=0,	/**< STL string */ 
		int_t,		/**< int  */
		uint_t,		/**< unsigned int  */
		long_t,		/**< long  */
		ulong_t,	/**< unsinged long  */
		double_t,	/**< double  */
		float_t,	/**< float  */
		flag_t,		/**< No argument; bool value is flipped.  */
		func_t,		/**< function  */
		func_one_t,	/**< function with one argument  */
		none_t					
	};

private:
	/** \brief 私有缺省构造函数
	 */
	Option ();

	/** \brief 缺省构造函数
	 */
	Option (char shopt_, const string& lopt_, type_t type_, void* val_);

	/** \brief dump实例状态到日志文件
	 */
	void dump () const;

private:
	/** \brief 单字母选项名.
	 */
	char    m_short_name;

	/** \brief 长选项名
	 */
	string  m_long_name;

	/** \brief 选项类型
	 */
	type_t  m_type;

	/** \brief 选项值
	 */
	void*   m_val;
};

inline 
Option::Option () :
	m_short_name (' '), m_long_name (""), m_type (none_t), m_val (NULL) 
{
	/* empty */
}

inline
Option::Option (char shopt_, const string& lopt_, type_t type_, void* val_) : 
	m_short_name (shopt_), 	m_long_name (lopt_),
	m_type (type_), m_val (val_) 
{
	trace_with_mask("Option::Option", CMDLINEOPTS);
}

/** \class CmdLineOpts CmdLineOpts.h
 * \brief 解释命令行选项。
 * 这是一个基类，要使用它，必需写一个继承于这个类的子类. 可看例子程序。
*/
class CmdLineOpts
{
public:
	typedef void (* OPTS_FUNC) (void);
	typedef void (* OPTS_FUNC_ONE) (const string&);

	typedef vector<Option> OptionSet;

	/** \brief 缺省构造函数
	 */
	CmdLineOpts ();

	/** \brief 析构函数
	 */
	virtual ~CmdLineOpts () { 
		trace_with_mask ("CmdLineOpts::~CmdLineOpts", CMDLINEOPTS); 
	}

	/** \brief 增加一个布尔选项.
	   增加一个布尔选项.
	    @param c  选项的短名
	    @param s  选项的长名
	    @param f  布尔选项变量的指针
	    @return 成功返回true, 否则返回false.
	*/
	bool add_flag_opt (const char c, const string& s, bool* f);

	/** \brief 增加一个STL string选项
	  增加一个STL string选项
	    @param c  选项的短名
	    @param s  选项的长名
	    @param str  string选项变量指针
	    @return 成功返回true, 否则返回false.
	*/
	bool add_opt (const char c, const string& s, string* str);

	/** \brief 增加一个整形选项
	   增加一个整形选项
	    @param c  选项的短名
	    @param s  选项的长名
	    @param i  整形选项变量指针
	    @return 成功返回true, 否则返回false.
	*/
	bool add_opt (const char c, const string& s, int* i);

	/** \brief 增加一个unsigned integer选项
   	 增加一个unsigned integer选项
	    @param c  选项的短名
	    @param s  选项的长名
	    @param ui  u_int选项变量指针
	    @return 成功返回true, 否则返回false.
	*/
	bool add_opt (const char c, const string& s, unsigned int* ui);

	/** \brief 增加一个长整形选项
	    增加一个长整形选项
	    @param c  选项的短名
	    @param s  选项的长名
	    @param l  长整形选项变量指针
	    @return 成功返回true, 否则返回false.
	*/
	bool add_opt (const char c, const string& s, long* l);

	/** \brief 增加一个unsigned long选项.
	   增加一个unsigned long选项.
	    @param c  选项的短名
	    @param s  选项的长名
	    @param ul  unsigned long选项变量指针.
	    @return 成功返回true, 否则返回false.
	*/
	bool add_opt (const char c, const string& s, unsigned long* ul);

	/*1 \brief 增加一个double选项.
	    增加一个double选项.
	    @param c  选项的短名
	    @param s  选项的长名
	    @param d  double选项变量指针.
	    @return 成功返回true, 否则返回false.
	*/
	bool add_opt (const char c, const string& s, double* d);

	/** \brief 增加一个float选项.
	    增加一个float选项.
	    @param c  选项的短名
	    @param s  选项的长名
	    @param f  float选项变量指针
	    @return 成功返回true, 否则返回false.
	*/
	bool add_opt (const char c, const string& s, float* f);

	/** \brief 增加一个布尔选项.
	    增加一个布尔选项.
		 在处理命令行选项时参数f_所指向的函数将被调用.

	    @param c_  选项的短名
	    @param s_  选项的长名
	    @param f_  函数指针

	    @return 成功返回true, 否则返回false.
	*/
	bool add_opt (const char c_, const string& s_, OPTS_FUNC f_);

	/** \brief 增加一个STL string选项.
		 在处理命令行选项时参数f_所指向的函数将被调用, 选项值作为函数的参数.

	    @param c_  选项的短名
	    @param s_  选项的长名
	    @param f_  函数指针
	    @return 成功返回true, 否则返回false.
	*/
	bool add_opt (const char c_, const string& s_, OPTS_FUNC_ONE f_);

	/** \brief 移除选项
	    移除选项
	    @param c_  选项的短名
	    @param s_  选项的长名
	    @return 成功返回true, 否则返回false.
	*/
	bool rm_opt (const char c_, const string& s_);

	/** \brief 分析命令行参数
	    分析命令行参数
	    @return 成功返回true, 否则返回false.
	*/
	bool parse_args (const char* argv[]);

	/** \brief 取错误信息(如果调用parse_args失败)
	   取错误信息(如果调用parse_args失败)
	*/
	const char* get_opt_error () const;

	/** \brief 字符串转换成argv数组
	   把命令行字符串转换为动态分配的类似argv数组格式的变量argv_中.
	 	 数组以NULL结尾. 内存必需要在使用完后释放.
	    @param src_  命令行字符串
	    @param argc_ 在命令行字符串中找到的选项个数
	    @param argv_ 从堆中分配的内存, argv数组.
	*/
	static void str_to_argv (const string& src_, int&  argc_, char**& argv_);

	/** \brief 释放调用<TT>str_to_argv()</TT>所返回的内存.	 
	 */
	static void free_argv (char**& argv_);

	/** \brief 把选项信息写到日志文件里.
	 */
	void dump () const;

protected:
	/** \brief 检查选项合法性.
	 */
	bool is_valid (const char sopt_, const string& lopt_);
	
	/** \brief 重置错误信息
	 */
	void set_error_none ();

	/** Perform value assignment to the node. Data conversion  happens here. */
	bool assign (Option* node_, const char* op_[]);

	/** \brief 从选项集中定位选项
	 */
	Option* find_option (const char* str_);

	/** \brief 从选项集中定位选项
	 */
	Option* find_option (const char letter_);

	/** Process positional argument arg_. This method must be
	    overloaded by the derived class to take advantage of it.
	    @param arg_ positional argument value
	*/
	virtual void pos_arg (const char* arg_);

private:
	OptionSet m_opts_set; /**< 选项集 */

	string    m_error; /**< 保留最后的错误信息 */
};


inline void 
CmdLineOpts::pos_arg (const char* /*arg_*/) { /* no-opt*/ }
	
inline
CmdLineOpts::CmdLineOpts () : m_opts_set (), m_error ("")
{
	trace_with_mask("CmdLineOpts::CmdLineOpts", CMDLINEOPTS);
	set_error_none ();
}

inline void
CmdLineOpts::set_error_none ()
{
	trace_with_mask("CmdLineOpts::set_error_none", CMDLINEOPTS);
	m_error = "";
}

inline const char*
CmdLineOpts::get_opt_error () const
{
	return (m_error.c_str ());
}


END_SUNRISE_NAMESPACE

#endif /* CMD_LINE_OPTS_H */  
