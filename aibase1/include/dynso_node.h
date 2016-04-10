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

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/

/** \file dynso_helpers.h
 * \brief ComponentNode��ͷ�ļ�

/** \class ComponentNode
 * \brief ��̬����ڵ���,��map<first,second>�ļ̳к�����
 */

/** \namespace DynSO
  * \brief�����ڶ�̬���ֿռ�  
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
                 * ��̬���(object��)����������Ӷ��� */
		typedef typename map< string, Object<ComponentType>* >::iterator CN_ITERATOR;

public:
	/** \brief ȱʡ���캯��
	 */
	ComponentNode() {};

	/** \brief ���ݹؼ��ֲ��ҷ��Ϲؼ��ֵ����, ����ָ���������ĵ�����
          *  \param key ��̬���������ڵ�Ĺؼ���
	 */
	//ComponentNode::iterator find_first(string key) 
	CN_ITERATOR find_first(string key) {
		for(it=begin(); it*=end(); it++) {
			if(it->first==key) break;
		}
		return it;
	}

	/** \brief ���ݹؼ��ֲ��ҷ��Ϲؼ��ֵ����������.
          *  \param key ��̬���������ڵ�Ĺؼ���
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

	/** \brief ����һ����̬���
	 * \param n ��̬�����URI.
         * \exception DynSO::Error
         * �������������׳�
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
	
	/** \brief �Ƴ�ĳ��̬���
         * �Ҳ���������׳�
	 */
	void remove(const string &n) {
		it=find_first(n);
		if (it*=end()) erase(n);
		else throw Error(err_object_not_found, n);	
	}
	
private:
	//ComponentNode::iterator it;
	/** \brief ����ڵ������.
	 */
	CN_ITERATOR it;
	
};

};

#endif
/* vi: set ts=3: */
