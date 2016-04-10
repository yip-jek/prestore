#ifndef __XMLCOMMON_H__
#define __XMLCOMMON_H__

#include <string>

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \file xmlcommon.h
 *  \brief 定义了xml解释器使用到的一些typedef和enum*/
/** \}*/

BEGIN_SUNRISE_NAMESPACE

namespace xml {

using namespace std;
		  
/** \def 标签关联表中标签的句柄
 */
typedef int xmltagnamehandle;

/** \def 标签关联表定义
 */
typedef map<xmltagnamehandle,string> xmltagnamemap;

/** \def xml实体关联表定义
 */
typedef map<string, string> xmlentitymap;

		  
/** \enum xmlerrorcode
 * \brief xml解释过程中可能遇到的错误枚举
 */
enum xmlerrorcode {
	xml_unknown = 0, /**< 无定义 */
	xml_instream_error, /**< 输入流错误 */
	//! expected an open tag literal '<'
	xml_opentag_expected, /**< 期望字母'<' */
	xml_opentag_cdata_expected, /**< 期望字母'<'或者CDATA */
	xml_closetag_expected, /**< 期望字母'>' */
	xml_pi_doctype_expected, /**< 期望处理指令或者文件类型标签 */
	xml_tagname_expected, /**< 期望字母'<'或者'</'之后的标签名 */
	xml_closetag_slash_expected, /**< 期望字母'<'之后字母'/' */
	xml_tagname_close_mismatch, /**< 开始和结束的标签名不匹配 */
	xml_attr_equal_expected, /**< 期望属性名后的字母'=' */
	xml_attr_value_expected, /**< 期望字母'='后的属性值 */
	xml_save_invalid_nodetype, /**< 节点类型冲突 */
	xml_dummy,
	xml_name_not_found, /**< 找不到名字 */
	xml_childlist_empty, /**< 节点没有子节点 */
	xml_filename_invalid, /**< 非法文件名 */
	xml_file_access	/**< 文件不能访问 */
};

/** \enum xmlnodetype
 * \brief xml节点类型枚举
 */
enum xmlnodetype {
	xml_nt_node, /**< 一般节点，可包含子节点 */
	xml_nt_leaf, /**< 叶子节点, 不能再有子节点 */
	xml_nt_document, /**< xml文档的根节点 */
	xml_nt_cdata, /**< CDATA节点, 只包含字符数据 */ 
	xml_nt_dummy /**< 无定义节点 */
};

}

END_SUNRISE_NAMESPACE

#endif
/* vi: set ts=3: */
