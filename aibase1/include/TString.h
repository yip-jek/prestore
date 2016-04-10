/*
 * @All rights reserved
 *
 *   定义了在系统中使用字符串类,在系统中使用字符串类,可以大大简化系统
 * 中的一些代码实现,MFC类移植参考了 zjguo 的部分移植.
 * 
 * @Author  : devis
 * @Date    : 2002.09.16
 * @Version : 01.01.002
 * @History : 
 */
#ifndef __TSTRING_H__
#define __TSTRING_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "TUtility.h"
#include "Prelude.h"
#include "devisdefs.h"
////////////////////////////////////////////////////////////////////////////////////////

/** \example TString_test.cpp
  * \brief TString类的测试程序*/

/** \defgroup libsr 基础工具库(libsr)
 *  \{*/
/** \file TString.h 
 *  \brief  字符串类的头文件
 *  \warning 在Sun上测试会出现coredump,在AIX也没有经过严格测试
 *           最好当作一个C++字符串的习作看待,一定要用的话还需要严格测试一番.
 */

/** \struct TStringData
 *  \brief 字符串的头结构
 */
/** \}*/

struct TStringData
{
    long nRefs;            /**< 字符串的引用数*/
	long nDataLength;  /**< 字符串的数据长度*/
	long nAllocLength; /**< 分配给字符串的长度*/
	/** brief 返回指向实际字符串的指*/
	char * data()      
		{ return (char *)(this+1); }
};

////////////////////////////////////////////////////////////////////////////////////////
/** \defgroup libsr 基础工具库(libsr)
 *  \{*/
/** \class TString
 *  \brief 字符串类
 *  \warning 在Sun上测试会出现coredump,在AIX也没有经过严格测试
 *           最好当作一个C++字符串的习作看待,一定要用的话还需要严格测试一番.
 */
/** \}*/
class TString
{
public:
	/** \brief 构造函数*/
    TString();
	/** \brief 析构函数*/
	~TString();

	/** \brief 拷贝构造函数*/
    TString(const TString& stringSrc);
	/** \brief 赋值构造函数
	 *  \brief ch 字符
	 *  \brief nRepeat 字符重复出现次数*/
    TString(char ch, long nRepeat = 1);
	/** \brief 赋值构造函数*/
    TString(LPCSTR lpsz);
	/** \brief 赋值构造函数*/
    TString(LPCSTR lpch, long nLength);
	/** \brief 赋值构造函数*/
    TString(const unsigned char* psz);


////////////////////////////////////////////////////////////////////////////////////////
// 操作符重载
public:
    const TString& operator=(const TString& stringSrc);		// 重载操作符 =
    const TString& operator=(char ch);						// 过载操作符 =
    const TString& operator=(LPCSTR lpsz);					// 过载操作符 =
    const TString& operator=(const unsigned char* psz);		// 过载操作符 =

    const TString& operator+=(const TString& string);		// 重载操作符 +=
    const TString& operator+=(char ch);						// 过载操作符 +=
    const TString& operator+=(LPCSTR lpsz);					// 过载操作符 +=
	const TString& operator+=(char *lpsz);					// 过载操作符 +=

    friend TString operator+(const TString& string1,const TString& string2);
    friend TString operator+(const TString& string, char ch);
    friend TString operator+(char ch, const TString& string);
    friend TString operator+(const TString& string, LPCSTR lpsz);
    friend TString operator+(LPCSTR lpsz, const TString& string);

////////////////////////////////////////////////////////////////////////////////////////
// 字符串比较
public:
	/** \brief 字符串比较
	 *  \param lpsz 和本字符串比较的字符串
	 *  \return 相等返回0,本字符串小于返回-1,本字符串大于返回1
	 */
    inline long Compare(LPCSTR lpsz) const;
	/** \brief 字符串比较,不区分大小写
	 *  \param lpsz 和本字符串比较的字符串
	 *  \return 相等返回0,本字符串小于返回-1,本字符串大于返回1
	 */
    inline long CompareNoCase(LPCSTR lpsz) const;			// 字符串比较	
	/** \brief 指定长度的字符串比较,不区分大小写
	 *  \param lpsz 和本字符串比较的字符串
	 *  \param nLen 比较的长度
	 *  \return 相等返回0,本字符串小于返回-1,本字符串大于返回1
	 */
    inline long CompareNoCase(LPCSTR lpsz,long nLen) const; // 字符串比较
	/** \brief 字符串比较
	 *  \param lpsz 和本字符串比较的字符串
	 *  \return 相等返回0,本字符串小于返回-1,本字符串大于返回1
	 */
    inline long Collate(LPCSTR lpsz) const;					// 字符串比较
	/** \brief 字符串比较,不区分大小写
	 *  \param lpsz 和本字符串比较的字符串
	 *  \return 相等返回0,本字符串小于返回-1,本字符串大于返回1
	 */
    inline long CollateNoCase(LPCSTR lpsz) const;			// 字符串比较	

////////////////////////////////////////////////////////////////////////////////////////
// 子串抽取操作 Extraction
public:
	/** \brief 从字符串指定位置抽取指定长度的字符串
	 *  \param nFires 指定起始位置
	 *  \param nCount 指定抽取长度
	 *  \return 抽取到的子串
	 */
    TString Mid(long nFirst, long nCount) const;			
	/** \brief 抽取从字符串指定位位置开始到字符串末尾的子串
	 *  \param nFires 指定起始位置
	 *  \return 抽取到的子串
	 */
    TString Mid(long nFirst) const;
	/** \brief 抽取从左边开始的指定长度的字符串
	  * \param nCount 指定的长度
	  * \return 抽取到的子串
	  */
    TString Left(long nCount) const;						
	/** \brief 抽取从右边开始的指定长度的字符串
	  * \param nCount 指定的长度
          * \return 抽取到的子串
	  */
    TString Right(long nCount) const;
	/** \brief 抽取从字符串中第一次出现指定字符集中的字符的地方开始,到字符串结束的字符串
	  * \param lpszCharSet 字符串,指定要查找的字符
	  * \return 返回抽取得到的字符串,如果字符串中没有出现指定的字符,返回空串
	  */
    TString SpanIncluding(LPCSTR lpszCharSet) const;
	/** \brief 抽取从字符串开始,到第一次出现指定字符指定的字符集中字符的地方结束的字符串
	 *  \param lpszCharSet 字符串,指定要查找的字符
	 *  \return 返回抽取得到的字符串,如果字符串第一个字符就在指定字符集中,返回空串
	 */
    TString SpanExcluding(LPCSTR lpszCharSet) const;

////////////////////////////////////////////////////////////////////////////////////////
// 字符串操作
public:
	/** \brief 把字符串所有小写字符变成大写字符
	  */
    void MakeUpper();
	/** \brief 把字符串所有大写字符变成小写字符
	  */
    void MakeLower();
	/** \brief 把字符串反转
	  */
    void MakeReverse();
	/** \brief 去掉字符串尾部空格
	  */
	void TrimRight();
	/** \brief 去掉字符串头部空格
	  */
    void TrimLeft();
	/** \brief 去掉字符串尾部的指定字符
	  * \param chTarget 指定去掉的字符
	  */
	void TrimRight(char chTarget);
	/** \brief 去掉字指定字符集里面的字符组成的字符串尾部
	  * \param lpszTargets 由指定字符组成的字符串
	  */
    void TrimRight(LPCSTR lpszTargets);
	/** \brief 去掉字符串头部的指定字符
	  * \param chTarget 指定去掉的字符
	  */
    void TrimLeft(char chTarget);
	/** \brief 去掉字指定字符集里面的字符组成的字符串头部
	  * \param lpszTargets 由指定字符组成的字符串
	  */
    void TrimLeft(LPCSTR lpszTargets);
	/** \brief 去掉字符串头尾空格 */
    void TrimAll();
	/** \brief 判断输入的字符串是否全是空格
	  * \param p 判断的字符串
	  */
    int Space(char *p);
	/** \brief 去掉输入字符串头部空格
	  * \param s 输入的字符串
	  * \return 去掉头部空格后的字符串
	  */
    char * Ltrim(char *s);
	/** \brief 去掉输入字符串尾部空格
	  * \param s 输入的字符串
	  * \return 去掉尾部空格后的字符串
	  */
    char * Rtrim(char *s);
	/** \brief 去掉输入字符串头尾空格
	  * \param s 输入的字符串
	  * \return 去掉头尾空格后的字符串
	  */
    char * Alltrim(char *s);


////////////////////////////////////////////////////////////////////////////////////////
// 字符串操作
public:
	/** \brief 字符置换操作
	  * \param chOld 需要被置换的字符
	  * \param chNew 要置换成的字符
	  * \return 返回发生了置换的字符个数
	  */
    long Replace(char chOld, char chNew);
	/** \brief 字符串置换操作
	  * \param lpszOld 需要被置换的字符串
	  * \param lpszNew 要置换成的字符
	  * \return 返回发生了置换的字符个数
	  */
    long Replace(LPCSTR lpszOld, LPCSTR lpszNew);
	/** \brief 删除串内指定字符
	  * \param chRemove 需要被删除的指定字符
	  * \return 被删除的字符个数
	  */
    long Remove(char chRemove);
	/** \brief 在字符串指定的位置插入字符
	  * \param nIndex 字符串指定位置
	  * \param ch 指定的要插入的字符
	  * \return 返回插入之后字符串的长度
	  */
    long Insert(long nIndex, char ch);
	/** \brief 在字符串指定的位置插入字符串
	  * \param nIndex 字符串指定位置
	  * \param pstr  要插入的字符串
	  * \return 返回插入之后字符串的长度
	  */
    long Insert(long nIndex, LPCSTR pstr);
	/** \brief 删除字符串指定位置开始的指定个数字符
	  * \param nIndex  开始删除的位置
	  * \param nCount  删除的长度
	  * \return  删除字符串之后字符串的长度
	  */
    long Delete(long nIndex, long nCount = 1);

////////////////////////////////////////////////////////////////////////////////////////
// 字符串查找
public:
    /** \brief 查找字符串中第一次出现某字符的位置
      * \param ch 指定的查找字符
      * \return 返回查找到的位置,查找失败返回-1
      */
    long Find(char ch) const;
	/** \brief 似乎是从尾部开始查找第一个出现某字符的位置但是实现似乎有问题
	  * \TODO 实现似乎有问题*/
    long ReverseFind(char ch) const;
	/** \brief 从字符串指定位置开始,查找第一个出现某字符的位置
	  * \param ch 查找的字符
	  * \param nStart 开始查找的位置
	  * \return 返回查找到的位置,查找失败返回-1
	  */
    long Find(char ch, long nStart) const;
    	/** \brief 查找字符串中第一次出现指定字符集中的字符的位置
	  * \param lpszCharSet 指定的字符集
	  * \return 返回查找到的位置,查找失败返回-1
	  */
    long FindOneOf(LPCSTR lpszCharSet) const;
	/** \brief 查找字符串中第一次出现指定字符串的位置
	  * \param lpszSub 指定的字符串
	  * \return 返回查找到的位置,查找失败返回-1
	  */
    long Find(LPCSTR lpszSub) const;
	/** \brief 从指定的位置开始 查找字符串中第一次出现指定字符串的位置
	  * \param lpszSub 指定的字符串
	  * \return 返回查找到的位置,查找失败返回-1
	  */
    long Find(LPCSTR lpszSub, long nStart) const;

////////////////////////////////////////////////////////////////////////////////////////
// 高级功能,直接内存访问
public:
    LPSTR GetBuffer(long nMinBufLength);
    LPSTR GetBufferSetLength(long nNewLength);
	LPSTR LockBuffer();
	void UnlockBuffer();
	void ReleaseBuffer(long nNewLength = -1);
    void FreeExtra();

////////////////////////////////////////////////////////////////////////////////////////
// 字符串转换
public:
	/** \brief 转换 Int 为 TString 对象
	  * \param i 输入的int对象 
	  * \return 返回转换后的字符串长度,转换失败返回小于0
	  */
	long SetIntToStr(long i);						// 转换 Int 为 TString 对象
	/** \brief 字符串转成long
	  * \return 字符串转换成long的结果
	  */
	long ToInt(void) const;							// 将 TString 对象值转换为 long
	/** \brief 字符串转换成double
	  * \return 字符串转换成double的结果
	  */
	double ToFloat(void) const;						// 将 TString 对象值转换为 double

////////////////////////////////////////////////////////////////////////////////////////
// 字符串属性
protected:
	/** \brief 内部字符串数据的指针*/
    LPSTR m_pchData;   
    	/** \brief 获得内部字符串数据局头部结构的指针
	  * \return 返回内部字符串数据局头部结构的指针
	  */
    inline TStringData* GetData() const;

public:	
	/** \brief 获取字符串长度
	  * \return 返回字符串长度
	  */
    long GetLength() const;
	/** \brief 将字串置空并释放资源*/
    void Empty();
    	/** \brief 判断字符串是否为空串
	  * \return 如果字符串是空串返回true,否则返回false
	  */
	BOOL IsEmpty() const;

	/** \brief 获取字符串指定位置的字符
	  * \param nIndex 指定的位置,指定的位置不应小于0或者超出字符的长度
	  * \return 指定位置的字符
	  */
    char GetAt(long nIndex) const;
	/** \brief 把字符串指定位置的字符设置为指定的字符
	  * \param nIndex 指定的位置
	  * \param ch 指定的字符
	  */
    void SetAt(long nIndex, char ch);
	/** \brief 直接访问内存中C风格的字符串
	  * \return 返回C风格字符串的指针
	  */
    operator LPCSTR() const;
	/** \brief 直接访问内存中C风格的字符串
	  * \return 返回C风格字符串的指针
	  */
    operator char *() const;
	/** \brief 重载[],获得指定位置的字符
	  * \param nIndex 指定位置,指定的位置不应小于0或者超出字符的长度
	  * \return 返回获取的字符
	  */
    char operator[](long nIndex) const;

////////////////////////////////////////////////////////////////////////////////////////
// 字符串属性
protected:
	/** \brief 初始化字符串*/
    void Init();
	/** \brief 从当前字符串指定位置开始,拷贝指定长度到目标字符串 
	 *  \param dest 拷贝目标字符串
	 *  \param nCopyLen 拷贝的长度
	 *  \param nCopyIndex 开始拷贝位置
	 *  \param nExtraLen 分配额外长度,一般为0
	 */
    void AllocCopy(TString& dest, long nCopyLen, long nCopyIndex, long nExtraLen) const;
	/** \brief 分配实际内存空间
	  * \param nLen 字符串长度,实际上会分配nLen+1加上一个 \ref TStringData 的大小的空间
	  */
    void AllocBuffer(long nLen);
	/** \brief 拷贝输入的字符串到当前字符串,如果当前字符串空间不足可以重新分配足够空间
	 *  \param nSrcLen 被拷贝字符串长度
	 *  \param lpszSrcData 被拷贝字符串
	 */
    void AssignCopy(long nSrcLen, LPCSTR lpszSrcData);
    	/** \brief 拷贝输入的两个字符串到当前字符串,如果当前字符串空间不足可以重新分配足够空间
	  * \param nSrc1Len  第一个字符串的长度
	  * \param lpszSrc1Data 第一个字符串
	  * \param nSrc2Len 第二个字符串的长度
	  * \param lpszSrc2Data 第二个字符串
	  */
    void ConcatCopy(long nSrc1Len, LPCSTR lpszSrc1Data, long nSrc2Len, LPCSTR lpszSrc2Data);
	/** \brief 在当前字符串的尾部加上指定的字符串,如果当前字符串空间不足可以重新分配足够空间
	  * \param nSrcLen 加上字符串的长度
	  * \param lpszSrcData 加上的字符串
	  */
    void ConcatInPlace(long nSrcLen, LPCSTR lpszSrcData);
	/** \brief 检查当前TString对象是否和其他TString对象共用一个TStringData实体+字符串,
	           如果是的话,修改当前TString对象之前必须给当前对象复制一份TStringData+字符串
		   否则引起所有共用此TStringData对象的TString都改变了
	 */
    void CopyBeforeWrite();
	/** \brief 检查字符串分配的空间是否小于指定长度,是的话重新分配足够空间
	  * \param nLen 需要的空间长度
	  */
    void AllocBeforeWrite(long nLen);
	/** \brief 减少当前字符串的引用计数,如果是字符串的最后一个引用,则释放字符串*/
    void Release();
	/** \brief 获得分配给字符串的长度
	  * \return 返回分配给字符串的长度
	  */
    long GetAllocLength() const;

protected:
	/** \brief 减少传入字符串数据的引用计数,如果是字符串的最后一个引用,则释放字符串
	  * \param TStringData 传入的字符串数据
	  */
    static void Release(TStringData* pData);
	/** \brief 更安全的判断字符串长度,对空指针特殊处理
	  * \param lpsz 需要判断长度的字符串
	  * \return 返回字符串长度,如果是空指针返回0 
	  */
    static long SafeStrlen(LPCSTR lpsz);
	/** \brief 删除实际存储的空间
	  * \param 实际存储空间的指针
	  */
    static void FreeData(TStringData* pData);
};

////////////////////////////////////////////////////////////////////////////////////////
/*
 * @describe : 直接访问内存 C-style 字符串
 *	  directly accesses characters stored in a TString object as a C-style string.
 *
 * @param : None
 * @return: C-style string 
 */
inline TString::operator LPCSTR() const
{ 
	return m_pchData; 
}

/*
 * @describe : 直接访问内存 C-style 字符串
 *        directly accesses characters stored in a TString object as a C-style string.
 *
 * @param : None
 * @return: C-style string
 */
inline TString::operator char *() const
{
        return (char *) m_pchData;
}

inline TStringData* TString::GetData() const
{
    ASSERT(m_pchData != NULL); 
    return ((TStringData*)m_pchData)-1;
}

////////////////////////////////////////////////////////////////////////////////////////
inline long TString::GetAllocLength() const
{ 
	return GetData()->nAllocLength; 
}
inline long TString::SafeStrlen(LPCSTR lpsz)
{ 
	return (lpsz == NULL) ? 0 : strlen(lpsz); 
}

////////////////////////////////////////////////////////////////////////////////////////
const TString& __GetEmptyString();
#define __EmptyString __GetEmptyString()

////////////////////////////////////////////////////////////////////////////////////////
/*
 * @describe : 重载操作符, 关系判断操作符 == 
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. 比较字符串
 *          const TString &s2 : TString objects to compare. 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator==(const TString& s1, const TString& s2)
    { return s1.Compare(s2) == 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 == 
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. 比较字符串
 *          LPCSTR s2 :         比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator==(const TString& s1, LPCSTR s2)
    { return s1.Compare(s2) == 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 == 
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s2 : TString objects to compare. 比较字符串
 *          LPCSTR s1 :         比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator==(LPCSTR s1, const TString& s2)
    { return s2.Compare(s1) == 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 != 
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. 比较字符串
 *          const TString &s2 : TString objects to compare. 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator!=(const TString& s1, const TString& s2)
    { return s1.Compare(s2) != 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 != 
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. 比较字符串
 *          LPCSTR s2         : 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator!=(const TString& s1, LPCSTR s2)
    { return s1.Compare(s2) != 0; }
/*
 * @describe : 重载操作符, 关系判断操作符 != 
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s2 : TString objects to compare. 比较字符串
 *          LPCSTR s1         : 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator!=(LPCSTR s1, const TString& s2)
    { return s2.Compare(s1) != 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 <
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s2 : TString objects to compare. 比较字符串
 *          const TString &s1 : TString objects to compare. 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator<(const TString& s1, const TString& s2)
    { return s1.Compare(s2) < 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 <
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. 比较字符串
 *          LPCSTR s2         : 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator<(const TString& s1, LPCSTR s2)
    { return s1.Compare(s2) < 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 <
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. 比较字符串
 *          LPCSTR s1         : 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator<(LPCSTR s1, const TString& s2)
    { return s2.Compare(s1) > 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 >
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. 比较字符串
 *          const TString &s2 : TString objects to compare. 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator>(const TString& s1, const TString& s2)
    { return s1.Compare(s2) > 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 >
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. 比较字符串
 *          LPCSTR s2         : 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator>(const TString& s1, LPCSTR s2)
    { return s1.Compare(s2) > 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 >
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s2 : TString objects to compare. 比较字符串
 *          LPCSTR s1         : 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator>(LPCSTR s1, const TString& s2)
    { return s2.Compare(s1) < 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 <=
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. 比较字符串
 *          const TString &s2 : TString objects to compare. 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator<=(const TString& s1, const TString& s2)
    { return s1.Compare(s2) <= 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 <=
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. 比较字符串
 *          LPCSTR s2         : 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator<=(const TString& s1, LPCSTR s2)
    { return s1.Compare(s2) <= 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 <=
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s2 : TString objects to compare. 比较字符串
 *          LPCSTR s1         : 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator<=(LPCSTR s1, const TString& s2)
    { return s2.Compare(s1) >= 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 >=
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. 比较字符串
 *          const TString &s1 : TString objects to compare. 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator>=(const TString& s1, const TString& s2)
    { return s1.Compare(s2) >= 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 >=
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. 比较字符串
 *          LPCSTR s2         : 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator>=(const TString& s1, LPCSTR s2)
    { return s1.Compare(s2) >= 0; }

/*
 * @describe : 重载操作符, 关系判断操作符 >=
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s2 : TString objects to compare. 比较字符串
 *          LPCSTR s1         : 比较字符串
 * @return: 满足条件返回 TRUE,失败返回 FALSE
 */
inline BOOL operator>=(LPCSTR s1, const TString& s2)
    { return s2.Compare(s1) <= 0; }

/*
 * @describe : 重载赋值操作符
 *
 *    The TString assignment (=) operator reinitializes an existing TString object 
 * with new data. If the destination string (that is, the left side) is already large 
 * enough to store the new data, no new memory allocation is performed. You should be 
 * aware that memory exceptions may occur whenever you use the assignment operator 
 * because new storage is often allocated to hold the resulting TString object.
 *
 * @param : const unsigned char * lpsz 需要进行赋值的字符串
 * @return: TString &   对象引用
 */
inline const TString& TString::operator=(const unsigned char* lpsz)
    { *this = (LPCSTR)lpsz; return *this; }

////////////////////////////////////////////////////////////////////////////////////////
/*
 * @describe : 字符串比较
 *     用于字符串操作符的重载. 例如 < , <= , > , >= , != , == 等等.
 *
 * @param : LPCSTR lpsz 字符串
 * @return: 比较值 
 *          Zero if the strings are identical, < 0 if this TString object is less than 
 *      lpsz, or > 0 if this TString object is greater than lpsz.
 */
inline long TString::Compare(LPCSTR lpsz) const
{ 
    ASSERT(lpsz!=NULL); 
    return strcmp(m_pchData, lpsz); 
}

/*
 * @describe : 字符串比较 CompareNoCase 
 *	 字符串的比较功能.实际上调用了标准的 strcasecmp 功能,由于有些平台的 strcasecmp 没有
 * 提供,因而需要提供此功能.
 *
 * @param : LPCSTR lpsz
 * @return: 比较返回值.
 */
inline long TString::CompareNoCase(LPCSTR lpsz) const
{ 
	/* __VER@01.01.001 by devis */
    ASSERT(lpsz!=NULL);
	return strcasecmp(m_pchData, lpsz); 
}

/*
 * @describe : 字符串比较 CompareNoCase 
 *	 字符串的比较功能.实际上调用了标准的 strcasecmp 功能,由于有些平台的 strcasecmp 没有
 * 提供,因而需要提供此功能.
 *
 * @param : LPCSTR lpsz: 比较字符串
 *          long nLen  : 返回值.
 * @return: 比较返回值.
 */
inline long TString::CompareNoCase(LPCSTR lpsz,long nLen) const
{ 
	/* __VER@01.01.001 by devis */
    ASSERT(lpsz!=NULL); 
    return strncasecmp(m_pchData,lpsz,nLen); 
}

/*
 * @describe : 字符串比较.
 *
 * @param : LPCSTR lpsz : The other string used for comparison
 * @return: 比较返回值
 *	  Zero if the strings are identical, <0 if this TString object is less than lpsz,
 * or >0 if this TString object is greater than lpsz.
 */
inline long TString::Collate(LPCSTR lpsz) const
{ 
	/* __VER@01.01.001 by devis */
    ASSERT(lpsz!=NULL); 
    return strcoll(m_pchData, lpsz); 
}

/*
 * @describe : 字符串比较, strcasecoll 
 *     Zero if the strings are identical (ignoring case), <0 if this TString 
 * object is less than lpsz (ignoring case), or >0 if this TString object is
 * greater than lpsz (ignoring case).
 *
 * @param : LPCSTR lpsz  : A pointer to the other string used for comparison
 * @return: long 
 */
inline long TString::CollateNoCase(LPCSTR lpsz) const
{ 
    ASSERT(lpsz!=NULL); 
    return strcasecoll(m_pchData, lpsz); 
}

/*
 * @describe : 取指定位置的字符.
 * 	  A TCHAR containing the character at the specified position in the string.
 * You can think of a CString object as an array of characters. The GetAt member 
 * function returns a single character specified by an index number. The overloaded 
 * subscript ([]) operator is a convenient alias for GetAt.
 * 
 * @param : long nIndex 基于零的索引值
 *          Zero-based index of the character in the TString object. The nIndex 
 *  parameter must be greater than or equal to 0 and less than the value returned 
 *  by GetLength.
 *
 * @return: char 型字符
 *    A TCHAR containing the character at the specified position in the string.
 */
inline char TString::operator[](long nIndex) const
{
    ASSERT(nIndex >= 0);
    ASSERT(nIndex < GetData()->nDataLength);
    return m_pchData[nIndex];
}

#endif
