#ifndef __DYNSO_HELPERS_H__
#define __DYNSO_HELPERS_H__

#include <string>
#include <typeinfo>

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/

/*! \file dynso_helpers.h
 * \brief ��̬��Э����Ķ���
 * -errorcodeö�ٶ���
 * -BaseComponent�ඨ�� 
 * -Error�ඨ�� 
 */

/** \}*/

using namespace std;

BEGIN_SUNRISE_NAMESPACE
namespace DynSO {


#define ERRORCODE(x,y)  case x: ret = y; break;
/* \enum errorcode
 * \brief �����ö�ٶ���
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
 * \brief ��̬����ӿ���, ������.
 * ������Ҫ����.so�����Ա�Ӧ�ó���������ʱ���ص��඼����̳в�ʵ������ӿ�
 */
class BaseComponent {
public:
	virtual void DestructObject()=0;
};

/*! \class Error
 * \brief �쳣��
 */
class Error {
public:
	
        /*! \brief ���캯��.
            @param c  �����
            @param i  ������Ϣ
        */
	Error(errorcode c, string i="") { code=c; info=i; };

        /** \brief ��ȡ����� */
	errorcode get_error() const { return code; };

        /** \brief ��ȡ������Ϣ, */ 
	const string get_info() const { return info; };
		  
        /** \brief ��ȡ����Ŷ�Ӧ�Ĵ�����Ϣ */
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
        /** \brief �����*/
	errorcode code;
        /** \brief ������Ϣ*/
	string info;
};

}

END_SUNRISE_NAMESPACE

#endif
/* vi: set ts=3: */



