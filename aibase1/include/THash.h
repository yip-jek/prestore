/*
 * $Id: THash.h,v 1.3 2007/05/11 07:00:38 qsh Exp $
 * Author: vincent, hace
 */

#ifndef __SRBC_THASH_H__
#define __SRBC_THASH_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern unsigned int hash_crctab32[];

using namespace std;

#ifndef HASH_KEY_LEN
#define HASH_KEY_LEN 32
#endif

template <typename T> class SRBC_HASH_NODE {
public:
	SRBC_HASH_NODE <T> *next, *prev, *h_next, *h_prev;
	char name[HASH_KEY_LEN];
	T value;
	unsigned int hash_value;
};


/** \defgroup srbc libshm 
 */

/** \file THash.h
 *  \brief 哈希类的头文件
 */

/** \class THash
 *  \brief 哈希类
 */

/** \example exTHash.cpp
 *  \brief THash类示例程序 
 */

template <typename T> class THash {
	protected:
		SRBC_HASH_NODE <T> *m_pNode, *m_Iter, *h_Iter;
		SRBC_HASH_NODE <T> **m_Brucket;
		SRBC_HASH_NODE <T> *m_begin, *m_end, *m_Orig;
		int m_MaxSize, m_UsedSize;

	protected:
		unsigned int m_hash_begin;

	public:
		THash();
		virtual ~ THash();

	private:
		THash(const THash & right);

	public:
		/*! \brief 功能说明：初始化hash结构
		 * 	@param[in] nSize : hash大小
		 * 	@return : 返回0表示成功，-1失败
		 */
		virtual int Init(int nSize = 1024);


		/*! \brief 功能说明：清空hash
		 * 	@return : 无
		 */
		virtual void Clear(void);

		/*! \brief 功能说明：计算hash值
		 * 	@param[in] s: 关键字
		 * 	@param[in] len: 关键字的长度
		 * 	@param[in] hash: m_hash_begin
		 * 	@return : 返回hash值
		 */
		unsigned int make_hash(unsigned char *s, int len, unsigned int hash);

	protected:
		/*! \brief 功能说明：创建节点
		 * 	@param[in] s : 关键字
		 * 	@param[in] hash_value: hash值
		 * 	@return : 成功返回节点指针，失败NULL
		 */
		SRBC_HASH_NODE <T> *Create(const char *s, unsigned int hash_value);


		/*! \brief 功能说明：删除节点
		 * 	@param[in] pNode : 节点地址
		 * 	@return : 成功返回节点指针，失败NULL
		 */
		int Delete(SRBC_HASH_NODE <T> *pNode);


		/*! \brief 功能说明：查找关键字
		 * 	@param[in] s: 关键字
		 * 	@param[in] hash_value: hash值
		 * 	@param[out] pNode: 值的地址
		 * 	@return : 返回0表示成功，-1失败
		 */
		virtual int Find(const char *s, unsigned int hash_value, T * &pNode);

	public:
		/*! \brief 功能说明：增加{ 关键字:值 }
		 * 	@param[in] s: 关键字
		 * 	@param[in] pNode: 值的地址
		 * 	@return : 返回0表示成功，-1失败
		 */
		virtual int Add(const char *s, T * pNode);


		/*! \brief 功能说明：删除关键字
		 * 	@param[in] s: 关键字
		 * 	@param[out] Node: 值
		 * 	@return : 返回0表示成功，-1失败
		 */
		virtual int Remove(const char *s, T &Node );



		/*! \brief 功能说明：查找关键字
		 * 	@param[in] s: 关键字
		 * 	@param[out] pNode: 值的地址
		 * 	@return : 返回0表示成功，-1失败
		 */
		virtual int Find(const char *s, T * &pNode);

		/*! \brief 功能说明：遍历指定关键字,之前必需先Find()第一个
		 * 	@param[in] s: 关键字
		 * 	@param[out] pNode: 值的地址
		 * 	@return : 返回0表示成功，-1失败
		 */
		int Next(const char *s, T * &pNode);

		
		/*! \brief 功能说明：重置,为ReadNext()做准备
		 * 	@return : 无
		 */
		void Begin();

		/*! \brief 功能说明：遍历指定关键字,之前必需先Begin()
		 * 	@param[in] s: 关键字
		 * 	@param[out] pNode: 值的地址
		 * 	@return : 返回0表示成功，-1失败
		 */
		int ReadNext(const char *s, T * &pNode);


		/*! \brief 功能说明：重置,为遍历所有关键字做准备
		 * 	@return : 无
		 */
		void Reset(void);

		/*! \brief 功能说明：遍历所有关键字,之前必需先Reset(void);
		 * 	@param[out] pNode: 值的地址
		 * 	@param[out] key: 关键字
		 * 	@return : 返回0表示成功，-1失败
		 */
		int Next(T * &pNode, char *key = NULL);


		int Size();
		void Reset(T * &pNode);
		virtual int Remove();
};

template <typename T> THash <T>::THash()
{
	m_pNode = m_Iter = h_Iter = NULL;
	m_begin = m_end = m_Orig = NULL;
	m_Brucket = NULL;
	m_hash_begin = ~0;
	m_MaxSize = m_UsedSize = 0;
}

template <typename T> THash <T>::~THash()
{
	Clear();
	if (m_Brucket != NULL) {
		delete[]m_Brucket;
		m_Brucket = NULL;
	}
}

template <typename T>
    unsigned int THash <T>::make_hash(unsigned char *s, int len,
					   unsigned int hash)
{
	while (len-- > 0)
		hash =
		    hash_crctab32[(unsigned) (((int) hash ^ (*s++)) & 0xff)] ^
		    (hash >> 8);
	return hash;
}

template <typename T> int THash <T>::Init(int nSize)
{
	if (nSize <= 0) {
		return -1;
	}
	m_begin = m_end = NULL;
	m_UsedSize = 0;
	m_MaxSize = nSize;
	if (nSize <= 100) {
		m_MaxSize = 512;
	} else if (nSize < 5000 && nSize > 100) {
		m_MaxSize = 5120;
	} else if (nSize < 50000 && nSize >= 5000) {
		m_MaxSize = 51200;
	} else if (nSize < 500000 && nSize >= 50000) {
		m_MaxSize = 512000;
	} else if (nSize < 1000000 && nSize >= 500000) {
		m_MaxSize = 1024 * 1024;
	} else {
		m_MaxSize = nSize;
	}
	m_Brucket = new SRBC_HASH_NODE <T> *[m_MaxSize];
	if (m_Brucket == NULL) {
		printf("Create the THash Brucket(SIZE = %d) fail!!\n",
		       m_MaxSize);
		return -1;
	}
	for (int i = 0; i < m_MaxSize; i++) {
		m_Brucket[i] = NULL;
	}
	return 0;
}

template <typename T> void THash <T>::Clear(void)
{
	SRBC_HASH_NODE <T> *pPrev, *pNode;
	for (pNode = m_end; pNode; pNode = pPrev) {
		if (pNode != NULL) {
			pPrev = pNode->next;
			delete pNode;
			pNode = NULL;
		}
	}
	for (int i = 0; i < m_MaxSize; i++) {
		m_Brucket[i] = NULL;
	}
	m_begin = m_end = NULL;
	m_UsedSize = 0;
}

template <typename T>
    SRBC_HASH_NODE <T> *THash <T>::Create(const char *s,
					    unsigned int hash_value)
{
	m_pNode = new SRBC_HASH_NODE <T>;
	if (m_pNode == NULL) {
		return NULL;
	}
	if (m_begin == NULL) {
		m_begin = m_pNode;
	}
	m_pNode->hash_value = hash_value;
	m_pNode->next = m_end;
	m_pNode->prev = NULL;
	m_pNode->h_next = m_Brucket[hash_value];
	m_pNode->h_prev = NULL;
	if (m_end) {
		m_end->prev = m_pNode;
	}
	m_end = m_pNode;
	if (m_Brucket[hash_value]) {
		m_Brucket[hash_value]->h_prev = m_pNode;
	}
	m_Brucket[hash_value] = m_pNode;
	m_UsedSize++;
	strcpy(m_pNode->name, s);
	return m_pNode;
}

template <typename T> int THash <T>::Delete(SRBC_HASH_NODE <T> *pNode)
{
	if (pNode == NULL) {
		return -1;
	}

	if (pNode->prev)
		pNode->prev->next = pNode->next;
	else
		m_end = pNode->next;

	if (pNode->h_prev)
		pNode->h_prev->h_next = pNode->h_next;
	else
		m_Brucket[pNode->hash_value] = pNode->h_next;


	if (pNode->next) {
		pNode->next->prev = pNode->prev;
	}else{
		m_begin = pNode->prev;
	}

	if (pNode->h_next) {
		pNode->h_next->h_prev = pNode->h_prev;
	}

	m_UsedSize--;
	delete pNode;
	pNode = NULL;
	return 0;
}

template <typename T> int THash <T>::Add(const char *s, T * pNode)
{
	size_t slen = strlen(s);
	if (slen > (HASH_KEY_LEN - 1))
		return -1;

	unsigned int hash_value = make_hash((unsigned char *) s, slen, m_hash_begin);
	hash_value %= m_MaxSize;
	SRBC_HASH_NODE <T> *pOrig = Create(s, hash_value);
	if (pOrig != NULL) {
		memcpy(&(pOrig->value), pNode, sizeof(T));
		return 0;
	}
	return -1;
}

template <typename T>
    int THash <T>::Remove(const char *s, T &Node)
{
	T * pNode = NULL;
	unsigned int hash_value =
			make_hash((unsigned char *) s, strlen(s), m_hash_begin);
	hash_value %= m_MaxSize;
	int nRet = Find(s, hash_value, pNode);
	if (nRet < 0) {
		return -1;
	}
	memcpy(&Node, pNode, sizeof(Node));
	return Delete(m_pNode);
}

template <typename T>
    int THash <T>::Find(const char *s, unsigned int hash_value,
			     T * &pNode)
{
	SRBC_HASH_NODE <T> *pOrig = NULL;
	for (m_pNode = m_Brucket[hash_value]; m_pNode;
	     m_pNode = m_pNode->h_next) {
		if (strcmp(m_pNode->name, s) == 0) {
			pNode = &(m_pNode->value);
			m_Orig = m_pNode;
			return 0;
		}
	}
	pNode = NULL;
	return -1;

}

template <typename T> int THash <T>::Find(const char *s, T * &pNode)
{
	unsigned int hash_value =
			make_hash((unsigned char *) s, strlen(s), m_hash_begin);
	hash_value %= m_MaxSize;
	int nRet = Find(s, hash_value, pNode);
	if (nRet < 0) {
		h_Iter = NULL;
		return -1;
	}
	h_Iter = m_pNode;
	return 0;
}

template <typename T> void THash <T>::Reset(T * &pNode)
{
	pNode = &(m_Orig->value);
	m_Iter = m_Orig;
}

template <typename T> int THash <T>::Remove()
{
	return Delete(m_Orig);
}

template <typename T> int THash <T>::Next(const char *s, T * &pNode)
{
	if (h_Iter == NULL) {
		return -1;
	}
	for (SRBC_HASH_NODE <T> *pOrig = h_Iter->h_next; pOrig;
	     pOrig = pOrig->h_next) {
		if (strcmp(pOrig->name, s) == 0) {
			h_Iter = pOrig;
			m_Orig = pOrig;
			pNode = &(pOrig->value);
			return 0;
		}

	}
	h_Iter = NULL;
	return -1;
}

template <typename T> int THash <T>::Size()
{
	return m_UsedSize;
}


template <typename T> void THash <T>::Reset(void)
{
	m_Iter = m_begin;
}

template <typename T> int THash <T>::Next(T * &pNode, char *key)
{
	if (m_Iter == NULL) {
		return -1;
	}
	pNode = &(m_Iter->value);
	if (key != NULL)
		strcpy(key, m_Iter->name);
	m_Iter = m_Iter->prev;
	return 0;
}

template <typename T> void THash <T>::Begin()
{
	h_Iter = NULL;
}

template <typename T>
    int THash <T>::ReadNext(const char *s, T * &pNode)
{
	if (h_Iter == NULL)
		return Find(s, pNode);
	else
		return Next(s, pNode);
}

#endif
