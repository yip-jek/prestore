/*
 * $Id: TShmMd5Hash.h,v 1.5 2007/05/10 07:39:49 qsh Exp $
 * Author: vincent, hace
 */
#ifndef __T_SHM_MD5_HASH_H
#define __T_SHM_MD5_HASH_H

#include <string.h>
#include <iostream>
#include "THash.h"
#include "TShareMem.h"
#include "md5_.h"

using namespace std;

/* !\brief 哈希表节点
 */

template <typename T>
struct MD5_HASH_STRU
{
	unsigned char name[MD5_LEN];
	int next;
	T value;
	bool used_flag;
};

typedef struct
{
	//哈希表大小
	long hash_size;
	//数据区大小
	long data_size;
	//数据区已使用大小
	long used_size;
	//总共大小
	long total_size;
	//缓冲区占内存大小
	long buf_size;
}MD5_HASH_HEAD;

//##Documentation
//## 共享内存哈希容器
template <typename T>
class	TShmMd5Hash
{
	protected:
		MD5_HASH_STRU<T>      *m_pNode,*m_Iter,*h_Iter;
		MD5_HASH_STRU<T>      *m_Buffer;
		MD5_HASH_HEAD         *m_MemHead;
		unsigned long m_hash_begin;
		TShareMem        m_Share;
		key_t m_Key;
		unsigned char m_md5sum[MD5_LEN];
	public:
		TShmMd5Hash();
		virtual ~TShmMd5Hash();
	private:
		TShmMd5Hash(const TShmMd5Hash& right);
	public:
		virtual int     Init(key_t key);
		virtual int     MallocBuffer(key_t key, size_t nHashSize = 1024, size_t nRecSize = 2048);
		virtual int		DelBuffer();
		virtual void    Clear(void );
	public:
		int     Delete( MD5_HASH_STRU<T> *pNode );
		unsigned long make_hash( unsigned char *s, int len, unsigned long hash );	
		virtual int Find( const char *s, unsigned long hash_value, MD5_HASH_STRU<T> *&pNode );
	public:
		virtual int Add( const char *s, T *pNode );
		virtual int Remove( const char *s );
		virtual int Find( const char *s, T *&pNode );
	public:
		int     Next( const char *s, T *&pNode ) ;
		int     Size() ;
		void     GetAllSize(size_t &hash_size, size_t &data_size, size_t &used_size,size_t &total_size,size_t &buf_size);
		void    Begin(void) ;
		int     ReadNext( const char *s, T *&pNode ) ;
		void    Reset();
		int     Next( T *&pNode ,char *Key = NULL);
};

template <typename T>
TShmMd5Hash<T>::TShmMd5Hash()
{
	m_pNode = m_Iter = h_Iter = NULL;
	m_Buffer = NULL;
	m_MemHead = NULL;
	m_hash_begin = ~0;
}

template <typename T>
TShmMd5Hash<T>::~TShmMd5Hash()
{
	m_Share.f_unlinkMem();
}

template <typename T>
unsigned long TShmMd5Hash<T>::make_hash( unsigned char *s,int len,unsigned long hash )
{
	while (len-- > 0)
		hash = hash_crctab32[(unsigned)(((long)hash ^ (*s++)) & 0xff)] ^ (hash >> 8);
	return hash;
}

template <typename T>
int	TShmMd5Hash<T>::Init(key_t key)
{
	m_Key = key;
	void *pBuf=m_Share.f_getMem(m_Key ,0);
	if(pBuf==NULL || pBuf==(void *)-1)
		return -1;
	m_MemHead = (MD5_HASH_HEAD*)pBuf;
	m_Buffer = (MD5_HASH_STRU<T>   *)((char*)pBuf + sizeof(MD5_HASH_HEAD));
	return 0;
}

template <typename T>
void	TShmMd5Hash<T>::Clear( void )
{
	memset(m_Buffer,0,sizeof(MD5_HASH_STRU<T>)*(m_MemHead->hash_size+m_MemHead->data_size));
	
	m_MemHead->used_size = 0;
	
	m_MemHead->total_size = 0;	
}

template <typename T>
int	TShmMd5Hash<T>::Delete( MD5_HASH_STRU<T> *pNode )
{
	pNode->used_flag = false;
	m_MemHead->total_size--;
	return 0;	
}

template <typename T>
int     TShmMd5Hash<T>::Add( const char *s, T *pNode )
{

	/* !\brief 计算哈希值进行数据访问
	 */
	unsigned long hash_value = make_hash( (unsigned char *)s, strlen(s), m_hash_begin );
	hash_value%= m_MemHead->hash_size;


	
	/* !\brief 新增节点
	 */
	MD5_HASH_STRU<T> *pLastNode = m_Buffer+hash_value;
	MD5_HASH_STRU<T> *pNewNode = NULL;
	if( pLastNode->used_flag )
	{
		while( pLastNode->next != 0 )
		{
			if( ! pLastNode->used_flag )
			{
				pNewNode = pLastNode;
				break;				
			}
			else
			{
				pLastNode = m_Buffer + pLastNode->next;
			}
		}
		
		if(pNewNode == NULL)
		{
			//新增加一个节点
			if(m_MemHead->used_size >= m_MemHead->data_size)
			{
				cout<<"Buffer Size is Full!!"<<endl;
				return -1;
			}
		
			pNewNode = m_Buffer + m_MemHead->hash_size + m_MemHead->used_size;
			pLastNode->next = m_MemHead->hash_size + m_MemHead->used_size;	
			m_MemHead->used_size++;
			m_MemHead->total_size++;
		}
	}
	else
	{
		pNewNode = pLastNode;
		m_MemHead->total_size++;
	}
	
	memcpy(pNewNode->name, md5sum(s, m_md5sum), MD5_LEN);
	memcpy(&(pNewNode->value), pNode, sizeof(T));
	pNewNode->used_flag = true;

	return 0;
}


template <typename T>
int	TShmMd5Hash<T>::Remove( const char *s)
{
	/* !\brief 计算哈希值进行数据访问
	 */
	unsigned long 
	hash_value = make_hash( (unsigned char *)s, strlen(s), m_hash_begin );
	hash_value%= m_MemHead->hash_size;


	/* !\brief 先查找到该节点, 然后删除
	 */

	int nRet = Find( s, hash_value, m_pNode );
	if( nRet < 0 ) {
		return -1;
	}


	/* !\brief 删除节点
	 */
	return Delete( m_pNode );

}


template <typename T>
int	TShmMd5Hash<T>::Find( const char *s, unsigned long hash_value, MD5_HASH_STRU<T> *&pNode )
{

	/* !\brief 在桶内进行查找
	 */
	
	pNode = m_Buffer + hash_value;
	while(true)
	{
		/* !\brief 相同的 KEY 值
		*/
		if(memcmp( pNode->name, md5sum(s, m_md5sum), MD5_LEN) == 0 && pNode->used_flag )
		{

			/* !\brief 数据回传
		 	*/
			return 0;
		}
		if(pNode->next == 0)
			break;
		pNode = m_Buffer+pNode->next;
	};

	/* !\brief 查找失败
	 */
	pNode = NULL;
	return -1;
}

template <typename T>
int	TShmMd5Hash<T>::Find( const char *s, T *&pNode )
{
	/* !\brief 计算哈希值进行数据访问
	 */
	unsigned long
			hash_value = make_hash( (unsigned char *)s, strlen(s), m_hash_begin );
	hash_value%= m_MemHead->hash_size;


	/* !\brief 进行数据查找
	 */
	int nRet = Find(s, hash_value, m_pNode);
	if( nRet < 0 ) { 

		/* !\brief 查找失败
		 */	
		h_Iter = NULL; return -1; 
	}

	/* !\brief 查找成功
	 */	
	pNode = &(m_pNode->value);
	h_Iter = m_pNode; return 0;
}

template <typename T>
void	TShmMd5Hash<T>::Begin() 
{
	h_Iter = NULL;
}

template <typename T>
int	TShmMd5Hash<T>::ReadNext(const char *s, T *&pNode) 
{
	/* !\brief 开始读取
	 */
	if( h_Iter == NULL ) 
	{ 
		return Find(s,pNode);
	}
	
	/* !\brief 取值迭代
	 */
	MD5_HASH_STRU<T> *pOrig = h_Iter; 
	while(pOrig->next != 0)
	{
		pOrig = m_Buffer + pOrig->next;

		/* !\brief 查找成功
	 	*/
		if(memcmp(pOrig->name, s, MD5_LEN) == 0 && pOrig->used_flag )
		{
			h_Iter = pOrig;

			pNode  = &(pOrig->value);
			return 0;
		}
	}

	/* !\brief 迭代查找成功
	 */
	h_Iter = NULL;  return 1;

}

template <typename T>
int	TShmMd5Hash<T>::Next( const char *s, T *&pNode ) 
{

	/* !\brief 已读取完成
	 */
	if( h_Iter == NULL ) { return 1; }


	/* !\brief 取值迭代
	 */
	MD5_HASH_STRU<T> *pOrig = h_Iter; 
	while(pOrig->next != 0)
	{
		pOrig = m_Buffer + pOrig->next;

		/* !\brief 查找成功
	 	*/
		if( memcmp(pOrig->name, s, MD5_LEN) == 0 && pOrig->used_flag )
		{
			h_Iter = pOrig;

			pNode  = &(pOrig->value);
			return 0;
		}
	}

	/* !\brief 迭代查找成功
	 */
	h_Iter = NULL;  return 1;
}


template <typename T>
void	TShmMd5Hash<T>::Reset( ) 
{
	h_Iter = m_Buffer;
}

template <typename T>
int	TShmMd5Hash<T>::Next(T *&pNode,char *Key) 
{
	while(h_Iter < m_Buffer+m_MemHead->buf_size)
	{
		if(h_Iter->used_flag)
			break;
		h_Iter++;
	}
	
	if(h_Iter >= m_Buffer+m_MemHead->buf_size) //已经遍历完毕
		return -1;
		
	pNode = &(h_Iter->value);
	
	if(Key!=NULL)
		memcpy(Key, h_Iter->name, MD5_LEN);
		
	h_Iter++;
	return 0;
}


template <typename T>
int	TShmMd5Hash<T>::Size()
{
	/* !\brief 已使用空间
	*/
	return m_MemHead->total_size;	
}


template <typename T>
void TShmMd5Hash<T>::GetAllSize(size_t &hash_size,size_t &data_size,size_t &used_size,size_t &total_size,size_t &buf_size)
{
	hash_size = m_MemHead->hash_size;
	data_size = m_MemHead->data_size;
	used_size = m_MemHead->used_size;
	total_size = m_MemHead->total_size;
	buf_size = m_MemHead->buf_size;

	return;
}

template <typename T>
int TShmMd5Hash<T>::MallocBuffer(key_t key, size_t nHashSize, size_t nRecSize)
{
	m_Key = key;
	
	/* !\brief 建议缓冲区分配的空间
	*/
	if(nRecSize < 10240)
		nRecSize = 10240;
		
	if(nHashSize < 10240)
		nHashSize = 10240;
		
	size_t _size = nRecSize*1.3;
	size_t buf_size = sizeof(MD5_HASH_HEAD)+sizeof(MD5_HASH_STRU<T>)*_size;
	void *pBuf = m_Share.f_getMem(m_Key,buf_size);
	if(pBuf == NULL)
	{
		cout<<"共享内存分配失败"<<endl;
		return -1;
	}
	
	memset(pBuf,0,buf_size);
	
	m_MemHead = (MD5_HASH_HEAD *)pBuf;
	
	m_MemHead->buf_size = _size;
	
	m_MemHead->hash_size = nHashSize*0.6;
	m_MemHead->data_size = _size - m_MemHead->hash_size;
	m_MemHead->used_size = 0;
	m_MemHead->total_size = 0;
	
	m_Buffer = (MD5_HASH_STRU<T>   *)((char*)pBuf + sizeof(MD5_HASH_HEAD));
	
	return 0;
}


template <typename T>
int TShmMd5Hash<T>::DelBuffer()
{
	//删除占用的共享内存空间
	m_Share.f_delMem();	
	return 0;
}

#endif
