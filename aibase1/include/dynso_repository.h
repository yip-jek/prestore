#ifndef __DYNSO_REPOSITORY_H__
#define __DYNSO_REPOSITORY_H__

#define USING_OLD_IOSTREAM
#include <string>
#include <list>
#include <map>

#include "Prelude.h"
#include "dynso_helpers.h"
#include "dynso_object.h"
#include "dynso_node.h"

using std::string;
using std::list;
using std::map;


/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/

/** \file dynso_repository.h
 * \brief ��̬����ֿ��ඨ��
 */

/** \class ComponentRepository
 * \brief ����ֿ���
 */

/** \}*/

BEGIN_SUNRISE_NAMESPACE
namespace DynSO {

template<class ComponentType> 
class ComponentRepository {

public:
	/** \brief ���캯��
	 * @param u - �������·��,ȱʡΪ��ǰĿ¼
	 * @param p - ���URI
	 */
	ComponentRepository(string u="", string p="") {
		components.clear();
		if(p.size()) module_path(p);
		else module_path("./");
		if(u.size()) uri(u);
		else uri("null:/");	
	}

	/** \brief �������캯��
	 */
	ComponentRepository(const ComponentRepository &r) {
		 components=r.components;
	    mod_path=r.mod_path;
	    base_uri=r.base_uri;
	}

	/** \brief ��������
	 */
	virtual ~ComponentRepository() {
		// destruct all instated Object*'s
		ComponentNode<ComponentType>::iterator it;
		for(it=components.begin(); it*=components.end(); it++) {
			delete (*it).second;
		}
		components.clear(); // unneccessary
	}

	/** \brief ��ȡ���ʵ��
         * \exception DynSO::Error
         * @param n object�ڵ�ؼ���
	 */
	ComponentType *get(string n) {
		Object<ComponentType> *obj_tmp;		  
		try {			  
			obj_tmp=components.get(n);
		} catch (Error e) {
			throw Error(err_object_not_found, n);
			return NULL;				  
		}
		return obj_tmp->get();
	}

	/** \brief ������ֿ��������
	 * @param name - object�ڵ�ؼ���
	 * @param module -��̬���ļ���
	 * @param instate -�Ƿ�Ҫ��ȡ��̬�����ȱʡΪ��
         * \return ��̬���
	 */
	ComponentType *add(string name, string module, bool instate=true) {
		Object<ComponentType> *obj_tmp=new Object<ComponentType>();
		obj_tmp->create(mod_path+module, instate);
		components.add(name, obj_tmp);
		return obj_tmp->get_raw();
	}

	/*
	ComponentType *add(string name, string full_name, bool instate = true) {
		Object<ComponentType> *obj_tmp=new Object<ComponentType>();
		obj_tmp->create(full_name, instate);
		components.add(name, obj_tmp);
		return obj_tmp->get_raw();
	}
	*/

	/** \brief ��ȡ���URI
	 * \todo: URI Scheme����:
	 * 		scheme://some/path/to/object
	 *        ����://��˼���������粿,����ip����������
	 *
	 *
	 * RFC 2396 'URI Generic Syntax'
	 * 
	 * absoluteURI   = scheme ":" ( hier_part | opaque_part )
	 * hier_part     = ( net_path | abs_path ) [ "?" query ]
	 * net_path      = "//" authority [ abs_path ]
	 * abs_path      = "/"  path_segments
	 * 	
	 * scheme        = alpha *( alpha | digit | "+" | "-" | "." )
	 *	
	 * authority     = <userinfo>@<host>:<port>
 	 *	
	 * opaque_part   = uric_no_slash *uric
	 *	
	 * uric_no_slash = unreserved | escaped | ";" | "?" | ":" | "@" |
	 *                 "&" | "=" | "+" | "$" | ","      
	 */
	string uri(string u="") {
		if(u.size()) {
			if(u.at(u.size()-2)==':' && u.at(u.size()-1)=='/') {
				base_uri=u;
			} else {
				
				base_uri=u+string(":/");
			}
		} 
		return base_uri;
	}
	
        /** \brief ��ȡ��̬�ļ�·��*/
	string module_path(string p="") {
		if(p.size()) {
			mod_path=p;
		}
		return mod_path;
	}
		  
private:
	ComponentNode<ComponentType> components; /**< ��̬����� */
	string mod_path; /**< �������·�� */
	string base_uri; /**< URI�е�schemeֵ+:/*/
};

}
END_SUNRISE_NAMESPACE

#endif
/* vi: set ts=3: */
