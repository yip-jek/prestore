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
 *  \brief 共享内存管理类的头文件
 */

/** \class TShareMem
 *  \brief 共享内存管理类
 */

/** \example exTShareMemld.cpp 
 *  \example exTShareMem.cpp
 *  \brief TShareMem类示例程序 
 */

class TShareMem {
private:
	int m_shmid;
	void *m_pmem;

public:
	TShareMem();
	
	/*! \brief 功能说明：链接共享内存
	 * 	@param[in] id : 共享内存的key
	 * 	@param[in] isize : 共享内存的大小，如果大于0即创建，否则只是链接
	 * 	@param[in] mode: 共享内存的模式
	 * 	@return : 成功，返回共享内存的首地址；失败，返回NULL
	 */
	void *f_getMem(key_t id, size_t isize, int mode = SHARE_SHM_PERM);
	
	/*! \brief 功能说明：删除共享内存
	 * 	@return : 无
	 */
	void f_delMem();

	/*! \brief 功能说明：释放共享内存链接
	 * 	@return : 无
	 */
	void f_unlinkMem();

	/*! \brief 功能说明：返回共享内存的大小
	 * 	@return : 共享内存的大小
	 */
	size_t f_getSize();

	/*! \brief 功能说明：返回共享内存的id(不是key)
	 * 	@return : 无
	 */
	int f_getShmID();
};

#endif
