#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <sstream>

#include <string> 
#include <vector>

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif

using std::vector;
using std::string;


/** CommonUtils.h

   A random collection of unrelated static functions.
*/

/** \defgroup libsr  基础功能模块(libsr) 
 */
/** \{*/
/** \file CommonUtils.h
 *  \brief Utils名字空间定义.*/

/** \namespace Utils
 * \brief 定义Utils名字空间.包括环境变量解释,保存当前目录，分离串等函数 */
/** \}*/

BEGIN_SUNRISE_NAMESPACE
	namespace Utils {

		/** 
			Split character string into tokens separated by the whitespace
			character (blank, tab, newline, formfeed, and carriage return).
			The vec_ vector is emptied out prior parsing string str_.
			
			@param src_  string of tokens to split
			@param vec_  vector with tokens extracted from the string str_
		*/
		/**
		 *\brief 分离长串src_为为多个string变量，保存在vector里
		 *\param src_ 被分离的长串
		 *\param vec_ 保存分离出来的string vector的引用
		*/
		void split (const char* src_, std::vector<std::string>& vec_);

		/** 
		 * Expand the passed string in_ by substituting environment
		 * variable names for their values. Environment variables must be 
		 * preceeded by dollar sign and optionally enclosed in parentheses:
		 * $ENV_NAME, or $(ENV_NAME), or ${ENV_NAME}. $HOME is equivalent
		 * to '~' or '~username'. If later is used, "username" is looked up
		 * in the password file.
		*/
		/*
		 *\brief 如果串中带有环境变量，则解释为实际内容后的串
		 *\param in_ 可能带环境变量的字符串 
		 *\return 返回取代环境变量为实际值的string
		 *支持环境变量格式有：
		 *-$ENV_NAME
		 *-$(ENV_NAME)
		 *-${ENV_NAME}
		 *-~
		 *-~username
		*/
		std::string strenv (const char* in_);

		/**
		 * \brief 获取当前工作目录的绝对路径
		 * \return 成功则返回当前工作目录；失败返回空string,并设置错误代码
		 */
		std::string get_cwd_name ();

	} // end namespace Utils
END_SUNRISE_NAMESPACE

#endif /* COMMON_UTILS_H */  
