#ifndef __GZCOMP_H__
#define __GZCOMP_H__
#include "zlib.h"
/*

Notice:
*	The class CGZip can be compiled only when the system have installed zlib library
* which always install  in "/usr" directory,and it's header is "/usr/include/zlib.h" with the 
* ibrary "/usr/lib/libz".In this case you must include it in your makefile;

*/




/*

When the mode is MEMORY or FILE2MEM:
*
*	The size of the destination must be large enough; It should be assured by user;
*	when gzdecompress ,we can't guess the actual size of how large the destination needs.
* The size of the uncompressed data must have been saved previously by the
* compressor and transmitted to the decompressor by some mechanism outside
* the scope of this compression library;
*	when compress, it must be at least 0.1% larger than sourceLen plus 12 bytes.

*/



/** \example zcomp_test.cpp
 * \brief 压缩/解压缩Demo
 */

/** \file gzcomp.h
   BSD gzip/libz 压缩/解压算法
 */

/** \class CGZip
    SD gzip/libz 压缩/解压算法类
 */

class CGZip
{
public:
	/** \brief 缺省构造函数
	*/
	CGZip();

	/** \brief 析构函数
	*/
	~CGZip();

public:
	/** \brief 压缩
	* \param szDst 目的数据指针
	* \param szSrc  源数据指针
	* \param mode 压缩模式 
		(MEMORY:内存模式 FILE:文件模式 MEM2FILE: 内存到文件模式 FILE2MEM: 文件到内存模式)
	* \param num  源数据长度
	* @return 返回目的数据长度
	*/
	int gzcompress(char *szDst, const char *szSrc, int mode, int num=0);

	/** \brief 解压缩
	* \param szDst 目的数据指针
	* \param szSrc  源数据指针
	* \param mode 解压缩模式 
		(MEMORY:内存模式 FILE:文件模式 MEM2FILE: 内存到文件模式 FILE2MEM: 文件到内存模式)
	* \param num  源数据长度
	* @return 返回目的数据长度
	*/ 
	int gzdecompress(char *szDst, const char *szSrc, int mode, int num=0);


	/** \brief 	打开压缩文件
	*   \param 	FileName 文件名
	*   \param 	Mode 打开模式,"r"对应读,"w"对应写，其它非法;
	*   \return 	成功为0,出错为-1
	*/
	int gz_open(const char * FileName, char * Mode);

	/** \brief 	读压缩文件
	*   \param 	buf 读入数据缓冲区
	*   \param 	len 读入字节数，为解压后的长度 
	*   \return 	成功返回实际读入长度,出错为-1
	*/
	int gz_read(char * buf, int len);

	/** \brief 	写压缩文件
	*   \param 	buf 写入数据缓冲区
	*   \param 	len 数据缓冲区长度
	*   \return 	成功返回实际写入长度(压缩前),出错为-1
	*/
	int gz_write(const char * buf, int len);

	/** \brief 	关闭压缩文件
	*   \return 	0;
	*/
	int gz_close();

	public:  
	/** \enum  压缩/解压缩模式枚举
	*/ 
	enum
	{
		MEMORY   = 0, /**< 内存模式 */
        	FILE     = 1, /**< 文件模式 */
        	MEM2FILE = 2, /**< 内存到文件模式 */ 
        	FILE2MEM = 3  /**< 文件到内存模式 */
	};

private:
	/** \brief 查错函数*/
	int CHECK_ERR(int err,const char* msg); 

private:
	/** \brief 压缩文件标识 */
	gzFile m_zfile; 
	gzFile m_zfile1; 
};

#endif // __GCOMP_H__

