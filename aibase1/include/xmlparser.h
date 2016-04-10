#ifndef __XMLPARSER_H__
#define __XMLPARSER_H__

#include <cstddef>            // for std::size_t
#include <memory>             // for std::auto_ptr
#include <algorithm>          // for std::swap
#include <functional>         // for std::less

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "xml.h"
#include "xmltokenizer.h"
#include "xmlhelpers.h"


BEGIN_SUNRISE_NAMESPACE

namespace xml {
/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \file xmlparser.h
 * \brief	xml������ */
 
/** \class xmlparser
 *  \brief xml������ */
/** \}*/
class xmlparser {
public:
   /** \brief ���캯�� */
   xmlparser( std::istream &inputstream, xmllocation &loc );

   /** \brief ����xml�ĵ�
	 * \param doc - ���ڵ�, ���������. */
   bool parse_document( XMLDocument &doc, XMLContextPtr &ctxptr );

   /** \brief ���ͽڵ� */
   bool parse_node( XMLNode &node, XMLContextPtr &ctxptr );

protected:
   /** \brief ����xmlͷ
	 * ���ʹ���ָ�doctype�ȵ�. */
   bool parse_header( XMLDocument &doc, XMLContextPtr &ctxptr );

   /** \brief ����xml��ǩ���� */
   bool parse_attributes( XMLAttributes &attr );

   /** \brief ����<!-- -->  */
   void parse_comment( XMLContextPtr &ctxptr );

   std::istream &instream; /**< ������ */
   xmlstream_iterator tokenizer; /**< ������������ */
};

}

END_SUNRISE_NAMESPACE

#endif
/* vi: set ts=3: */
