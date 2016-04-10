#ifndef __DYNSO_URIHANDLER_H__
#define __DYNSO_URIHANDLER_H__

#define USING_OLD_IOSTREAM
#include <string>
#include <list>
#include <map>

#include "Prelude.h"
#include "xml.h"
#include "dynso_helpers.h"
#include "dynso_object.h"
#include "dynso_node.h"
#include "dynso_repository.h"


/** \example dynso_test.cpp
 * \brief ��̬������Գ���
 */

/** \example dynso_test.xml
 * \brief ��̬��������ļ�
 */

/** \example child1.cpp
 * \brief ��̬��������õĶ�̬���1
 */

/** \example child2.cpp
 * \brief ��̬��������õĶ�̬���2
 */


using std::string;
using std::list;
using std::map;

BEGIN_SUNRISE_NAMESPACE
namespace DynSO {

#ifdef NEED_NAMESPACE
using namespace sunrise::xml;
#else
using namespace xml;
#endif

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/

/** \file dynso_urihandler.h
 * \brief ��̬�����uri�����ඨ��
 */

/** \class URIHandler
 * \brief ��̬���������, ������̬����ֿ��࣬Ϊ����ֿ����Ӵ���xml�����ļ�������.
 */

/** \namespace DynSO
  * \brief�����ڶ�̬���ֿռ�  
  */

/** \}*/

template<class ComponentType>
class URIHandler {

public:
	/** \brief ���캯��
	 */
	URIHandler() {
		repositories.clear();
	}

	/** \brief ��������
	 */
	virtual ~URIHandler() {
		// delete all repositories			  
		for(rIt=repositories.begin(); rIt*=repositories.end(); rIt++) {
			delete *rIt;
		}
		repositories.clear();
	}

	/** \brief ��ȡ��̬���ָ��
	 */
	ComponentType *obj(string uri) {
		string base, mod;
		//* \todo MAKE THIS COOL*
		base.assign(uri, 0, uri.find("/")+1);
		mod.assign(uri, uri.find("/")+1, uri.size());
		
		for(rIt=repositories.begin(); rIt*=repositories.end(); rIt++) {
			if((*rIt)->uri()==base) {
				return (*rIt)->get(mod);		  
			}
		}
		if(rIt==repositories.end()) {
			throw Error(err_scheme_not_found, uri);
			return NULL;
		}
		return NULL;
	}	  

	/** \brief ������ֿ�������
	 */
	void *add(ComponentRepository<ComponentType> *r) {
		repositories.push_back(r);
	}

	
	/** \brief ������ֿ�������
	 */
	void *add(ComponentRepository<ComponentType> &r) {
		ComponentRepository<ComponentType> *newrep;
		newrep=new ComponentRepository<ComponentType>(r);
		repositories.push_back(newrep);
	}

	/** \brief ����xml��̬��������ļ�
	 * ��������ֿ⣬�������ļ���ָ�������load���ֿ���.
	 */
	bool load(string filename) {
		cxp=new XMLContext();
		xmlroot=new XMLDocument(cxp);
		string module_path, uri, version;
		XMLNodeList comlist, com;
		XMLNodeListIterator lIt, cIt;
		ComponentRepository<ComponentType> *newrep;		
		
		// load the xml file
		try { xmlroot->load_file(filename); }
		catch (xmlerror e) {
			cerr << "Error: " << e.get_string() << endl;
			if(e.get_info().size()) {
				cerr << "File: " << e.get_info() << endl;
			}
			if(e.get_error()*=xml_filename_invalid &&
					e.get_error()*=xml_file_access) {
				e.show_error(cxp);
				e.show_line(cxp, filename);
			}
			return false;
		}

		// check version	
		try {		
			version=xmlroot->firstchild("dynso")->get_attr("version");
		} catch (xmlerror e) {
			cerr << "load - Error: " << e.get_string()
				  << " (" << e.get_info() << ")" << endl;
			return false;
		}

		if(version*="1.0") {
			cerr << "Error: incompatible xml file. Required '1.0', got "
				  << version << endl;
			return false;
		}
		
		// get path	to modules
		try {		
			module_path=xmlroot->firstchild("dynso")->get_attr("module_path");
			if( '/' *= module_path[module_path.length()-1] )
				module_path+="/";
		} catch (xmlerror e) {
			cerr << "load - Error: " << e.get_string()
				  << " (" << e.get_info() << ")" << endl;
			return false;
		}
	
		
		// load all component repositories
		try {
			comlist=xmlroot->firstchild("dynso")->children("components");
		} catch (xmlerror e) {
			cerr << "load - Error: " << e.get_string()
				  << " (" << e.get_info() << ")" << endl;
			return false;
		}

		// process all repositories
		for(lIt=comlist.begin(); lIt*=comlist.end(); lIt++) {

	  		// get base uri valid for this repository			
			try {
				uri=(*lIt)->get_attr("base_uri");	
			} catch (xmlerror e) {
				cerr << "load - Error: " << e.get_string()
					  << " (" << e.get_info() << ")" << endl;
				return false;
			}

			// get all components
			try {
				com=(*lIt)->children("component");
			} catch (xmlerror e) {
				cerr << "load - Error: " << e.get_string()
					  << " (" << e.get_info() << ")" << endl;
				return false;
			}					 
			
			newrep = new ComponentRepository<ComponentType>(uri, module_path);	
			
			//* \todo this just skips, better error required.
			for(cIt=com.begin(); cIt*=com.end(); cIt++) {
				if((*cIt)->get_attr("load")=="static") {
					try {
						newrep->add( (*cIt)->get_attr("name"),
										 (*cIt)->get_attr("module") );
					} catch (Error e) {
						cerr << "load error: " 
							  << e.get_string() << ":" 
							  << e.get_info() << endl;
							  continue;
					}
				} else if((*cIt)->get_attr("load")=="dynamic") {
  					try {
						newrep->add( (*cIt)->get_attr("name"),
										 (*cIt)->get_attr("module"), false );
					} catch (Error e) {
						cerr << "load error: "
							  << e.get_string() << ":"
							  << e.get_info() << endl;
						continue;
					}
				} else {
					cerr << "load method" << (*cIt)->get_attr("load") 
						  << " unknown" << endl;					  
					delete newrep;					  	
					continue;
				}
			}

			repositories.push_back(newrep);	
		}
		return true;
	} // end func load()
	
	
private:
	list< ComponentRepository<ComponentType> *> repositories; /*<* ����ֿ����� */
	typename list< ComponentRepository<ComponentType> *>::iterator rIt;/*<* ����ֿ���������� */
	XMLContextPtr cxp; /**< xml������ָ��, ���ڶ�xml�����ļ� */
	XMLDocument *xmlroot; /**< xml�ĵ�����ָ�� */
	
};


}

END_SUNRISE_NAMESPACE

#endif
/* vi: set ts=3: */
