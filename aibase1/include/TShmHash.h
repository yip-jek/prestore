/*
 *
 * File name:      	TShmHash.h
 * Author:         	vincet
 */

#ifndef __T_SHM_HASH_H
#define __T_SHM_HASH_H

#include "THash.h"
#include "TShareMem.h"

//#include "Logger.h"

#ifndef SHM_HASH_KEY_LEN
#define SHM_HASH_KEY_LEN 32
#endif

/* sizeof(SHM_HASH_STRU) = 37 + sizeof(T) */

template <typename T>
struct SHM_HASH_STRU
{
	char name[SHM_HASH_KEY_LEN];
	int	next;
	T value;
	bool used_flag;
};

typedef struct
{
	int re_malloc_flag; /* = 1 re_malloc */
	int hash_size; 		/* 哈希表大小 */
	int data_size; 		/* 数据区大小 */
	int used_size;		/* 数据区已使用大小 */
	int total_size;		/* 总共使用记录数 */
	int buf_size; 		/* 最大记录数 */
}SHM_HASH_HEAD;

/** \defgroup srbc libshm 
 */

/** \file TShmHash.h
 *  \brief 用哈希算法管理共享内存类的头文件
 */

/** \class TShmHash
 *  \brief 用哈希算法管理共享内存类
 */

/** \example exTShmHashld.cpp
 * 	\example exTShmHash.cpp
 *  \brief TShmHash类示例程序 
 */

template <typename T>
class	TShmHash
{
	protected:
		SHM_HASH_STRU<T>      *m_pNode,*m_Iter,*h_Iter;
		SHM_HASH_STRU<T>      *m_Buffer;
		SHM_HASH_HEAD         *m_MemHead;
		unsigned int	 m_hash_begin;
		TShareMem        m_Share;
		int              m_Key;
		int m_mode;
	public:
		TShmHash();
		~TShmHash();
	private:
		TShmHash(const TShmHash& right);
	public:
		/*! \brief 功能说明：连接共享内存
		 * 	@param[in] key : 共享内存key
		 * 	@param[in] mode: 共享内存mode,默认SHARE_SHM_PERM
		 * 	@return : 返回0表示连接成功，-1连接失败
		 */
		int     Init(int key, int mode = SHARE_SHM_PERM);


		/*! \brief 功能说明：分配共享内存并初始化hash结构
		 * 	@param[in] key : 共享内存key
		 * 	@param[in] nHashSize: 
		 * 	@param[in] nRecSize: 要分配的共享内存的大小
		 * 	@return : 返回0表示成功，-1失败
		 */
		int     MallocBuffer(int key,int nHashSize = 1024,int nRecSize = 2048);
		
	
		/*! \brief 功能说明：删除共享内存
		 * 	@return : 返回0表示成功，-1失败
		 */
		int		DelBuffer();


		/*! \brief 功能说明：释放共享内存连接
		 * 	@return : 返回0表示成功，-1失败
		 */
		int 	UnlinkMem();

		
		/*! \brief 功能说明：清空共享内存
		 * 	@return : 返回0表示成功，-1失败
		 */
		int		Clear(void );


		/*! \brief 功能说明：共享内存是否重新分配
		 * 	@return : 返回1表示重新分配
		 */
		int     IsReMalloc();

	protected:
		/*! \brief 功能说明：自动扩展共享内存
		 * 	@return : 返回0表示成功，-1失败
		 */
		int     ReMalloc();

	public:
		/*! \brief 功能说明：将{关键字，值}加入hash
		 * 	@param[in] s : 关键字
		 * 	@param[in] pNode: 值
		 * 	@return : 返回0表示成功，-1失败
		 */
		int Add( const char *s, T *pNode );
		

		/*! \brief 功能说明：将关键字节点删除
		 * 	@param[in] s : 关键字
		 * 	@return : 返回0表示成功，-1失败
		 */
		int Remove( const char *s );

		int Remove(T *p);

		
		/*! \brief 功能说明：查找关键字,得到关键字对应值的地址
		 * 	@param[in] s : 关键字
		 * 	@param[out] pNode : 得到关键字对应值的地址（返回成功才能取）
		 * 	@return : 返回0表示成功，-1失败
		 */
		int Find( const char *s, T *&pNode );

		
		/*! \brief 功能说明：遍历指定关键字，使用前必需先Find()出第一个
		 * 	@param[in] s : 关键字
		 * 	@param[out] pNode : 得到关键字对应值的地址（返回成功才能取）
		 * 	@return : 返回0表示成功，-1失败
		 */
		int     Next( const char *s, T *&pNode ) ;


		/*! \brief 功能说明：重置,为ReadNext()做准备
		 * 	@return : 返回0表示成功，-1失败
		 */
		int 	Begin(void) ;

		/*! \brief 功能说明：遍历指定关键字，使用前必需先Begin(void) ;
		 * 	@param[in] s : 关键字
		 * 	@param[out] pNode : 得到关键字对应值的地址（返回成功才能取）
		 * 	@return : 返回0表示成功，-1失败
		 */
		int     ReadNext( const char *s, T *&pNode ) ;

		
		/*! \brief 功能说明：重置，为遍历所有关键字做准备
		 * 	@return : 返回0表示成功，-1失败
		 */
		int		Reset();
		
		/*! \brief 功能说明：遍历所有关键字，使用前必需先Reset()
		 * 	@param[out] pNode : 得到关键字对应值的地址（返回成功才能取）
		 * 	@param[out] key : 得到关键字（返回成功才能取）
		 * 	@return : 返回0表示成功，-1失败
		 */
		int     Next( T *&pNode ,char *Key = NULL);


		int     Size() ;
		int		GetAllSize(int &hash_size,int &data_size,int &used_size,int &total_size,int &buf_size);
	
	protected:
		/*! \brief 功能说明：得到关键字的hash值
		 * 	@return : 返回关键字的hash值
		 */
		unsigned int make_hash( unsigned char *s,int len,unsigned int hash );	
		
		
		/*! \brief 功能说明：删除节点
		 * 	@param[in] pNode : 关键字
		 * 	@return : 返回0表示成功，-1失败
		 */
		int     Delete( SHM_HASH_STRU<T> *pNode );

		
		/*! \brief 功能说明：查找关键字节点
		 * 	@param[in] s : 关键字
		 * 	@param[in] hash_value : 关键字hash值
		 * 	@param[out] pNode : 节点地址（返回成功才能取）
		 * 	@return : 返回0表示成功，-1失败
		 */
		int Find( const char *s, unsigned int hash_value, SHM_HASH_STRU<T> *&pNode );
};

template <typename T>
TShmHash<T>::TShmHash()
{
	m_pNode = m_Iter = h_Iter = NULL;
	m_Buffer = NULL;
	m_MemHead = NULL;
	m_hash_begin = ~0;
	
}

template <typename T>
TShmHash<T>::~TShmHash()
{
	m_Share.f_unlinkMem();
}

template <typename T>
unsigned int TShmHash<T>::make_hash( unsigned char *s,int len,unsigned int hash )
{

	while (len-- > 0)
		hash = hash_crctab32[(unsigned)(((int)hash ^ (*s++)) & 0xff)] ^ (hash >> 8);

	return hash;
}

template <typename T>
int	TShmHash<T>::Init(int key, int mode)
{
	m_Key = key;
	m_mode = mode;
	void *pBuf=m_Share.f_getMem(m_Key ,0, mode);
	if(pBuf==NULL || pBuf==(void *)-1)
		return -1;
	
	m_MemHead = (SHM_HASH_HEAD*)pBuf;

	m_Buffer = (SHM_HASH_STRU<T>   *)((char*)pBuf + sizeof(SHM_HASH_HEAD));
			
	return 0;
}

template <typename T>
int TShmHash<T>::IsReMalloc()
{
	return m_MemHead->re_malloc_flag;
}

template <typename T>
int 	TShmHash<T>::Clear( void )
{
	if(m_MemHead->re_malloc_flag == 1) {
		UnlinkMem();
		if(Init(m_Key, m_mode) == -1) {
			return -1;
		}
	}

	memset(m_Buffer,0,sizeof(SHM_HASH_STRU<T>)*(m_MemHead->hash_size+m_MemHead->data_size));
	
	m_MemHead->used_size = 0;
	
	m_MemHead->total_size = 0;	

	return 0;
}

template <typename T>
int	TShmHash<T>::Delete( SHM_HASH_STRU<T> *pNode )
{
	pNode->used_flag = false;
	m_MemHead->total_size--;
	return 0;	
}

template <typename T>
int TShmHash<T>::Add( const char *s, T *pNode )
{
	if(m_MemHead->re_malloc_flag == 1) {
		UnlinkMem();
		if(Init(m_Key, m_mode) == -1) {
			return -1;
		}
	}
	unsigned int hash_value = make_hash( (unsigned char *)s, strlen(s), m_hash_begin );
	hash_value%= m_MemHead->hash_size;
	SHM_HASH_STRU<T> *pLastNode = m_Buffer+hash_value;
	SHM_HASH_STRU<T> *pNewNode = NULL;
	if(pLastNode->used_flag) {
		while(pLastNode->next != 0 ) {
			if(!pLastNode->used_flag ) {
				pNewNode = pLastNode;
				break;				
			} else {
				pLastNode = m_Buffer + pLastNode->next;
			}
		}
		
		if(pNewNode == NULL) {
			//新增加一个节点
			if(m_MemHead->used_size >= m_MemHead->data_size) {
				if(ReMalloc() == -1)
					return -1;
				return Add(s, pNode);
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
	
	strcpy(pNewNode->name,s);
	memcpy(&(pNewNode->value),pNode,sizeof(T));
	pNewNode->used_flag = true;

	return 0;
}


template <typename T>
int	TShmHash<T>::Remove( const char *s)
{
	if(m_MemHead->re_malloc_flag == 1) {
		UnlinkMem();
		if(Init(m_Key, m_mode) == -1) {
			return -1;
		}
	}

	unsigned int
			hash_value = make_hash( (unsigned char *)s, strlen(s), m_hash_begin );
	hash_value%= m_MemHead->hash_size;
	int nRet = Find( s, hash_value, m_pNode );
	if( nRet < 0 ) {
		return -1;
	}
	return Delete( m_pNode );
}

template <typename T>
int	TShmHash<T>::Find( const char *s, unsigned int hash_value, SHM_HASH_STRU<T> *&pNode )
{
	pNode = m_Buffer + hash_value;
	while(true) {
		if(strcmp(pNode->name, s) == 0 && pNode->used_flag) {
			return 0;
		}
		if(pNode->next == 0)
			break;
		pNode = m_Buffer+pNode->next;
	};
	pNode = NULL;
	return -1;
}

template <typename T>
int	TShmHash<T>::Find( const char *s, T *&pNode )
{
	if(m_MemHead->re_malloc_flag == 1) {
		UnlinkMem();
		if(Init(m_Key, m_mode) == -1) {
			return -1;
		}
	}
	unsigned int
			hash_value = make_hash( (unsigned char *)s, strlen(s), m_hash_begin );
	hash_value %= m_MemHead->hash_size;

	int nRet = Find(s, hash_value, m_pNode);
	if( nRet < 0 ) { 
		h_Iter = NULL;
		return -1; 
	}
	pNode = &(m_pNode->value);
	h_Iter = m_pNode;
	return 0;
}

template <typename T>
int TShmHash<T>::Begin() 
{
	if(m_MemHead->re_malloc_flag == 1) {
		UnlinkMem();
		if(Init(m_Key, m_mode) == -1) {
			return -1;
		}
	}

	h_Iter = NULL;
	return 0;
}

template <typename T>
int	TShmHash<T>::ReadNext(const char *s, T *&pNode) 
{
	if(h_Iter == NULL) { 
		return Find(s,pNode);
	}
	SHM_HASH_STRU<T> *pOrig = h_Iter; 
	while(pOrig->next != 0) {
		pOrig = m_Buffer + pOrig->next;
		if( strcmp(pOrig->name, s) == 0 && pOrig->used_flag ) {
			h_Iter = pOrig;
			pNode  = &(pOrig->value);
			return 0;
		}
	}
	h_Iter = NULL;
	return -1;
}

template <typename T>
int	TShmHash<T>::Next( const char *s, T *&pNode ) 
{
	if(h_Iter == NULL)
		return -1;
	SHM_HASH_STRU<T> *pOrig = h_Iter; 
	while(pOrig->next != 0) {
		pOrig = m_Buffer + pOrig->next;
		if( strcmp(pOrig->name, s) == 0 && pOrig->used_flag ) {
			h_Iter = pOrig;
			pNode  = &(pOrig->value);
			return 0;
		}
	}
	h_Iter = NULL;
	return -1;
}

template <typename T>
int TShmHash<T>::Reset( ) 
{
	if(m_MemHead->re_malloc_flag == 1) {
		UnlinkMem();
		if(Init(m_Key, m_mode) == -1) {
			return -1;
		}
	}

	h_Iter = m_Buffer;
	return 0;
}

template <typename T>
int	TShmHash<T>::Next(T *&pNode,char *Key) 
{
	while(h_Iter < m_Buffer+m_MemHead->buf_size) {
		if(h_Iter->used_flag)
			break;
		h_Iter++;
	}
	
	if(h_Iter >= m_Buffer+m_MemHead->buf_size) //已经遍历完毕
		return -1;
		
	pNode = &(h_Iter->value);
	
	if(Key!=NULL)
		strcpy(Key,h_Iter->name);
		
	h_Iter++;
	return 0;
}


template <typename T>
int	TShmHash<T>::Size()
{
	if(m_MemHead->re_malloc_flag == 1) {
		UnlinkMem();
		if(Init(m_Key, m_mode) == -1) {
			return -1;
		}
	}

	return m_MemHead->total_size;	
}


template <typename T>
int TShmHash<T>::GetAllSize(int &hash_size,int &data_size,int &used_size,int &total_size,int &buf_size)
{
	if(m_MemHead->re_malloc_flag == 1) {
		UnlinkMem();
		if(Init(m_Key, m_mode) == -1) {
			return -1;
		}
	}

	hash_size = m_MemHead->hash_size;
	data_size = m_MemHead->data_size;
	used_size = m_MemHead->used_size;
	total_size = m_MemHead->total_size;
	buf_size = m_MemHead->buf_size;

	return 0;
}

template <typename T>
int TShmHash<T>::MallocBuffer(int key,int nHashSize,int nRecSize)
{
	m_Key = key;
	if(nRecSize < 10240)
		nRecSize = 10240;
	if(nHashSize < 10240)
		nHashSize = 10240;
	int _size = nRecSize*1.3;
	size_t buf_size = sizeof(SHM_HASH_HEAD)+sizeof(SHM_HASH_STRU<T>)*_size;
	void *pBuf = m_Share.f_getMem(m_Key,buf_size);
	if(pBuf == NULL) {
		return -1;
	}
	
	memset(pBuf,0,buf_size);
	m_MemHead = (SHM_HASH_HEAD *)pBuf;
	m_MemHead->re_malloc_flag = 0;
	m_MemHead->buf_size = _size;
	m_MemHead->hash_size = nHashSize * 0.6;
	m_MemHead->data_size = _size - m_MemHead->hash_size;
	m_MemHead->used_size = 0;
	m_MemHead->total_size = 0;
	
	m_Buffer = (SHM_HASH_STRU<T>   *)((char*)pBuf + sizeof(SHM_HASH_HEAD));
	
	return 0;
}

template <typename T>
int TShmHash<T>::ReMalloc()
{
	char *tmp;
	int old_rec_total, new_rec_total;
	size_t old_buf_size;

#ifdef DL
	DL((APP, "%s:%d Start ReMalloc.\n", __FILE__, __LINE__));
#endif
	SHM_HASH_STRU<T> *buf, *p;
	old_rec_total  = m_MemHead->buf_size;
	old_buf_size = sizeof(SHM_HASH_HEAD)
			+ old_rec_total * sizeof(SHM_HASH_STRU<T>);
	new_rec_total = old_rec_total; 
	if((tmp = (char *)malloc(old_buf_size)) == NULL) {
#ifdef DL
		DL((APP, "%s:%d malloc: %s\n", __FILE__, __LINE__, strerror(errno)));
#endif
		return -1;
	}
	memcpy(tmp, (char *)m_MemHead, old_buf_size);
	DelBuffer();
	buf = (SHM_HASH_STRU<T> *) ((char *)tmp + sizeof(SHM_HASH_HEAD));
	if(MallocBuffer(m_Key, new_rec_total, new_rec_total) == -1) {
		free(tmp);
#ifdef DL
		DL((APP, "%s:%d re malloc error.", __FILE__, __LINE__));
#endif
		return -1;
	}
	int f = 0;
	for(int i = 0; i < old_rec_total; i++) {
		p = &buf[i];
		if(p->used_flag) {
			if(Add(p->name, &(p->value)) == -1) {
				free(tmp);
				return -1;
			}
			f++;
		}
	}
	free(tmp);
#ifdef DL
	DL((APP, "%s:%d ReMalloc done.\n", __FILE__, __LINE__));
#endif
	return 0;
}

template <typename T>
int TShmHash<T>::DelBuffer()
{
	//删除占用的共享内存空间
	m_MemHead->re_malloc_flag = 1;
	m_Share.f_delMem();	
	
	return 0;
}

template <typename T>
int TShmHash<T>::UnlinkMem()
{
	m_Share.f_unlinkMem();
	return 0;
}

template <typename T>
int TShmHash<T>::Remove(T *p)
{
	SHM_HASH_STRU <T> *s;

	s = (SHM_HASH_STRU<T> *)((char *)p - ((char *)&((SHM_HASH_STRU<T> *)0)->value));

	return Delete(s);
}

#endif

