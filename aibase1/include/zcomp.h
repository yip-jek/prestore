/*
 * $Id: zcomp.h,v 1.7 2007/05/10 10:38:29 qsh Exp $
 */
#ifndef _ZCOMP_H
#define _ZCOMP_H

#include <stdio.h>


/** \example zcomp_test.cpp
 *  \brief .Zѹ����Ĳ��Գ���,�����ܲ��� */

/** \defgroup libcd �������ģ��(libcd)
 *  \{*/
/** \file zcomp.h 
 *  \brief ������.Zѹ����ѹ�����ͷ�ļ� 
 */
/** \class CZip
 *  \brief .Zѹ����ѹ��
 */
/** \}*/

class CZip {
public:
	/** \brief ���캯�� */
	CZip();
	/** \brief �������� */
	~CZip();
	/** \brief .Zѹ������
	 *  \param out ����ȡ����mode���� MEMORY,FILE2MEM:Ŀ������ָ�� FILE,MEM2FILE:Ŀ���ļ��� 
	 *  \param in  ����ȡ����mode���� MEMORY,MEM2FILE:Դ����ָ�� FILE,FILE2MEM:Դ�ļ���
	 *  \param mode ѹ��ģʽ (MEMORY:�ڴ�ģʽ FILE:�ļ�ģʽ MEM2FILE: �ڴ浽�ļ�ģʽ FILE2MEM: �ļ����ڴ�ģʽ)
	 *  \param num/i_len  Դ���ݳ���
	 *  \param o_len ��������С
	 *  \param pNoMem ��������Ƿ�ռ䲻�㣨TRUEΪ���㣩
	 *  \return ����Ŀ�����ݳ���
	 */
	int compress(char *out, const char *in, int mode, int num = 0);
	int compress(char *out, const char *in, int mode, int i_len, int o_len, bool *pNoMem = NULL);
	/** \brief .Z��ѹ������
	 *  \param out ����ȡ����mode���� MEMORY,FILE2MEM:Ŀ������ָ�� FILE,MEM2FILE:Ŀ���ļ��� 
	 *  \param in  ����ȡ����mode���� MEMORY,MEM2FILE:Դ����ָ�� FILE,FILE2MEM:Դ�ļ���
	 *  \param mode ѹ��ģʽ (MEMORY:�ڴ�ģʽ FILE:�ļ�ģʽ MEM2FILE: �ڴ浽�ļ�ģʽ FILE2MEM: �ļ����ڴ�ģʽ)
	 *  \param num/i_len  Դ���ݳ���
	 *  \param o_len ��������С
	 *  \param pNoMem ��������Ƿ�ռ䲻�㣨TRUEΪ���㣩
	 *  \return ����Ŀ�����ݳ���
	 */
	int decompress(char *out, const char *in, int mode, int num = 0);
	int decompress(char *out, const char *in, int mode, int i_len, int o_len, bool *pNoMem = NULL);
	static char *error();
	/** \brief ����ѹ��,��ѹ��ģʽ */
	enum {
		MEMORY = 0,	/**< �ڴ浽�ڴ�ģʽ */
		FILE = 1,	/**< �ļ����ļ�ģʽ */
		MEM2FILE = 2,	/**< �ڴ浽�ļ�ģʽ */
		FILE2MEM = 3	/**< �ļ����ڴ�ģʽ */
	};
private:
	int zread();
	int zwrite();
};
#endif
