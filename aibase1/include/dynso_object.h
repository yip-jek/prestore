
#ifndef __DYNSO_OBJECT_H__
#define __DYNSO_OBJECT_H__

#include <string>
#include <dlfcn.h>

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif

#include "dynso_helpers.h"

using namespace std;

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/

/** \file dynso_object.h
 * \brief 加载和管理动态组件的核心类
 */

/** \class Object
 * \brief 可被加载和卸载的动态对象包裹类,typename 对象必须继承BaseComponent
 */

/** \}*/

BEGIN_SUNRISE_NAMESPACE
namespace DynSO {

/** \def 工厂方法的方法名
 */
#define DYNCOM_CREATE_METHOD "CreateObject"		  
		
//* core object representation and handling of loading/unloading modules		  

template<class ComponentType>
class Object {

public:		  
	/** \brief 构造函数
	 */
	Object() {
		component_handle=NULL;
		component_instance=NULL;
		loaded=false;
		instated=false;
	}

	/** \brief 析构函数
	 * 删除动态对象并释放这个模组
	 */
	virtual ~Object() {
		if(isinstated()) component_instance->DestructObject();
		dlclose(component_handle);		
	}

	/** 加载.so文件，创建动态组件
	 * \param file .so文件名
	 * \param create_instance 如果为true，搜索.so文件中的工作方法，否则在使用的时候获取.
	 */
	ComponentType *create(string file, bool create_instance=true) {
		const char* error=NULL;

		//component_handle=dlopen(file.c_str(), RTLD_LAZY);
		/*
		setenv("LIBPATH","/usr/local/SRBC/lib", 1);
		if(errno *= 0)
			perror("setenv fail");
		*/
		component_handle=dlopen(file.c_str(), RTLD_NOW);
		//component_handle=dlopen(file.c_str(), RTLD_GLOBAL);
		if (component_handle==NULL) {
			perror("open fail");
			error = dlerror();
			printf("%s\n", error);
			if (error!=NULL)
				throw Error(err_component_open, file);
			return NULL;
		}
		_file_=file;
		loaded=true;	
		
		if(create_instance) instate();
		return(component_instance);
	}

	/** 调用.so里面的工厂方法创建动态组件
	 * \todo 增加类型检测
	 * \exception DynSO::Error 
	 */
	ComponentType *instate() {
		if(!isloaded() || isinstated()) {
			throw Error(err_component_initialize);
			return NULL;
		}
		
		const char* error=NULL;
		ComponentType *(*component_create)(void)=NULL;
		component_create=(ComponentType *(*)(void))dlsym(component_handle, 
							 											 DYNCOM_CREATE_METHOD);
		if (component_create==NULL) {
			error = dlerror();
			if (error!=NULL)
				throw Error(err_component_incompatible, _file_);
			return NULL;
		}

		component_instance=(*component_create)();
		if(component_instance==NULL) {
			throw Error(err_component_initialize, _file_);
			return NULL;
		}
		instated=true;	
		return(component_instance);
	}

	/** \brief 返回动态组件
	 */
	ComponentType *get() {
		if(*isinstated()) instate();
		return(component_instance);
	}

	/** \brief 返回动态组件
	 */
	ComponentType *get_raw() {
		return(component_instance);
	}

	/** 获取动态组件的状态(是否已经加载到内存)
	 */
	bool isloaded() { return loaded; }; 

	/** 获取动态组件的状态(是否已经可以使用)
	 */
	bool isinstated() { return instated; };

private:			  
        /** \brief 动态函数库文件地址*/
	void* component_handle;	

        /** \brief 动态函数,组件,对象(工厂方法地址)*/
	ComponentType *component_instance;

	string _file_; /**< .so文件名 */
	bool loaded; /**< 是否已经加载的标志 */
	bool instated; /**< 是否已经创建的标志 */
	
}; 


}

END_SUNRISE_NAMESPACE

#endif
/* vi: set ts=3: */
