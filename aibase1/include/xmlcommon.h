#ifndef __XMLCOMMON_H__
#define __XMLCOMMON_H__

#include <string>

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \file xmlcommon.h
 *  \brief ������xml������ʹ�õ���һЩtypedef��enum*/
/** \}*/

BEGIN_SUNRISE_NAMESPACE

namespace xml {

using namespace std;
		  
/** \def ��ǩ�������б�ǩ�ľ��
 */
typedef int xmltagnamehandle;

/** \def ��ǩ��������
 */
typedef map<xmltagnamehandle,string> xmltagnamemap;

/** \def xmlʵ���������
 */
typedef map<string, string> xmlentitymap;

		  
/** \enum xmlerrorcode
 * \brief xml���͹����п��������Ĵ���ö��
 */
enum xmlerrorcode {
	xml_unknown = 0, /**< �޶��� */
	xml_instream_error, /**< ���������� */
	//! expected an open tag literal '<'
	xml_opentag_expected, /**< ������ĸ'<' */
	xml_opentag_cdata_expected, /**< ������ĸ'<'����CDATA */
	xml_closetag_expected, /**< ������ĸ'>' */
	xml_pi_doctype_expected, /**< ��������ָ������ļ����ͱ�ǩ */
	xml_tagname_expected, /**< ������ĸ'<'����'</'֮��ı�ǩ�� */
	xml_closetag_slash_expected, /**< ������ĸ'<'֮����ĸ'/' */
	xml_tagname_close_mismatch, /**< ��ʼ�ͽ����ı�ǩ����ƥ�� */
	xml_attr_equal_expected, /**< ���������������ĸ'=' */
	xml_attr_value_expected, /**< ������ĸ'='�������ֵ */
	xml_save_invalid_nodetype, /**< �ڵ����ͳ�ͻ */
	xml_dummy,
	xml_name_not_found, /**< �Ҳ������� */
	xml_childlist_empty, /**< �ڵ�û���ӽڵ� */
	xml_filename_invalid, /**< �Ƿ��ļ��� */
	xml_file_access	/**< �ļ����ܷ��� */
};

/** \enum xmlnodetype
 * \brief xml�ڵ�����ö��
 */
enum xmlnodetype {
	xml_nt_node, /**< һ��ڵ㣬�ɰ����ӽڵ� */
	xml_nt_leaf, /**< Ҷ�ӽڵ�, ���������ӽڵ� */
	xml_nt_document, /**< xml�ĵ��ĸ��ڵ� */
	xml_nt_cdata, /**< CDATA�ڵ�, ֻ�����ַ����� */ 
	xml_nt_dummy /**< �޶���ڵ� */
};

}

END_SUNRISE_NAMESPACE

#endif
/* vi: set ts=3: */
