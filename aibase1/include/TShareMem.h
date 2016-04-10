/*
 * $Id: TShareMem.h,v 1.2 2007/04/24 05:47:17 qsh Exp $
 * Author: vincent
 */

#ifndef TSHARE_MEM_H
#define TSHARE_MEM_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define SHARE_SHM_PERM 0644

/** \defgroup srbc libshm 
 */

/** \file TShareMem.h
 *  \brief �����ڴ�������ͷ�ļ�
 */

/** \class TShareMem
 *  \brief �����ڴ������
 */

/** \example exTShareMemld.cpp 
 *  \example exTShareMem.cpp
 *  \brief TShareMem��ʾ������ 
 */

class TShareMem {
private:
	int m_shmid;
	void *m_pmem;

public:
	TShareMem();
	
	/*! \brief ����˵�������ӹ����ڴ�
	 * 	@param[in] id : �����ڴ��key
	 * 	@param[in] isize : �����ڴ�Ĵ�С���������0������������ֻ������
	 * 	@param[in] mode: �����ڴ��ģʽ
	 * 	@return : �ɹ������ع����ڴ���׵�ַ��ʧ�ܣ�����NULL
	 */
	void *f_getMem(key_t id, size_t isize, int mode = SHARE_SHM_PERM);
	
	/*! \brief ����˵����ɾ�������ڴ�
	 * 	@return : ��
	 */
	void f_delMem();

	/*! \brief ����˵�����ͷŹ����ڴ�����
	 * 	@return : ��
	 */
	void f_unlinkMem();

	/*! \brief ����˵�������ع����ڴ�Ĵ�С
	 * 	@return : �����ڴ�Ĵ�С
	 */
	size_t f_getSize();

	/*! \brief ����˵�������ع����ڴ��id(����key)
	 * 	@return : ��
	 */
	int f_getShmID();
};

#endif
