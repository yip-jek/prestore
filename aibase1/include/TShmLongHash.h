#ifndef __T_SHM_LONG_HASH_H
#define __T_SHM_LONG_HASH_H

#include <string.h>
#include <iostream>
#include "TShareMem.h"

inline unsigned long long my_10n(unsigned int y)
{
	unsigned long long t = 1;
	while(y--)
		t *= 10;
	return t;
}

unsigned long long Atoll(const char *s)
{
	unsigned long long x = 0;
	int len = 0;
	while(*s && *s <= '9' && *s >= '0') {
		s++;
		len++;
	}
	s -= len;
	len--;
	while(*s && *s <= '9' && *s >= '0') {
		x += ((unsigned long long)(*s - '0')) * my_10n(len);
		s++;
		len--;
	}
	return x;
}

using namespace std;

/* sizeof(SHM_LONG_HASH_STRU) = 13 + sizeof(T) */

template <typename T>
struct SHM_LONG_HASH_STRU
{
	unsigned long long name;
	int	next;
	T value;
	bool used_flag;
};

typedef struct
{
	int hash_size; //哈希表大小
	int data_size; //数据区大小
	int used_size; //数据区已使用大小
	int total_size; //总共大小
	int buf_size; //缓冲区占内存大小
}SHM_LONG_HASH_HEAD;

template <typename T>
class	TShmLongHash
{
protected:
	SHM_LONG_HASH_STRU<T> *m_pNode, *m_Iter, *h_Iter;
	SHM_LONG_HASH_STRU<T>      *m_Buffer;
	SHM_LONG_HASH_HEAD         *m_MemHead;
	unsigned int	 m_hash_begin;
	TShareMem        m_Share;
	int              m_Key;
public:
	TShmLongHash();
	virtual ~TShmLongHash();
private:
	TShmLongHash(const TShmLongHash& right);
public:
	virtual int     Init(int key);
	virtual int     MallocBuffer(int key,int nHashSize = 1024,int nRecSize = 2048);
	virtual int		DelBuffer();
	virtual void    Clear(void );
public:
	int     Delete( SHM_LONG_HASH_STRU<T> *pNode);
	virtual int Find(unsigned long long key, unsigned int hash_value,
			SHM_LONG_HASH_STRU<T> *&pNode);
public:
	virtual int Add(unsigned long long key, T *pNode);
	virtual int Add(const char *s, T *pNode);
	virtual int Remove(unsigned long long key);
	virtual int Find(unsigned long long key, T *&pNode);
	virtual int Find(const char *s, T *&pNode);
public:
	int     Next(unsigned long long key, T *&pNode);
	int     Size();
	void     GetAllSize(int &hash_size,int &data_size,int &used_size,int &total_size,int &buf_size);
	void    Begin(void);
	int     ReadNext(unsigned long long key, T *&pNode);
	void    Reset();
	int     Next(T *&pNode, unsigned long long *key);
};

template <typename T>
TShmLongHash<T>::TShmLongHash()
{
	m_pNode = m_Iter = h_Iter = NULL;
	m_Buffer = NULL;
	m_MemHead = NULL;
	m_hash_begin = ~0;
}

template <typename T>
TShmLongHash<T>::~TShmLongHash()
{
	m_Share.f_unlinkMem();
}

template <typename T>
int	TShmLongHash<T>::Init(int key)
{
	m_Key = key;
	void *pBuf=m_Share.f_getMem(m_Key ,0);
	if(pBuf==NULL || pBuf==(void *)-1)
		return -1;
	m_MemHead = (SHM_LONG_HASH_HEAD*)pBuf;
	m_Buffer = (SHM_LONG_HASH_STRU<T>   *)((char*)pBuf + sizeof(SHM_LONG_HASH_HEAD));
	return 0;
}


template <typename T>
void	TShmLongHash<T>::Clear( void )
{
	memset(m_Buffer,0,sizeof(SHM_LONG_HASH_STRU<T>)*(m_MemHead->hash_size+m_MemHead->data_size));
	
	m_MemHead->used_size = 0;
	
	m_MemHead->total_size = 0;	
}

template <typename T>
int	TShmLongHash<T>::Delete( SHM_LONG_HASH_STRU<T> *pNode )
{
	pNode->used_flag = false;
	m_MemHead->total_size--;
	return 0;	
}

template <typename T>
int TShmLongHash<T>::Add(const char *s, T *pNode )
{
	return Add(Atoll(s), pNode);
}

template <typename T>
int TShmLongHash<T>::Add(unsigned long long key, T *pNode )
{
	unsigned long long hash_value = key;
	hash_value %= m_MemHead->hash_size;

	SHM_LONG_HASH_STRU<T> *pLastNode = m_Buffer+hash_value;
	SHM_LONG_HASH_STRU<T> *pNewNode = NULL;
	if( pLastNode->used_flag ) {
		while( pLastNode->next != 0 ) {
			if( ! pLastNode->used_flag ) {
				pNewNode = pLastNode;
				break;				
			} else {
				pLastNode = m_Buffer + pLastNode->next;
			}
		}
		
		if(pNewNode == NULL) {
			if(m_MemHead->used_size >= m_MemHead->data_size) {
				cout<<"Buffer Size is Full!!"<<endl;
				return -1;
			}
			pNewNode = m_Buffer + m_MemHead->hash_size + m_MemHead->used_size;
			pLastNode->next = m_MemHead->hash_size + m_MemHead->used_size;	
			m_MemHead->used_size++;
			m_MemHead->total_size++;
		}
	} else {
		pNewNode = pLastNode;
		m_MemHead->total_size++;
	}
	
	pNewNode->name = key;
	memcpy(&(pNewNode->value),pNode,sizeof(T));
	pNewNode->used_flag = true;

	return 0;
}


template <typename T>
int	TShmLongHash<T>::Remove(unsigned long long key)
{
	unsigned long long hash_value = key % m_MemHead->hash_size;
	int nRet = Find(key, (unsigned int)hash_value, m_pNode );
	if( nRet < 0 ) {
		return -1;
	}
	return Delete( m_pNode );
}


template <typename T>
int	TShmLongHash<T>::Find(unsigned long long key, unsigned int hash_value, SHM_LONG_HASH_STRU<T> *&pNode )
{
	pNode = m_Buffer + hash_value;
	while(true) {
		if(pNode->name == key && pNode->used_flag ) {
			return 0;
		}
		if(pNode->next == 0)
			break;
		pNode = m_Buffer+pNode->next;
	}
	pNode = NULL;
	return -1;
}

template <typename T>
int	TShmLongHash<T>::Find(unsigned long long key, T *&pNode )
{
	unsigned long long hash_value = key ;
	hash_value %= m_MemHead->hash_size;
	int nRet = Find(key, (unsigned int)hash_value, m_pNode );
	if( nRet < 0 ) { 
		h_Iter = NULL;
		return -1; 
	}
	/* !\brief 查找成功
	 */	
	pNode = &(m_pNode->value);
	h_Iter = m_pNode; return 0;
}

template <typename T>
int	TShmLongHash<T>::Find(const char *s, T *&pNode )
{
	return Find(Atoll(s), pNode);
}

template <typename T>
void	TShmLongHash<T>::Begin() 
{
	h_Iter = NULL;
}

template <typename T>
int	TShmLongHash<T>::ReadNext(unsigned long long key, T *&pNode) 
{
	if( h_Iter == NULL ) { 
		return Find(s,pNode);
	}
	
	SHM_LONG_HASH_STRU<T> *pOrig = h_Iter; 
	while(pOrig->next != 0) {
		pOrig = m_Buffer + pOrig->next;
		if(pOrig->name == key && pOrig->used_flag ) {
			h_Iter = pOrig;
			pNode  = &(pOrig->value);
			return 0;
		}
	}
	h_Iter = NULL;
	return -1;
}

template <typename T>
int	TShmLongHash<T>::Next(unsigned long long key, T *&pNode ) 
{
	if(h_Iter == NULL) {
		return 1;
	}
	SHM_LONG_HASH_STRU<T> *pOrig = h_Iter; 
	while(pOrig->next != 0) {
		pOrig = m_Buffer + pOrig->next;
		if(pOrig->name == key  && pOrig->used_flag) {
			h_Iter = pOrig;
			pNode  = &(pOrig->value);
			return 0;
		}
	}
	h_Iter = NULL;
	return 1;
}

template <typename T>
void TShmLongHash<T>::Reset() 
{
	h_Iter = m_Buffer;
}

template <typename T>
int	TShmLongHash<T>::Next(T *&pNode, unsigned long long *key) 
{
	while(h_Iter < m_Buffer+m_MemHead->buf_size) {
		if(h_Iter->used_flag)
			break;
		h_Iter++;
	}
	if(h_Iter >= m_Buffer+m_MemHead->buf_size) //已经遍历完毕
		return -1;
	pNode = &(h_Iter->value);
	*key = h_Iter->name;
	h_Iter++;
	return 0;
}

template <typename T>
int	TShmLongHash<T>::Size()
{
	return m_MemHead->total_size;	
}


template <typename T>
void TShmLongHash<T>::GetAllSize(int &hash_size,int &data_size,int &used_size,int &total_size,int &buf_size)
{
	hash_size = m_MemHead->hash_size;
	data_size = m_MemHead->data_size;
	used_size = m_MemHead->used_size;
	total_size = m_MemHead->total_size;
	buf_size = m_MemHead->buf_size;

	return;
}

template <typename T>
int TShmLongHash<T>::MallocBuffer(int key, int nHashSize, int nRecSize)
{
	m_Key = key;
	
	/* !\brief 建议缓冲区分配的空间
	*/
	if(nRecSize < 10240)
		nRecSize = 10240;
		
	if(nHashSize < 10240)
		nHashSize = 10240;
		
	int _size = nRecSize*1.3;
	int buf_size = sizeof(SHM_LONG_HASH_HEAD)+sizeof(SHM_LONG_HASH_STRU<T>)*_size;
	void *pBuf = m_Share.f_getMem(m_Key,buf_size);
	if(pBuf == NULL) {
		cout<<"共享内存分配失败"<<endl;
		return -1;
	}
	
	memset(pBuf,0,buf_size);
	
	m_MemHead = (SHM_LONG_HASH_HEAD *)pBuf;
	
	m_MemHead->buf_size = _size;
	
	m_MemHead->hash_size = nHashSize*0.6;
	m_MemHead->data_size = _size - m_MemHead->hash_size;
	m_MemHead->used_size = 0;
	m_MemHead->total_size = 0;
	
	m_Buffer = (SHM_LONG_HASH_STRU<T>   *)((char*)pBuf + sizeof(SHM_LONG_HASH_HEAD));
	return 0;
}

template <typename T>
int TShmLongHash<T>::DelBuffer()
{
	//删除占用的共享内存空间
	m_Share.f_delMem();	
	
	return 0;
}

#endif
