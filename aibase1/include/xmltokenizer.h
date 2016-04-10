
#ifndef __XMLTOKENIZER_H__
#define __XMLTOKENIZER_H__

#include <stdio.h>
#include <string>
#include <stack>
#include <iosfwd>

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif

#include "xmlhelpers.h"

BEGIN_SUNRISE_NAMESPACE
namespace xml {

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \file xmltokenizer.h
 *  \brief xml流tokenizer */
 
/** \class xmltoken xmltokenizer.h
 * \brief xmltoken代表从流读入的一个符号, 可能是表程 */
 /** \}*/
class xmltoken
{
   friend class xmlstream_iterator;
public:
   /** \brief 缺省构造函数 */
   xmltoken(){ isliteral=true; literal=0; generic.empty(); }

   /** \brief 构造函数
    *  \param ch 传入单个字符作为token的内容 */
   xmltoken( char ch ){ isliteral=true; literal=ch; generic.empty(); }

   /** \brief 构造函数
    *  \param 传入一个字符串作为token的内容 */
   xmltoken( string &str ):generic(str){ isliteral=false; literal=0; }

   /** \brief 检测字母是否是字母. */
   bool is_literal(){ return isliteral; }

   /** \brief 检测是否到了流末端.
    *  \return 当前token是流结束符返回true，否则返回false */
   bool is_endofstream(){ return isliteral && literal==EOF/*xmlstring::traits_type::eof()*/; }

   /** \brief 返回字母	 */
   char get_literal(){ return literal; }

   /** \brief 返回字符串 */
   string &get_generic(){ return generic; }


   /** \brief ==操作符	 */
   bool operator ==(char ch){ return !isliteral?false:ch==literal; }

   /** \brief !=操作符	 */
   bool operator !=(char ch){ return !isliteral?true:ch!=literal; }

   /** \brief ==操作符   */
   bool operator ==(string str){ return !isliteral?str==generic:false; }

   /** \brief !=操作符 */
   bool operator !=(string str){ return !isliteral?str!=generic:true; }

   xmltoken &operator =(string &str){ generic.assign(str); isliteral=false; return *this; }

   /** \brief 设置字母 */
   xmltoken &operator =(char ch){ literal=ch; isliteral=true; return *this; }

protected:
   /** \brief 字母标识 */
   bool isliteral;

   /** \brief 字母 */
   char literal;

   /** \brief 普通串 */
   string generic;
};

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \class xmltokenizer
 *! \brief xml标记拆分类 */
/** \}*/
class xmltokenizer
{
public:
   /** \brief 构造函数	 
    *  \param is  输入流
    *  \param loc 输入流定位*/
   xmltokenizer(std::istream &is,xmllocation &loc):instr(is),location(loc){}

	/** \brief 析构函数	 */
   virtual ~xmltokenizer(){}

   /** \brief 取值操作符，取得当前token */
   xmltoken& operator*(){ return curtoken; }

   /** \brief 取域操作符，取得当前token的指针 */
   const xmltoken* operator->(){ return &curtoken; }

   /** \brief xml流迭代，当前token指向下一个token	 */
   xmltokenizer &operator++(){ get_next(); return *this; }

   /** \brief xml流迭代	 */
   xmltokenizer &operator++(int){ get_next(); return *this; }

   /** \brief 获取当前标记 */
   xmltoken& get(){ return curtoken; }

   /** \brief 把标记推入标记栈 */
   void put_back( xmltoken &token ){ tokenstack.push(token); }

   /** \brief 把当前标记推入标记栈 */
   void put_back(){ tokenstack.push(curtoken); }
protected:

   /** \brief 解析下一个标记 */
   virtual void get_next() = 0;

   /** \brief 输入流 */
   std::istream &instr;

   /** \brief xml流位置信息 */
   xmllocation &location; 

   /** \brief 当前标记 */
   xmltoken curtoken;

   /** \brief 标记栈 */
   std::stack<xmltoken> tokenstack;
};

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \class xmlstream_iterator
 * \brief xml流迭代子
 * 用于遍历xml输入流中所有的标记 */
/** \}*/
class xmlstream_iterator:public xmltokenizer
{
public:
   /** \brief 构造函数	 */
   xmlstream_iterator(std::istream &is,xmllocation &loc);

   /** \brief 设置在解释xml流时是否忽略空格 */
   void set_cdata_mode(bool to=false) { cdata_mode=to; }
   
protected:
   void get_next();

   /** \brief 判断是否字母 */
   bool is_literal( char c );

	/** \brief 判断是否空格 */
   bool is_whitespace( char c );

	/** \brief 判断是否新行符 */
   bool is_newline( char c );

	/** \brief 判断是否分格符 */
   bool is_stringdelimiter( char c ); 

   /** \brief 标识是否忽略空白字符 */
   bool cdata_mode;

   char putback_char;
};

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \class xmldtd_iterator
 * \brief DTD输入流迭代子 */
/** \}*/
class xmldtd_iterator:public xmltokenizer
{
public:
   /** \brief 构造函数 */
   xmldtd_iterator(std::istream &is,xmllocation &loc);

protected:
   void get_next(){}
};


}

END_SUNRISE_NAMESPACE

#endif
/* vi: set ts=3: */
