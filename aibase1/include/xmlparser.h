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
/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \file xmlparser.h
 * \brief	xml解释类 */
 
/** \class xmlparser
 *  \brief xml解析类 */
/** \}*/
class xmlparser {
public:
   /** \brief 构造函数 */
   xmlparser( std::istream &inputstream, xmllocation &loc );

   /** \brief 解释xml文档
	 * \param doc - 根节点, 从这里解释. */
   bool parse_document( XMLDocument &doc, XMLContextPtr &ctxptr );

   /** \brief 解释节点 */
   bool parse_node( XMLNode &node, XMLContextPtr &ctxptr );

protected:
   /** \brief 解释xml头
	 * 解释处理指令，doctype等等. */
   bool parse_header( XMLDocument &doc, XMLContextPtr &ctxptr );

   /** \brief 解释xml标签属性 */
   bool parse_attributes( XMLAttributes &attr );

   /** \brief 解释<!-- -->  */
   void parse_comment( XMLContextPtr &ctxptr );

   std::istream &instream; /**< 输入流 */
   xmlstream_iterator tokenizer; /**< 输入流迭代子 */
};

}

END_SUNRISE_NAMESPACE

#endif
/* vi: set ts=3: */
