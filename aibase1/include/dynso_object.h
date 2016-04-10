
#ifndef __DYNSO_OBJECT_H__
#define __DYNSO_OBJECT_H__

#include <string>
#include <dlfcn.h>

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif

#include "dynso_helpers.h"

using namespace std;

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/

/** \file dynso_object.h
 * \brief ���غ͹���̬����ĺ�����
 */

/** \class Object
 * \brief �ɱ����غ�ж�صĶ�̬���������,typename �������̳�BaseComponent
 */

/** \}*/

BEGIN_SUNRISE_NAMESPACE
namespace DynSO {

/** \def ���������ķ�����
 */
#define DYNCOM_CREATE_METHOD "CreateObject"		  
		
//* core object representation and handling of loading/unloading modules		  

template<class ComponentType>
class Object {

public:		  
	/** \brief ���캯��
	 */
	Object() {
		component_handle=NULL;
		component_instance=NULL;
		loaded=false;
		instated=false;
	}

	/** \brief ��������
	 * ɾ����̬�����ͷ����ģ��
	 */
	virtual ~Object() {
		if(isinstated()) component_instance->DestructObject();
		dlclose(component_handle);		
	}

	/** ����.so�ļ���������̬���
	 * \param file .so�ļ���
	 * \param create_instance ���Ϊtrue������.so�ļ��еĹ���������������ʹ�õ�ʱ���ȡ.
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

	/** ����.so����Ĺ�������������̬���
	 * \todo �������ͼ��
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

	/** \brief ���ض�̬���
	 */
	ComponentType *get() {
		if(*isinstated()) instate();
		return(component_instance);
	}

	/** \brief ���ض�̬���
	 */
	ComponentType *get_raw() {
		return(component_instance);
	}

	/** ��ȡ��̬�����״̬(�Ƿ��Ѿ����ص��ڴ�)
	 */
	bool isloaded() { return loaded; }; 

	/** ��ȡ��̬�����״̬(�Ƿ��Ѿ�����ʹ��)
	 */
	bool isinstated() { return instated; };

private:			  
        /** \brief ��̬�������ļ���ַ*/
	void* component_handle;	

        /** \brief ��̬����,���,����(����������ַ)*/
	ComponentType *component_instance;

	string _file_; /**< .so�ļ��� */
	bool loaded; /**< �Ƿ��Ѿ����صı�־ */
	bool instated; /**< �Ƿ��Ѿ������ı�־ */
	
}; 


}

END_SUNRISE_NAMESPACE

#endif
/* vi: set ts=3: */
