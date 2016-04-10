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
 * \brief ѹ��/��ѹ��Demo
 */

/** \file gzcomp.h
   BSD gzip/libz ѹ��/��ѹ�㷨
 */

/** \class CGZip
    SD gzip/libz ѹ��/��ѹ�㷨��
 */

class CGZip
{
public:
	/** \brief ȱʡ���캯��
	*/
	CGZip();

	/** \brief ��������
	*/
	~CGZip();

public:
	/** \brief ѹ��
	* \param szDst Ŀ������ָ��
	* \param szSrc  Դ����ָ��
	* \param mode ѹ��ģʽ 
		(MEMORY:�ڴ�ģʽ FILE:�ļ�ģʽ MEM2FILE: �ڴ浽�ļ�ģʽ FILE2MEM: �ļ����ڴ�ģʽ)
	* \param num  Դ���ݳ���
	* @return ����Ŀ�����ݳ���
	*/
	int gzcompress(char *szDst, const char *szSrc, int mode, int num=0);

	/** \brief ��ѹ��
	* \param szDst Ŀ������ָ��
	* \param szSrc  Դ����ָ��
	* \param mode ��ѹ��ģʽ 
		(MEMORY:�ڴ�ģʽ FILE:�ļ�ģʽ MEM2FILE: �ڴ浽�ļ�ģʽ FILE2MEM: �ļ����ڴ�ģʽ)
	* \param num  Դ���ݳ���
	* @return ����Ŀ�����ݳ���
	*/ 
	int gzdecompress(char *szDst, const char *szSrc, int mode, int num=0);


	/** \brief 	��ѹ���ļ�
	*   \param 	FileName �ļ���
	*   \param 	Mode ��ģʽ,"r"��Ӧ��,"w"��Ӧд�������Ƿ�;
	*   \return 	�ɹ�Ϊ0,����Ϊ-1
	*/
	int gz_open(const char * FileName, char * Mode);

	/** \brief 	��ѹ���ļ�
	*   \param 	buf �������ݻ�����
	*   \param 	len �����ֽ�����Ϊ��ѹ��ĳ��� 
	*   \return 	�ɹ�����ʵ�ʶ��볤��,����Ϊ-1
	*/
	int gz_read(char * buf, int len);

	/** \brief 	дѹ���ļ�
	*   \param 	buf д�����ݻ�����
	*   \param 	len ���ݻ���������
	*   \return 	�ɹ�����ʵ��д�볤��(ѹ��ǰ),����Ϊ-1
	*/
	int gz_write(const char * buf, int len);

	/** \brief 	�ر�ѹ���ļ�
	*   \return 	0;
	*/
	int gz_close();

	public:  
	/** \enum  ѹ��/��ѹ��ģʽö��
	*/ 
	enum
	{
		MEMORY   = 0, /**< �ڴ�ģʽ */
        	FILE     = 1, /**< �ļ�ģʽ */
        	MEM2FILE = 2, /**< �ڴ浽�ļ�ģʽ */ 
        	FILE2MEM = 3  /**< �ļ����ڴ�ģʽ */
	};

private:
	/** \brief �����*/
	int CHECK_ERR(int err,const char* msg); 

private:
	/** \brief ѹ���ļ���ʶ */
	gzFile m_zfile; 
	gzFile m_zfile1; 
};

#endif // __GCOMP_H__

