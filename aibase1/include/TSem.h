/***************************************************************
  File name:     TSem.h
  Author:        vincent
  Version:       1.0
  Description:   �ź���������
  Others:        // �������ݵ�˵��
  History:       // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                 // �߼��޸����ݼ���  
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
 *  \brief �ź����������ͷ�ļ�
 */

/** \class TSem
 *  \brief �ź���������
 */

/** \example exTSem.cpp
 *  \brief TSem��ʾ������ 
 */

//##Documentation
//## �ź���������
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
		/*! \brief ����˵�������캯��
		 * 	@param[in] id : �ź�����key
		 * 	@return �� 
		 */
		TSem(int id);
		~TSem();
		
	public:
		/*! \brief ����˵��������
		 * 	@param[in] id : �ź�����key
		 * 	@return : �ɹ�true��ʧ��false 
		 */
		bool lock();
		
		/*! \brief ����˵��������
		 * 	@param[in] id : �ź�����key
		 * 	@return : �ɹ�true��ʧ��false 
		 */
		bool unlock();

		int getval();
		
		/*! \brief ����˵���������ź���
		 * 	@return : �ɹ�true��ʧ��false 
		 */
		bool lnkSem();

		/*! \brief ����˵�����½��ź���
		 * 	@return : �ɹ�true��ʧ��false 
		 */
		bool crtSem();

		/*! \brief ����˵����ɾ���ź���
		 * 	@return : �ɹ�true��ʧ��false 
		 */
		bool delSem();
};


#endif
