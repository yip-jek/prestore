/*
 * $Id: zcomp.h,v 1.7 2007/05/10 10:38:29 qsh Exp $
 */
#ifndef _ZCOMP_H
#define _ZCOMP_H

#include <stdio.h>


/** \example zcomp_test.cpp
 *  \brief .Z压缩类的测试程序,带性能测试 */

/** \defgroup libcd 编码解码模块(libcd)
 *  \{*/
/** \file zcomp.h 
 *  \brief 定义了.Z压缩解压缩类的头文件 
 */
/** \class CZip
 *  \brief .Z压缩解压类
 */
/** \}*/

class CZip {
public:
	/** \brief 构造函数 */
	CZip();
	/** \brief 析构函数 */
	~CZip();
	/** \brief .Z压缩函数
	 *  \param out 含义取决于mode参数 MEMORY,FILE2MEM:目的数据指针 FILE,MEM2FILE:目的文件名 
	 *  \param in  含义取决于mode参数 MEMORY,MEM2FILE:源数据指针 FILE,FILE2MEM:源文件名
	 *  \param mode 压缩模式 (MEMORY:内存模式 FILE:文件模式 MEM2FILE: 内存到文件模式 FILE2MEM: 文件到内存模式)
	 *  \param num/i_len  源数据长度
	 *  \param o_len 输出缓存大小
	 *  \param pNoMem 输出缓存是否空间不足（TRUE为不足）
	 *  \return 返回目的数据长度
	 */
	int compress(char *out, const char *in, int mode, int num = 0);
	int compress(char *out, const char *in, int mode, int i_len, int o_len, bool *pNoMem = NULL);
	/** \brief .Z解压缩函数
	 *  \param out 含义取决于mode参数 MEMORY,FILE2MEM:目的数据指针 FILE,MEM2FILE:目的文件名 
	 *  \param in  含义取决于mode参数 MEMORY,MEM2FILE:源数据指针 FILE,FILE2MEM:源文件名
	 *  \param mode 压缩模式 (MEMORY:内存模式 FILE:文件模式 MEM2FILE: 内存到文件模式 FILE2MEM: 文件到内存模式)
	 *  \param num/i_len  源数据长度
	 *  \param o_len 输出缓存大小
	 *  \param pNoMem 输出缓存是否空间不足（TRUE为不足）
	 *  \return 返回目的数据长度
	 */
	int decompress(char *out, const char *in, int mode, int num = 0);
	int decompress(char *out, const char *in, int mode, int i_len, int o_len, bool *pNoMem = NULL);
	static char *error();
	/** \brief 四种压缩,解压缩模式 */
	enum {
		MEMORY = 0,	/**< 内存到内存模式 */
		FILE = 1,	/**< 文件到文件模式 */
		MEM2FILE = 2,	/**< 内存到文件模式 */
		FILE2MEM = 3	/**< 文件到内存模式 */
	};
private:
	int zread();
	int zwrite();
};
#endif
