///////////////////////////////////////////////////////////////////////////////////////////////////
// 
//     File name:      	TShmBuf.h
//     Author:         	vincet
//     Version:       	01.01.001 
//     Date:
//     Description:     共享内存数组缓存区
// 
//     Others:         // 其它内容的说明
//     Function List:  // 主要函数列表，每条记录应包括函数名及功能简要说明
//          1....History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
//                               // 者及修改内容简述  
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
	int data_size; /* 数据区大小 */
	int used_size; /* 数据区已使用大小 */
	int buf_size;  /* 缓冲区占内存大小 */
} SHM_BUF_HEAD;


template <typename T>
class	TShmBuf
{
protected:
		T *m_Iter; /* 处理指针 */
		T *m_Buffer; /* 数组Buffer */
		SHM_BUF_HEAD *m_MemHead; /* 内存数据头 */
		
		//##Documentation
		//## 共享内存访问类
		TShareMem        m_Share;
		
		//##Documentation
		//## 共享内存键值
		int              m_Key;
			

	/* !\brief 构造函数
	 */
	public:
		TShmBuf();
		virtual ~TShmBuf();


	private:
		TShmBuf(const TShmBuf& right);


	
	/* !\brief 数据访问控制
	 */
	public:
		//##Documentation
		//## 初始化
		virtual int     Init(int key);

		//##Documentation
		//##  分配缓冲区空间
		virtual int     MallocBuffer(int key,int nRecSize = 2048);

		//##Documentation
		//##  删除缓冲区空间
		virtual int		DelBuffer();

		//##Documentation
		//##  清除资源
		virtual void    Clear(void );


	//  数据访问
	public:

		//##Documentation
		//##  新增节点
		virtual int Add( T *pNode );
		
		//##Documentation		
		//##  随机访问数组
		virtual int	Read(int _index, T *&pNode );
		
		//##Documentation		
		//##  读取数组中最后一个元素，并删除		
		virtual int PopBack(T &pNode);


	//##  函数执行
	public:
		//##Documentation
		//##  已使用空间
		int     Size() ;

		//##Documentaion
		//##  总的空间数
		void     GetAllSize(int &buff_size,int &data_size,int &used_size) ;

		//##Documentation
		//##  桶查找置位
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
	//  新增节点
	if(m_MemHead->used_size >= m_MemHead->data_size)
	{
		cout<<"缓冲区满!!"<<endl;
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
	//  已使用空间
	return m_MemHead->used_size;	
}


template <typename T>
void TShmBuf<T>::GetAllSize(int &buff_size,int &data_size,int &used_size)
{
	//  总共的空间
	buff_size = m_MemHead->buf_size;
	data_size = m_MemHead->data_size;
	used_size = m_MemHead->used_size;

	return;
}

template <typename T>
int TShmBuf<T>::MallocBuffer(int key,int nRecSize)
{
	m_Key = key;
	
	//  建议缓冲区分配的空间
	if(nRecSize == 0)
		nRecSize = 1000;
	int buf_size = sizeof(SHM_BUF_HEAD)+sizeof(T)*nRecSize;
	void *pBuf = m_Share.f_getMem(m_Key,buf_size);
	if(pBuf == NULL)
	{
		cout<<"共享内存分配失败"<<endl;
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
	// 删除占用的共享内存空间
	m_Share.f_delMem();	
	return 0;
}

#endif
