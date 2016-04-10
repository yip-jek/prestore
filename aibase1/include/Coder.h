#ifndef __CODER_H__
#define __CODER_H__

#include <stdio.h>
#include <string.h>

/** \example CCoder_test.cpp
 *  \brief CCoder_test类示例程序 */

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \file Coder.h 
    \brief CCoder类的头文件 */   
    
/** \class CCoder
  * \brief Base64加密解密类 */
/** \}*/
class CCoder
{
public:
	static unsigned int m_LineWidth;   /**< 编码后每行的长度，缺省是76 */
	static const char BASE64_ENCODE_TABLE[64]; /**< Base64编码表 */
	static const unsigned int BASE64_DECODE_TABLE[256];  /**< Base64解码表 */
	static const unsigned char QUOTED_ENCODE_TABLE[256]; /**< Quoted编码表 */

public:
	/** \brief 根据编码对象实际长度获取Base64编码后的长度,分配空间的时候可能用到
	  * \param iSize 编码对象的长度
	  * \return 编码结果的长度 */
	static int Base64EncodeSize(int iSize);	
	/** \brief 根据解码对象实际长度获取Base64解码后的长度,分配空间的时候可能用到
	  * \param iSize 解码对象的长度
	  * \return 解码结果的长度 */	
	static int Base64DecodeSize(int iSize);

	/** \brief 对一段Buffer进行Base64编码
	  * \param pSrc 输入Buffer
	  * \param nSize Buffer长度
	  * \param pDest 输出缓冲
	  * \return 编码结果的实际长度,如果失败,返回0 */
	static int base64_encode(char *pSrc, unsigned int nSize, char *pDest);

	/** \brief 对一段Buffer进行Base64解码
	  * \param pSrc 输入Buffer
	  * \param nSize Buffer长度
	  * \param pDest 输出缓冲
	  * \return 解码结果的实际长度,如果失败,返回0 */
	static int  base64_decode(char *pSrc, unsigned int nSize, char *pDest);

};

#endif //__CODER_H__
