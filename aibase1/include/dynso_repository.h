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


/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/

/** \file dynso_repository.h
 * \brief 动态组件仓库类定义
 */

/** \class ComponentRepository
 * \brief 组件仓库类
 */

/** \}*/

BEGIN_SUNRISE_NAMESPACE
namespace DynSO {

template<class ComponentType> 
class ComponentRepository {

public:
	/** \brief 构造函数
	 * @param u - 组件所在路径,缺省为当前目录
	 * @param p - 组件URI
	 */
	ComponentRepository(string u="", string p="") {
		components.clear();
		if(p.size()) module_path(p);
		else module_path("./");
		if(u.size()) uri(u);
		else uri("null:/");	
	}

	/** \brief 拷贝构造函数
	 */
	ComponentRepository(const ComponentRepository &r) {
		 components=r.components;
	    mod_path=r.mod_path;
	    base_uri=r.base_uri;
	}

	/** \brief 析构函数
	 */
	virtual ~ComponentRepository() {
		// destruct all instated Object*'s
		ComponentNode<ComponentType>::iterator it;
		for(it=components.begin(); it*=components.end(); it++) {
			delete (*it).second;
		}
		components.clear(); // unneccessary
	}

	/** \brief 获取组件实例
         * \exception DynSO::Error
         * @param n object节点关键字
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

	/** \brief 往组件仓库增加组件
	 * @param name - object节点关键字
	 * @param module -动态库文件名
	 * @param instate -是否要获取动态组件，缺省为是
         * \return 动态组件
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

	/** \brief 获取组件URI
	 * \todo: URI Scheme如下:
	 * 		scheme://some/path/to/object
	 *        其中://意思是这是网络部,包含ip或者主机名
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
	
        /** \brief 获取动态文件路径*/
	string module_path(string p="") {
		if(p.size()) {
			mod_path=p;
		}
		return mod_path;
	}
		  
private:
	ComponentNode<ComponentType> components; /**< 动态组件集 */
	string mod_path; /**< 组件所在路径 */
	string base_uri; /**< URI中的scheme值+:/*/
};

}
END_SUNRISE_NAMESPACE

#endif
/* vi: set ts=3: */
