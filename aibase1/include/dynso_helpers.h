#ifndef __DYNSO_HELPERS_H__
#define __DYNSO_HELPERS_H__

#include <string>
#include <typeinfo>

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/

/*! \file dynso_helpers.h
 * \brief 动态库协助类的定义
 * -errorcode枚举定义
 * -BaseComponent类定义 
 * -Error类定义 
 */

/** \}*/

using namespace std;

BEGIN_SUNRISE_NAMESPACE
namespace DynSO {


#define ERRORCODE(x,y)  case x: ret = y; break;
/* \enum errorcode
 * \brief 错误号枚举定义
 */
enum errorcode {
	err_unknown = 0,
	err_component_open,
	err_component_initialize,
	err_component_incompatible,
	err_component_not_open,
	err_object_not_found,
	err_duplicate_object,
	err_scheme_not_found
};

/*! \class BaseComponent
 * \brief 动态组件接口类, 纯虚类.
 * 所有需要做成.so并可以被应用程序在运行时加载的类都必需继承并实现这个接口
 */
class BaseComponent {
public:
	virtual void DestructObject()=0;
};

/*! \class Error
 * \brief 异常类
 */
class Error {
public:
	
        /*! \brief 构造函数.
            @param c  错误号
            @param i  错误信息
        */
	Error(errorcode c, string i="") { code=c; info=i; };

        /** \brief 获取错误号 */
	errorcode get_error() const { return code; };

        /** \brief 获取错误信息, */ 
	const string get_info() const { return info; };
		  
        /** \brief 获取错误号对应的错误信息 */
	const string get_string() const {
		string ret;
		switch(code) {
			ERRORCODE(err_unknown,"unspecified or unknown error");
			ERRORCODE(err_component_open,"loading component failed");
			ERRORCODE(err_component_initialize,"initializing component failed");
			ERRORCODE(err_component_incompatible,"incompatible compoment");
			ERRORCODE(err_component_not_open,"component not open");
			ERRORCODE(err_object_not_found,"object path not found");
			ERRORCODE(err_duplicate_object,"duplicate object");
			ERRORCODE(err_scheme_not_found,"scheme not found");
		}
		return ret;
	}

	
protected:
        /** \brief 错误号*/
	errorcode code;
        /** \brief 错误信息*/
	string info;
};

}

END_SUNRISE_NAMESPACE

#endif
/* vi: set ts=3: */



