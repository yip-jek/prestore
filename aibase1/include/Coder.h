#ifndef __CODER_H__
#define __CODER_H__

#include <stdio.h>
#include <string.h>

/** \example CCoder_test.cpp
 *  \brief CCoder_test��ʾ������ */

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \file Coder.h 
    \brief CCoder���ͷ�ļ� */   
    
/** \class CCoder
  * \brief Base64���ܽ����� */
/** \}*/
class CCoder
{
public:
	static unsigned int m_LineWidth;   /**< �����ÿ�еĳ��ȣ�ȱʡ��76 */
	static const char BASE64_ENCODE_TABLE[64]; /**< Base64����� */
	static const unsigned int BASE64_DECODE_TABLE[256];  /**< Base64����� */
	static const unsigned char QUOTED_ENCODE_TABLE[256]; /**< Quoted����� */

public:
	/** \brief ���ݱ������ʵ�ʳ��Ȼ�ȡBase64�����ĳ���,����ռ��ʱ������õ�
	  * \param iSize �������ĳ���
	  * \return �������ĳ��� */
	static int Base64EncodeSize(int iSize);	
	/** \brief ���ݽ������ʵ�ʳ��Ȼ�ȡBase64�����ĳ���,����ռ��ʱ������õ�
	  * \param iSize �������ĳ���
	  * \return �������ĳ��� */	
	static int Base64DecodeSize(int iSize);

	/** \brief ��һ��Buffer����Base64����
	  * \param pSrc ����Buffer
	  * \param nSize Buffer����
	  * \param pDest �������
	  * \return ��������ʵ�ʳ���,���ʧ��,����0 */
	static int base64_encode(char *pSrc, unsigned int nSize, char *pDest);

	/** \brief ��һ��Buffer����Base64����
	  * \param pSrc ����Buffer
	  * \param nSize Buffer����
	  * \param pDest �������
	  * \return ��������ʵ�ʳ���,���ʧ��,����0 */
	static int  base64_decode(char *pSrc, unsigned int nSize, char *pDest);

};

#endif //__CODER_H__
