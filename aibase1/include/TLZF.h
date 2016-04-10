/***************************************************************
  File name:     TLZF.h
  Author:        Tony
  Version:       1.0
  Date:          2002/12/31
  Description:   lzf算法压缩/解压缩类
  Others:        // 其它内容的说明
  History:       // 修改历史记录列表，每条修改记录应包括修改日期、修改
                 // 者及修改内容简述  
    1. Date: 10/23/03
       Author: Tony
       Modification: 调整crc32.h
    2. ...
***************************************************************/
/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \file TLZF.h
  * \brief TLZF类的头文件*/
/** \}*/

/** \example TLZF_test.cpp
 *  \brief TLZF类的示例程序 */

#ifndef __TLZF_H__
#define __TLZF_H__

#include "lzfP.h"

#define HSIZE (1 << (HLOG))

/*
 * don't play with this unless you benchmark!
 * decompression is not dependent on the hash function
 * the hashing function might seem strange, just believe me
 * it works ;)
 */
#define FRST(p) (((p[0]) << 8) + p[1])
#define NEXT(v,p) (((v) << 8) + p[2])
#define IDX(h) ((((h ^ (h << 5)) >> (3*8 - HLOG)) + h*3) & (HSIZE - 1))
/*
 * IDX works because it is very similar to a multiplicative hash, e.g.
 * (h * 57321 >> (3*8 - HLOG))
 * the next one is also quite good, albeit slow ;)
 * (int)(cos(h & 0xffffff) * 1e6)
 */

#if 0
/* original lzv-like hash function */
# define FRST(p) (p[0] << 5) ^ p[1]
# define NEXT(v,p) ((v) << 5) ^ p[2]
# define IDX(h) ((h) & (HSIZE - 1))
#endif

#define        MAX_LIT        (1 <<  5)
#define        MAX_OFF        (1 << 13)
#define        MAX_REF        ((1 <<  8) + (1 << 3))

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \class TLZF
  * \brief lzf算法压缩/解压缩类 */
/** \}*/
class TLZF 
{
   public:
      /** \brief 内存方式压缩,压缩对象在内存缓冲区,压缩后输出到指定文件 
        * \param str 压缩对象字符串 
        * \param out_file 压缩结果输出文件名 */
      static void mem_compress(const char* str,
                              const char* out_file);
      /** \brief 文件方式压缩,压缩对象在指定的输入文件中,压缩后输出到指定文件
        * \param in_file 压缩对象文件名
        * \param out_file 压缩结果输出文件名 */
      static void compress(const char* in_file, 
                           const char* out_file);
      /** \brief 文件方式解压缩,从指定的输入压缩文件解压缩输出到指定的输出文件
        * \param in_file 输入压缩文件名
        * \param out_file 输出压缩文件名 */
      static void decompress(const char* in_file,
                           const char* out_file);

   private:
      /** \brief 内存方式lzf压缩
        * \param in_data  压缩对象缓冲区指针
        * \param in_len 压缩对象缓冲区长度
        * \param out_data 压缩结果输出缓冲区指针
        * \param out_len 压缩结果输出缓冲区长度
        * \return 返回压缩结果的实际长度,如果失败,返回0 */
      static unsigned int lzf_compress(const u8 *const  in_data,
                                       unsigned int in_len,
                                       u8 *out_data,
                                       unsigned int out_len);
      /** \brief 内存方式lzf解压缩
        * \param in_data  解压缩对象缓冲区指针
        * \param in_len 解压缩对象缓冲区长度
        * \param out_data 解压缩结果输出缓冲区指针
        * \param out_len 解压缩结果输出缓冲区长度
        * \return 返回解压缩结果的实际长度,如果失败,返回0 */
      static size_t lzf_decompress(const u8 *const  in_data,
                                          unsigned int in_len,
                                          u8* out_data,
                                          unsigned int out_len);

};

#endif
