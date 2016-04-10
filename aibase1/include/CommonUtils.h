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

/** \defgroup libsr  ��������ģ��(libsr) 
 */
/** \{*/
/** \file CommonUtils.h
 *  \brief Utils���ֿռ䶨��.*/

/** \namespace Utils
 * \brief ����Utils���ֿռ�.����������������,���浱ǰĿ¼�����봮�Ⱥ��� */
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
		 *\brief ���볤��src_ΪΪ���string������������vector��
		 *\param src_ ������ĳ���
		 *\param vec_ ������������string vector������
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
		 *\brief ������д��л��������������Ϊʵ�����ݺ�Ĵ�
		 *\param in_ ���ܴ������������ַ��� 
		 *\return ����ȡ����������Ϊʵ��ֵ��string
		 *֧�ֻ���������ʽ�У�
		 *-$ENV_NAME
		 *-$(ENV_NAME)
		 *-${ENV_NAME}
		 *-~
		 *-~username
		*/
		std::string strenv (const char* in_);

		/**
		 * \brief ��ȡ��ǰ����Ŀ¼�ľ���·��
		 * \return �ɹ��򷵻ص�ǰ����Ŀ¼��ʧ�ܷ��ؿ�string,�����ô������
		 */
		std::string get_cwd_name ();

	} // end namespace Utils
END_SUNRISE_NAMESPACE

#endif /* COMMON_UTILS_H */  
