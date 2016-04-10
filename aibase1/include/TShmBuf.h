///////////////////////////////////////////////////////////////////////////////////////////////////
// 
//     File name:      	TShmBuf.h
//     Author:         	vincet
//     Version:       	01.01.001 
//     Date:
//     Description:     �����ڴ����黺����
// 
//     Others:         // �������ݵ�˵��
//     Function List:  // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
//          1....History:        // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
//                               // �߼��޸����ݼ���  
//              1. Date:
//                 Author:
//                 Modification:
//              
//          2....
// 
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TSHM_BUFFER_H
#define __TSHM_BUFFER_H

#include "THash.h"
#include "TShareMem.h"

using namespace std;

typedef struct {
	int data_size; /* ��������С */
	int used_size; /* ��������ʹ�ô�С */
	int buf_size;  /* ������ռ�ڴ��С */
} SHM_BUF_HEAD;


template <typename T>
class	TShmBuf
{
protected:
		T *m_Iter; /* ����ָ�� */
		T *m_Buffer; /* ����Buffer */
		SHM_BUF_HEAD *m_MemHead; /* �ڴ�����ͷ */
		
		//##Documentation
		//## �����ڴ������
		TShareMem        m_Share;
		
		//##Documentation
		//## �����ڴ��ֵ
		int              m_Key;
			

	/* !\brief ���캯��
	 */
	public:
		TShmBuf();
		virtual ~TShmBuf();


	private:
		TShmBuf(const TShmBuf& right);


	
	/* !\brief ���ݷ��ʿ���
	 */
	public:
		//##Documentation
		//## ��ʼ��
		virtual int     Init(int key);

		//##Documentation
		//##  ���仺�����ռ�
		virtual int     MallocBuffer(int key,int nRecSize = 2048);

		//##Documentation
		//##  ɾ���������ռ�
		virtual int		DelBuffer();

		//##Documentation
		//##  �����Դ
		virtual void    Clear(void );


	//  ���ݷ���
	public:

		//##Documentation
		//##  �����ڵ�
		virtual int Add( T *pNode );
		
		//##Documentation		
		//##  �����������
		virtual int	Read(int _index, T *&pNode );
		
		//##Documentation		
		//##  ��ȡ���������һ��Ԫ�أ���ɾ��		
		virtual int PopBack(T &pNode);


	//##  ����ִ��
	public:
		//##Documentation
		//##  ��ʹ�ÿռ�
		int     Size() ;

		//##Documentaion
		//##  �ܵĿռ���
		void     GetAllSize(int &buff_size,int &data_size,int &used_size) ;

		//##Documentation
		//##  Ͱ������λ
		void    Reset( T *&pNode );
	
};

template <typename T>
TShmBuf<T>::TShmBuf()
{
	m_Buffer = NULL;
	m_MemHead = NULL;
	m_Key = -1;
}

template <typename T>
TShmBuf<T>::~TShmBuf()
{
	m_Share.f_unlinkMem();
}

template <typename T>
int	TShmBuf<T>::Init(int key)
{
	m_Key = key;
	void *pBuf=m_Share.f_getMem(m_Key ,0);
	if(pBuf==NULL || pBuf==(void *)-1)
		return -1;
	
	m_MemHead = (SHM_BUF_HEAD*)pBuf;

	m_Buffer = (T   *)((char*)pBuf + sizeof(SHM_BUF_HEAD));
			
	return 0;
}

template <typename T>
void	TShmBuf<T>::Clear( void )
{
	memset(m_Buffer,0,sizeof(T)*(m_MemHead->data_size));
	
	m_MemHead->used_size = 0;
	
}

template <typename T>
int     TShmBuf<T>::Add( T *pNode )
{
	//  �����ڵ�
	if(m_MemHead->used_size >= m_MemHead->data_size)
	{
		cout<<"��������!!"<<endl;
	 	return -1;
	}
	
	T *pNewNode = m_Buffer + m_MemHead->used_size;
	
	memcpy(pNewNode,pNode,sizeof(T));

	m_MemHead->used_size++;
	return 0;
}

template <typename T>
int	TShmBuf<T>::Read(int _index, T *&pNode ) 
{
	if(_index >= m_MemHead->used_size)
	 	return -1;

	pNode = m_Buffer + _index;
	
	return 0;
}

template <typename T>
int	TShmBuf<T>::PopBack(T &pNode ) 
{
	if(m_MemHead->used_size <= 0)
	 	return -1;

	m_MemHead->used_size--;
	memcpy(&pNode,m_Buffer + m_MemHead->used_size,sizeof(T));
			
	return 0;
}

template <typename T>
int	TShmBuf<T>::Size()
{
	//  ��ʹ�ÿռ�
	return m_MemHead->used_size;	
}


template <typename T>
void TShmBuf<T>::GetAllSize(int &buff_size,int &data_size,int &used_size)
{
	//  �ܹ��Ŀռ�
	buff_size = m_MemHead->buf_size;
	data_size = m_MemHead->data_size;
	used_size = m_MemHead->used_size;

	return;
}

template <typename T>
int TShmBuf<T>::MallocBuffer(int key,int nRecSize)
{
	m_Key = key;
	
	//  ���黺��������Ŀռ�
	if(nRecSize == 0)
		nRecSize = 1000;
	int buf_size = sizeof(SHM_BUF_HEAD)+sizeof(T)*nRecSize;
	void *pBuf = m_Share.f_getMem(m_Key,buf_size);
	if(pBuf == NULL)
	{
		cout<<"�����ڴ����ʧ��"<<endl;
		return -1;
	}
	memset(pBuf,0,buf_size);
	
	m_MemHead = (SHM_BUF_HEAD *)pBuf;
	
	m_MemHead->buf_size = buf_size;
	
	m_MemHead->data_size = nRecSize;
	m_MemHead->used_size = 0;
	
	m_Buffer = (T  *)((char*)pBuf + sizeof(SHM_BUF_HEAD));
	
	return 0;
}

template <typename T>
int TShmBuf<T>::DelBuffer()
{
	// ɾ��ռ�õĹ����ڴ�ռ�
	m_Share.f_delMem();	
	return 0;
}

#endif
