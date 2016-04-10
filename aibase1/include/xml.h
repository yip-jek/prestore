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
 *  \brief xml文件写，读入和导航测试 */ 

/** \example xml_test1.cpp
 *  \brief xml文件读入分层次打印测试 */
 
/** \example xml_test2.cpp
 *  \brief */ 

/** \example xml_test3.cpp
 *  \brief */ 

BEGIN_SUNRISE_NAMESPACE

namespace xml {

using namespace std;

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \file xml.h
 *  \brief xml辅助结构类定义.*/
  
/** \def XML上下文对象指针 */
typedef class XMLContext* XMLContextPtr;

/** \class xmlerror
 * \brief xml异常类. 含有解释xml文件过程中的错误码和错误信息. */ 
/** \}*/
class xmlerror {
public:
   /** \brief 构造函数 */
   xmlerror( xmlerrorcode code, string info="" ) { errorcode = code; errorinfo=info; }

   /** \brief 获取错误号 */
   xmlerrorcode get_error() const { return errorcode; }

   /** \brief 获取错误信息 */
   const string get_string() const;

   /** \brief 获取附加的错误信息 */
	const string get_info() const { return errorinfo; }

	/** \brief 显示错误信息	 */
	void show_error(XMLContextPtr c);

	/** \brief 显示错误出现在文件里所在的行	 */
	void show_line(XMLContextPtr c, string filename);

protected:
   xmlerrorcode errorcode;
	string errorinfo;	
};

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \class XMLContext
 * \brief 解释上下文类
 * xml结构栈的上下文. */
/** \}*/
class XMLContext {
public:
   /** \brief 构造函数 */
   XMLContext();

   /** \brief 析构函数 */
   virtual ~XMLContext();

   /** \brief 返回entity(实体)值 */
   string get_entity( string &entname );

   /** \brief 返回标签值 */
   string get_tagname( xmltagnamehandle handle );

   /** \brief 插入标签值. */
   xmltagnamehandle insert_tagname( string &tagname );

   /** \brief 定位xml流. */
   xmllocation &get_location() { return location; }

   /** \brief 初始化上下文
	 * 衍生类需要overload，并在初始化后设定init为true。 */
   virtual void init_context() { init = true; }

protected:
   /** \brief 已经初始化的标志 */
   bool init;

   /** \brief 下一个处理的标签 */
   int nexthandle;

   /** \brief 标签关联表. */
   xmltagnamemap tagnames;

   /** \brief 实体关联表 */
   xmlentitymap entities;

   /** \brief xml输入流定位信息 */
   xmllocation location;
};

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \def XMLNodePtr
 * xml节点指针定义 */
typedef class XMLNode* XMLNodePtr;

/** \def XMLNodeList
 * xml节点链表定义 */
typedef std::list<XMLNodePtr> XMLNodeList;

/** \def XMLNodeListIterator
 * xml节点链表迭代子定义 */
typedef XMLNodeList::iterator XMLNodeListIterator;

/** \def XMLNodeListConstIterator
 * xml节点链表const迭代子定义 */
typedef XMLNodeList::const_iterator XMLNodeListConstIterator;


/** \class XMLAttributes
 * \brief 使用STL map存放标签的所有属性和值 */
/** \}*/
class XMLAttributes: public std::map<string, string> {
friend class xmlparser;

public:
   /** \brief 构造函数 */
   XMLAttributes(){}

   /** \brief 获取属性值 */
   string get(string key);

   /** \brief 设置新的属性值 */
   void set(string key, string value);

	/** \brief 移除属性值 */
	void remove(const string &key);
};


/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \class XMLNode xml.h
 * \brief xml节点类 */
/** \}*/
class XMLNode {
friend class xmlparser;

public:
   /** \brief 构造函数
	 * 必需显式调用	*/
   explicit XMLNode(XMLContextPtr pctx);

   /** \brief 构造函数 */
   explicit XMLNode(XMLContextPtr pctx, string nname);

   /** \brief 析构函数 */
   ~XMLNode();

   /** \brief 拷贝构造函数 */
   XMLNode(const XMLNode &node);

   /** \brief 赋值操作符
	 * 右值是xml节点对象引用. */
   XMLNode & operator =(const XMLNode &node);

   /** \brief 赋值操作符
	 * 右值是xml节点指针 */
   XMLNode & operator =(const XMLNodePtr n);
	
	/** \brief 获取或设定节点值
	 *  \param _name 如果是空串 */
	string name(string _name="");

  	/** \brief 获取或设定节点类型 */
	xmlnodetype type(xmlnodetype ntype=xml_nt_dummy);

	/** \brief 返回或者设定CDATA值 */
   const string data(string d="");

   /** \brief 返回节点属性关联表 */
   XMLAttributes &get_attrmap() { return attributes; }

   /** \brief 返回给定属性的属性值 */
   string get_attr(string attr) { return attributes.get(attr); }

	/** \brief 设定属性值 */
   void add_attr(string attr, string value){ attributes.set(attr,value); } 

	/** \brief 移除属性 */
	void remove_attr(string attr) {attributes.remove(attr); }

	/** \brief 替换当前属性关联表 */
	void replace_attr(XMLAttributes &attrmap) { attributes=attrmap; }
		
	/** \brief 增加子节点
	 * \param front - 如果设为true，则插入到链表的前面，否则插入到list的末端. */
	void add_child(XMLNodePtr n, bool front=false);

	/** \brief 增加子节点
	 * \param front - 如果设为true，则插入到链表的前面，否则插入到list的末端. */
	void add_child(XMLNode &n, bool front=false);

	/** \brief 增加子节点
	 * \param front - 如果设为true，则插入到链表的前面，否则插入到list的末端. */
	XMLNodePtr add_child(XMLContextPtr ctxp, string n, bool front=false);

	/** \brief 移除子节点 */
	void remove_child(XMLNodePtr n);

   /** \brief 选择节点 */
	const XMLNodeList & children(const string & = string()) const;

	/** \brief 获取给定子名字的节点集合中的第一个子节点 */
   XMLNodePtr firstchild(const string &n);

   /** \brief 从输入流载入xml节点 */
   void load(std::istream &instream, XMLContextPtr &ctxptr);

   /** \brief 存储节点到xml输出流 */
   void save(std::ostream &outstream, int indent=0);
	
protected:
   /** \brief 构造函数 */
   XMLNode() { nodetype = xml_nt_node; }

	/** \brief 链表的深复制 */
	void deepcpy(const XMLNode &n);


   /** \brief 标签句柄 */
   xmltagnamehandle nodenamehandle;

   /** \brief 上下文对象指针 */
   XMLContextPtr contextptr;

   /** \brief 节点类型 */
   xmlnodetype nodetype;

   /** \brief 标签属性 */
   XMLAttributes attributes;

   /** \brief 岛数据 */
   string cdata;

   /** 子节点链表 */
   XMLNodeList nodelist;
};

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \class XMLDocument
 * \brief xml文档类 */
/** \}*/
class XMLDocument: public XMLNode 
{
friend class xmlparser;
public:
   /** \brief 缺省构造函数 */
   XMLDocument();

   /** \brief 构造函数
	 * 通过xml上下文指针构造xml文档对象 */
   explicit XMLDocument(XMLContextPtr pctx);

	/** \brief 析构函数 */
	~XMLDocument();
	
   /** \brief 获取指令处理节点 */
   XMLNodeList &get_pi_list(){ return procinstructions; }

   /** \brief 返回检测xml文件语法合法性的DTD链表 */
   XMLNodeList &get_dtd_list(){ return dtdrules; }

	/** \brief 增加处理指令节点 */
	void add_pi(XMLNodePtr n);

	/** \brief 增加处理指令节点 */
	void add_pi(XMLNode &n);

   /** \brief 加载xml节点 */
   void load( std::istream &instream, XMLContextPtr &ctxptr );

   /** \brief 存储xml节点 */
   void save( std::ostream &outstream );
	
	/** \brief 从文件加载xml节点 */
	void load_file(string filename="");

	/** \brief 保存xml文档对象 */
	void save_file(string filename="");

	/** \brief 获取或设定文件名 */
	string filename(string name="");
	
protected:
   /** \brief 节点链表 */
   XMLNodeList procinstructions;

   /** \brief DTD规则节点链表 */
   XMLNodeList dtdrules;

	/** \brief 保存xml的文件名 */
	string filename_;
};

}

END_SUNRISE_NAMESPACE

#endif

/* vi: set ts=3: */
