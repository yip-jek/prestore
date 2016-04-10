/***************************************************************
  File name:     TSem.h
  Author:        vincent
  Version:       1.0
  Description:   信号量互斥类
  Others:        // 其它内容的说明
  History:       // 修改历史记录列表，每条修改记录应包括修改日期、修改
                 // 者及修改内容简述  
    1. Date:
       Author:
       Modification:
    2. ...
***************************************************************/

#ifndef T_SRBC_MUTEX_H_
#define T_SRBC_MUTEX_H_


#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#define		SRBC_SEMNUM			1
#define		SRBC_SEM_0			0
#define		SRBC_SEM_1			1
#define		SRBC_SEM_PERM		0660

/** \defgroup srbc libshm 
 */

/** \file TSem.h
 *  \brief 信号量管理类的头文件
 */

/** \class TSem
 *  \brief 信号量管理类
 */

/** \example exTSem.cpp
 *  \brief TSem类示例程序 
 */

//##Documentation
//## 信号量管理类
class TSem
{
	private:
		key_t  m_key;
		int m_semid;
		
		union semun {

			int val;

			struct semid_ds *buf;

			unsigned short *array;

		} arg;
		
	public:
		/*! \brief 功能说明：构造函数
		 * 	@param[in] id : 信号量的key
		 * 	@return 无 
		 */
		TSem(int id);
		~TSem();
		
	public:
		/*! \brief 功能说明：锁定
		 * 	@param[in] id : 信号量的key
		 * 	@return : 成功true，失败false 
		 */
		bool lock();
		
		/*! \brief 功能说明：解锁
		 * 	@param[in] id : 信号量的key
		 * 	@return : 成功true，失败false 
		 */
		bool unlock();

		int getval();
		
		/*! \brief 功能说明：连接信号量
		 * 	@return : 成功true，失败false 
		 */
		bool lnkSem();

		/*! \brief 功能说明：新建信号量
		 * 	@return : 成功true，失败false 
		 */
		bool crtSem();

		/*! \brief 功能说明：删除信号量
		 * 	@return : 成功true，失败false 
		 */
		bool delSem();
};


#endif
