/***************************************************************
  File name:     TLZF.h
  Author:        Tony
  Version:       1.0
  Date:          2002/12/31
  Description:   lzf�㷨ѹ��/��ѹ����
  Others:        // �������ݵ�˵��
  History:       // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                 // �߼��޸����ݼ���  
    1. Date: 10/23/03
       Author: Tony
       Modification: ����crc32.h
    2. ...
***************************************************************/
/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \file TLZF.h
  * \brief TLZF���ͷ�ļ�*/
/** \}*/

/** \example TLZF_test.cpp
 *  \brief TLZF���ʾ������ */

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

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \class TLZF
  * \brief lzf�㷨ѹ��/��ѹ���� */
/** \}*/
class TLZF 
{
   public:
      /** \brief �ڴ淽ʽѹ��,ѹ���������ڴ滺����,ѹ���������ָ���ļ� 
        * \param str ѹ�������ַ��� 
        * \param out_file ѹ���������ļ��� */
      static void mem_compress(const char* str,
                              const char* out_file);
      /** \brief �ļ���ʽѹ��,ѹ��������ָ���������ļ���,ѹ���������ָ���ļ�
        * \param in_file ѹ�������ļ���
        * \param out_file ѹ���������ļ��� */
      static void compress(const char* in_file, 
                           const char* out_file);
      /** \brief �ļ���ʽ��ѹ��,��ָ��������ѹ���ļ���ѹ�������ָ��������ļ�
        * \param in_file ����ѹ���ļ���
        * \param out_file ���ѹ���ļ��� */
      static void decompress(const char* in_file,
                           const char* out_file);

   private:
      /** \brief �ڴ淽ʽlzfѹ��
        * \param in_data  ѹ�����󻺳���ָ��
        * \param in_len ѹ�����󻺳�������
        * \param out_data ѹ��������������ָ��
        * \param out_len ѹ������������������
        * \return ����ѹ�������ʵ�ʳ���,���ʧ��,����0 */
      static unsigned int lzf_compress(const u8 *const  in_data,
                                       unsigned int in_len,
                                       u8 *out_data,
                                       unsigned int out_len);
      /** \brief �ڴ淽ʽlzf��ѹ��
        * \param in_data  ��ѹ�����󻺳���ָ��
        * \param in_len ��ѹ�����󻺳�������
        * \param out_data ��ѹ��������������ָ��
        * \param out_len ��ѹ������������������
        * \return ���ؽ�ѹ�������ʵ�ʳ���,���ʧ��,����0 */
      static size_t lzf_decompress(const u8 *const  in_data,
                                          unsigned int in_len,
                                          u8* out_data,
                                          unsigned int out_len);

};

#endif
