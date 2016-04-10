#ifndef __XML_H__
#define __XML_H__

#include <string>
#include <list>
#include <map>
#include <iosfwd>

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif
#include "xmlcommon.h"
#include "xmlhelpers.h"

/** \example xml_test.cpp
 *  \brief xml�ļ�д������͵������� */ 

/** \example xml_test1.cpp
 *  \brief xml�ļ�����ֲ�δ�ӡ���� */
 
/** \example xml_test2.cpp
 *  \brief */ 

/** \example xml_test3.cpp
 *  \brief */ 

BEGIN_SUNRISE_NAMESPACE

namespace xml {

using namespace std;

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \file xml.h
 *  \brief xml�����ṹ�ඨ��.*/
  
/** \def XML�����Ķ���ָ�� */
typedef class XMLContext* XMLContextPtr;

/** \class xmlerror
 * \brief xml�쳣��. ���н���xml�ļ������еĴ�����ʹ�����Ϣ. */ 
/** \}*/
class xmlerror {
public:
   /** \brief ���캯�� */
   xmlerror( xmlerrorcode code, string info="" ) { errorcode = code; errorinfo=info; }

   /** \brief ��ȡ����� */
   xmlerrorcode get_error() const { return errorcode; }

   /** \brief ��ȡ������Ϣ */
   const string get_string() const;

   /** \brief ��ȡ���ӵĴ�����Ϣ */
	const string get_info() const { return errorinfo; }

	/** \brief ��ʾ������Ϣ	 */
	void show_error(XMLContextPtr c);

	/** \brief ��ʾ����������ļ������ڵ���	 */
	void show_line(XMLContextPtr c, string filename);

protected:
   xmlerrorcode errorcode;
	string errorinfo;	
};

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \class XMLContext
 * \brief ������������
 * xml�ṹջ��������. */
/** \}*/
class XMLContext {
public:
   /** \brief ���캯�� */
   XMLContext();

   /** \brief �������� */
   virtual ~XMLContext();

   /** \brief ����entity(ʵ��)ֵ */
   string get_entity( string &entname );

   /** \brief ���ر�ǩֵ */
   string get_tagname( xmltagnamehandle handle );

   /** \brief �����ǩֵ. */
   xmltagnamehandle insert_tagname( string &tagname );

   /** \brief ��λxml��. */
   xmllocation &get_location() { return location; }

   /** \brief ��ʼ��������
	 * ��������Ҫoverload�����ڳ�ʼ�����趨initΪtrue�� */
   virtual void init_context() { init = true; }

protected:
   /** \brief �Ѿ���ʼ���ı�־ */
   bool init;

   /** \brief ��һ������ı�ǩ */
   int nexthandle;

   /** \brief ��ǩ������. */
   xmltagnamemap tagnames;

   /** \brief ʵ������� */
   xmlentitymap entities;

   /** \brief xml��������λ��Ϣ */
   xmllocation location;
};

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \def XMLNodePtr
 * xml�ڵ�ָ�붨�� */
typedef class XMLNode* XMLNodePtr;

/** \def XMLNodeList
 * xml�ڵ������� */
typedef std::list<XMLNodePtr> XMLNodeList;

/** \def XMLNodeListIterator
 * xml�ڵ���������Ӷ��� */
typedef XMLNodeList::iterator XMLNodeListIterator;

/** \def XMLNodeListConstIterator
 * xml�ڵ�����const�����Ӷ��� */
typedef XMLNodeList::const_iterator XMLNodeListConstIterator;


/** \class XMLAttributes
 * \brief ʹ��STL map��ű�ǩ���������Ժ�ֵ */
/** \}*/
class XMLAttributes: public std::map<string, string> {
friend class xmlparser;

public:
   /** \brief ���캯�� */
   XMLAttributes(){}

   /** \brief ��ȡ����ֵ */
   string get(string key);

   /** \brief �����µ�����ֵ */
   void set(string key, string value);

	/** \brief �Ƴ�����ֵ */
	void remove(const string &key);
};


/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \class XMLNode xml.h
 * \brief xml�ڵ��� */
/** \}*/
class XMLNode {
friend class xmlparser;

public:
   /** \brief ���캯��
	 * ������ʽ����	*/
   explicit XMLNode(XMLContextPtr pctx);

   /** \brief ���캯�� */
   explicit XMLNode(XMLContextPtr pctx, string nname);

   /** \brief �������� */
   ~XMLNode();

   /** \brief �������캯�� */
   XMLNode(const XMLNode &node);

   /** \brief ��ֵ������
	 * ��ֵ��xml�ڵ��������. */
   XMLNode & operator =(const XMLNode &node);

   /** \brief ��ֵ������
	 * ��ֵ��xml�ڵ�ָ�� */
   XMLNode & operator =(const XMLNodePtr n);
	
	/** \brief ��ȡ���趨�ڵ�ֵ
	 *  \param _name ����ǿմ� */
	string name(string _name="");

  	/** \brief ��ȡ���趨�ڵ����� */
	xmlnodetype type(xmlnodetype ntype=xml_nt_dummy);

	/** \brief ���ػ����趨CDATAֵ */
   const string data(string d="");

   /** \brief ���ؽڵ����Թ����� */
   XMLAttributes &get_attrmap() { return attributes; }

   /** \brief ���ظ������Ե�����ֵ */
   string get_attr(string attr) { return attributes.get(attr); }

	/** \brief �趨����ֵ */
   void add_attr(string attr, string value){ attributes.set(attr,value); } 

	/** \brief �Ƴ����� */
	void remove_attr(string attr) {attributes.remove(attr); }

	/** \brief �滻��ǰ���Թ����� */
	void replace_attr(XMLAttributes &attrmap) { attributes=attrmap; }
		
	/** \brief �����ӽڵ�
	 * \param front - �����Ϊtrue������뵽�����ǰ�棬������뵽list��ĩ��. */
	void add_child(XMLNodePtr n, bool front=false);

	/** \brief �����ӽڵ�
	 * \param front - �����Ϊtrue������뵽�����ǰ�棬������뵽list��ĩ��. */
	void add_child(XMLNode &n, bool front=false);

	/** \brief �����ӽڵ�
	 * \param front - �����Ϊtrue������뵽�����ǰ�棬������뵽list��ĩ��. */
	XMLNodePtr add_child(XMLContextPtr ctxp, string n, bool front=false);

	/** \brief �Ƴ��ӽڵ� */
	void remove_child(XMLNodePtr n);

   /** \brief ѡ��ڵ� */
	const XMLNodeList & children(const string & = string()) const;

	/** \brief ��ȡ���������ֵĽڵ㼯���еĵ�һ���ӽڵ� */
   XMLNodePtr firstchild(const string &n);

   /** \brief ������������xml�ڵ� */
   void load(std::istream &instream, XMLContextPtr &ctxptr);

   /** \brief �洢�ڵ㵽xml����� */
   void save(std::ostream &outstream, int indent=0);
	
protected:
   /** \brief ���캯�� */
   XMLNode() { nodetype = xml_nt_node; }

	/** \brief �������� */
	void deepcpy(const XMLNode &n);


   /** \brief ��ǩ��� */
   xmltagnamehandle nodenamehandle;

   /** \brief �����Ķ���ָ�� */
   XMLContextPtr contextptr;

   /** \brief �ڵ����� */
   xmlnodetype nodetype;

   /** \brief ��ǩ���� */
   XMLAttributes attributes;

   /** \brief ������ */
   string cdata;

   /** �ӽڵ����� */
   XMLNodeList nodelist;
};

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \class XMLDocument
 * \brief xml�ĵ��� */
/** \}*/
class XMLDocument: public XMLNode 
{
friend class xmlparser;
public:
   /** \brief ȱʡ���캯�� */
   XMLDocument();

   /** \brief ���캯��
	 * ͨ��xml������ָ�빹��xml�ĵ����� */
   explicit XMLDocument(XMLContextPtr pctx);

	/** \brief �������� */
	~XMLDocument();
	
   /** \brief ��ȡָ���ڵ� */
   XMLNodeList &get_pi_list(){ return procinstructions; }

   /** \brief ���ؼ��xml�ļ��﷨�Ϸ��Ե�DTD���� */
   XMLNodeList &get_dtd_list(){ return dtdrules; }

	/** \brief ���Ӵ���ָ��ڵ� */
	void add_pi(XMLNodePtr n);

	/** \brief ���Ӵ���ָ��ڵ� */
	void add_pi(XMLNode &n);

   /** \brief ����xml�ڵ� */
   void load( std::istream &instream, XMLContextPtr &ctxptr );

   /** \brief �洢xml�ڵ� */
   void save( std::ostream &outstream );
	
	/** \brief ���ļ�����xml�ڵ� */
	void load_file(string filename="");

	/** \brief ����xml�ĵ����� */
	void save_file(string filename="");

	/** \brief ��ȡ���趨�ļ��� */
	string filename(string name="");
	
protected:
   /** \brief �ڵ����� */
   XMLNodeList procinstructions;

   /** \brief DTD����ڵ����� */
   XMLNodeList dtdrules;

	/** \brief ����xml���ļ��� */
	string filename_;
};

}

END_SUNRISE_NAMESPACE

#endif

/* vi: set ts=3: */
