#ifndef __DYNSO_NODE_H__
#define __DYNSO_NODE_H__

#include <string>
#include <list>
#include <map>

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif
#include "dynso_helpers.h"
#include "dynso_object.h"

using namespace std;

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/

/** \file dynso_helpers.h
 * \brief ComponentNode类头文件

/** \class ComponentNode
 * \brief 动态组件节点类,对map<first,second>的继承和扩充
 */

/** \namespace DynSO
  * \brief包括于动态名字空间  
  */
/** \}*/


BEGIN_SUNRISE_NAMESPACE

namespace DynSO {
template<class ComponentType>
class ComponentNode : public map< string, Object<ComponentType>* > {
	public:
		typedef ComponentNode self;
		typedef map< string, Object<ComponentType>* > parent;

                 /** \def CN_ITERATOR
                 * 动态组件(object类)关联表迭代子定义 */
		typedef typename map< string, Object<ComponentType>* >::iterator CN_ITERATOR;

public:
	/** \brief 缺省构造函数
	 */
	ComponentNode() {};

	/** \brief 根据关键字查找符合关键字的组件, 返回指向这个组件的迭代子
          *  \param key 动态组件关联表节点的关键字
	 */
	//ComponentNode::iterator find_first(string key) 
	CN_ITERATOR find_first(string key) {
		for(it=begin(); it*=end(); it++) {
			if(it->first==key) break;
		}
		return it;
	}

	/** \brief 根据关键字查找符合关键字的组件并返回.
          *  \param key 动态组件关联表节点的关键字
	 */
	Object<ComponentType> *get(string n) {
		it=find_first(n);
		if(it==end()) {
			throw Error(err_object_not_found, n);	
			return NULL;				  
		} else {
			return it->second;
		}
		
	}

	/** \brief 增加一个动态组件
	 * \param n 动态组件的URI.
         * \exception DynSO::Error
         * 如果组件存在则抛出
	 */
	void add(string n, Object<ComponentType> *t) {
		it=find_first(n);
		if (it==end()) { // only insert if name isn't already in the mapping
			ComponentNode::value_type cn(n,t);
			insert(cn);
		} else {
			throw Error(err_duplicate_object, n);
		}	  
	}
	
	/** \brief 移除某动态组件
         * 找不到组件则抛出
	 */
	void remove(const string &n) {
		it=find_first(n);
		if (it*=end()) erase(n);
		else throw Error(err_object_not_found, n);	
	}
	
private:
	//ComponentNode::iterator it;
	/** \brief 组件节点迭代子.
	 */
	CN_ITERATOR it;
	
};

};

#endif
/* vi: set ts=3: */
