
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

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \file xmltokenizer.h
 *  \brief xml��tokenizer */
 
/** \class xmltoken xmltokenizer.h
 * \brief xmltoken������������һ������, �����Ǳ�� */
 /** \}*/
class xmltoken
{
   friend class xmlstream_iterator;
public:
   /** \brief ȱʡ���캯�� */
   xmltoken(){ isliteral=true; literal=0; generic.empty(); }

   /** \brief ���캯��
    *  \param ch ���뵥���ַ���Ϊtoken������ */
   xmltoken( char ch ){ isliteral=true; literal=ch; generic.empty(); }

   /** \brief ���캯��
    *  \param ����һ���ַ�����Ϊtoken������ */
   xmltoken( string &str ):generic(str){ isliteral=false; literal=0; }

   /** \brief �����ĸ�Ƿ�����ĸ. */
   bool is_literal(){ return isliteral; }

   /** \brief ����Ƿ�����ĩ��.
    *  \return ��ǰtoken��������������true�����򷵻�false */
   bool is_endofstream(){ return isliteral && literal==EOF/*xmlstring::traits_type::eof()*/; }

   /** \brief ������ĸ	 */
   char get_literal(){ return literal; }

   /** \brief �����ַ��� */
   string &get_generic(){ return generic; }


   /** \brief ==������	 */
   bool operator ==(char ch){ return !isliteral?false:ch==literal; }

   /** \brief !=������	 */
   bool operator !=(char ch){ return !isliteral?true:ch!=literal; }

   /** \brief ==������   */
   bool operator ==(string str){ return !isliteral?str==generic:false; }

   /** \brief !=������ */
   bool operator !=(string str){ return !isliteral?str!=generic:true; }

   xmltoken &operator =(string &str){ generic.assign(str); isliteral=false; return *this; }

   /** \brief ������ĸ */
   xmltoken &operator =(char ch){ literal=ch; isliteral=true; return *this; }

protected:
   /** \brief ��ĸ��ʶ */
   bool isliteral;

   /** \brief ��ĸ */
   char literal;

   /** \brief ��ͨ�� */
   string generic;
};

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \class xmltokenizer
 *! \brief xml��ǲ���� */
/** \}*/
class xmltokenizer
{
public:
   /** \brief ���캯��	 
    *  \param is  ������
    *  \param loc ��������λ*/
   xmltokenizer(std::istream &is,xmllocation &loc):instr(is),location(loc){}

	/** \brief ��������	 */
   virtual ~xmltokenizer(){}

   /** \brief ȡֵ��������ȡ�õ�ǰtoken */
   xmltoken& operator*(){ return curtoken; }

   /** \brief ȡ���������ȡ�õ�ǰtoken��ָ�� */
   const xmltoken* operator->(){ return &curtoken; }

   /** \brief xml����������ǰtokenָ����һ��token	 */
   xmltokenizer &operator++(){ get_next(); return *this; }

   /** \brief xml������	 */
   xmltokenizer &operator++(int){ get_next(); return *this; }

   /** \brief ��ȡ��ǰ��� */
   xmltoken& get(){ return curtoken; }

   /** \brief �ѱ��������ջ */
   void put_back( xmltoken &token ){ tokenstack.push(token); }

   /** \brief �ѵ�ǰ���������ջ */
   void put_back(){ tokenstack.push(curtoken); }
protected:

   /** \brief ������һ����� */
   virtual void get_next() = 0;

   /** \brief ������ */
   std::istream &instr;

   /** \brief xml��λ����Ϣ */
   xmllocation &location; 

   /** \brief ��ǰ��� */
   xmltoken curtoken;

   /** \brief ���ջ */
   std::stack<xmltoken> tokenstack;
};

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \class xmlstream_iterator
 * \brief xml��������
 * ���ڱ���xml�����������еı�� */
/** \}*/
class xmlstream_iterator:public xmltokenizer
{
public:
   /** \brief ���캯��	 */
   xmlstream_iterator(std::istream &is,xmllocation &loc);

   /** \brief �����ڽ���xml��ʱ�Ƿ���Կո� */
   void set_cdata_mode(bool to=false) { cdata_mode=to; }
   
protected:
   void get_next();

   /** \brief �ж��Ƿ���ĸ */
   bool is_literal( char c );

	/** \brief �ж��Ƿ�ո� */
   bool is_whitespace( char c );

	/** \brief �ж��Ƿ����з� */
   bool is_newline( char c );

	/** \brief �ж��Ƿ�ָ�� */
   bool is_stringdelimiter( char c ); 

   /** \brief ��ʶ�Ƿ���Կհ��ַ� */
   bool cdata_mode;

   char putback_char;
};

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \class xmldtd_iterator
 * \brief DTD������������ */
/** \}*/
class xmldtd_iterator:public xmltokenizer
{
public:
   /** \brief ���캯�� */
   xmldtd_iterator(std::istream &is,xmllocation &loc);

protected:
   void get_next(){}
};


}

END_SUNRISE_NAMESPACE

#endif
/* vi: set ts=3: */
