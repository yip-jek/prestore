/*
 * @All rights reserved
 *
 *   ��������ϵͳ��ʹ���ַ�����,��ϵͳ��ʹ���ַ�����,���Դ���ϵͳ
 * �е�һЩ����ʵ��,MFC����ֲ�ο��� zjguo �Ĳ�����ֲ.
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
  * \brief TString��Ĳ��Գ���*/

/** \defgroup libsr �������߿�(libsr)
 *  \{*/
/** \file TString.h 
 *  \brief  �ַ������ͷ�ļ�
 *  \warning ��Sun�ϲ��Ի����coredump,��AIXҲû�о����ϸ����
 *           ��õ���һ��C++�ַ�����ϰ������,һ��Ҫ�õĻ�����Ҫ�ϸ����һ��.
 */

/** \struct TStringData
 *  \brief �ַ�����ͷ�ṹ
 */
/** \}*/

struct TStringData
{
    long nRefs;            /**< �ַ�����������*/
	long nDataLength;  /**< �ַ��������ݳ���*/
	long nAllocLength; /**< ������ַ����ĳ���*/
	/** brief ����ָ��ʵ���ַ�����ָ*/
	char * data()      
		{ return (char *)(this+1); }
};

////////////////////////////////////////////////////////////////////////////////////////
/** \defgroup libsr �������߿�(libsr)
 *  \{*/
/** \class TString
 *  \brief �ַ�����
 *  \warning ��Sun�ϲ��Ի����coredump,��AIXҲû�о����ϸ����
 *           ��õ���һ��C++�ַ�����ϰ������,һ��Ҫ�õĻ�����Ҫ�ϸ����һ��.
 */
/** \}*/
class TString
{
public:
	/** \brief ���캯��*/
    TString();
	/** \brief ��������*/
	~TString();

	/** \brief �������캯��*/
    TString(const TString& stringSrc);
	/** \brief ��ֵ���캯��
	 *  \brief ch �ַ�
	 *  \brief nRepeat �ַ��ظ����ִ���*/
    TString(char ch, long nRepeat = 1);
	/** \brief ��ֵ���캯��*/
    TString(LPCSTR lpsz);
	/** \brief ��ֵ���캯��*/
    TString(LPCSTR lpch, long nLength);
	/** \brief ��ֵ���캯��*/
    TString(const unsigned char* psz);


////////////////////////////////////////////////////////////////////////////////////////
// ����������
public:
    const TString& operator=(const TString& stringSrc);		// ���ز����� =
    const TString& operator=(char ch);						// ���ز����� =
    const TString& operator=(LPCSTR lpsz);					// ���ز����� =
    const TString& operator=(const unsigned char* psz);		// ���ز����� =

    const TString& operator+=(const TString& string);		// ���ز����� +=
    const TString& operator+=(char ch);						// ���ز����� +=
    const TString& operator+=(LPCSTR lpsz);					// ���ز����� +=
	const TString& operator+=(char *lpsz);					// ���ز����� +=

    friend TString operator+(const TString& string1,const TString& string2);
    friend TString operator+(const TString& string, char ch);
    friend TString operator+(char ch, const TString& string);
    friend TString operator+(const TString& string, LPCSTR lpsz);
    friend TString operator+(LPCSTR lpsz, const TString& string);

////////////////////////////////////////////////////////////////////////////////////////
// �ַ����Ƚ�
public:
	/** \brief �ַ����Ƚ�
	 *  \param lpsz �ͱ��ַ����Ƚϵ��ַ���
	 *  \return ��ȷ���0,���ַ���С�ڷ���-1,���ַ������ڷ���1
	 */
    inline long Compare(LPCSTR lpsz) const;
	/** \brief �ַ����Ƚ�,�����ִ�Сд
	 *  \param lpsz �ͱ��ַ����Ƚϵ��ַ���
	 *  \return ��ȷ���0,���ַ���С�ڷ���-1,���ַ������ڷ���1
	 */
    inline long CompareNoCase(LPCSTR lpsz) const;			// �ַ����Ƚ�	
	/** \brief ָ�����ȵ��ַ����Ƚ�,�����ִ�Сд
	 *  \param lpsz �ͱ��ַ����Ƚϵ��ַ���
	 *  \param nLen �Ƚϵĳ���
	 *  \return ��ȷ���0,���ַ���С�ڷ���-1,���ַ������ڷ���1
	 */
    inline long CompareNoCase(LPCSTR lpsz,long nLen) const; // �ַ����Ƚ�
	/** \brief �ַ����Ƚ�
	 *  \param lpsz �ͱ��ַ����Ƚϵ��ַ���
	 *  \return ��ȷ���0,���ַ���С�ڷ���-1,���ַ������ڷ���1
	 */
    inline long Collate(LPCSTR lpsz) const;					// �ַ����Ƚ�
	/** \brief �ַ����Ƚ�,�����ִ�Сд
	 *  \param lpsz �ͱ��ַ����Ƚϵ��ַ���
	 *  \return ��ȷ���0,���ַ���С�ڷ���-1,���ַ������ڷ���1
	 */
    inline long CollateNoCase(LPCSTR lpsz) const;			// �ַ����Ƚ�	

////////////////////////////////////////////////////////////////////////////////////////
// �Ӵ���ȡ���� Extraction
public:
	/** \brief ���ַ���ָ��λ�ó�ȡָ�����ȵ��ַ���
	 *  \param nFires ָ����ʼλ��
	 *  \param nCount ָ����ȡ����
	 *  \return ��ȡ�����Ӵ�
	 */
    TString Mid(long nFirst, long nCount) const;			
	/** \brief ��ȡ���ַ���ָ��λλ�ÿ�ʼ���ַ���ĩβ���Ӵ�
	 *  \param nFires ָ����ʼλ��
	 *  \return ��ȡ�����Ӵ�
	 */
    TString Mid(long nFirst) const;
	/** \brief ��ȡ����߿�ʼ��ָ�����ȵ��ַ���
	  * \param nCount ָ���ĳ���
	  * \return ��ȡ�����Ӵ�
	  */
    TString Left(long nCount) const;						
	/** \brief ��ȡ���ұ߿�ʼ��ָ�����ȵ��ַ���
	  * \param nCount ָ���ĳ���
          * \return ��ȡ�����Ӵ�
	  */
    TString Right(long nCount) const;
	/** \brief ��ȡ���ַ����е�һ�γ���ָ���ַ����е��ַ��ĵط���ʼ,���ַ����������ַ���
	  * \param lpszCharSet �ַ���,ָ��Ҫ���ҵ��ַ�
	  * \return ���س�ȡ�õ����ַ���,����ַ�����û�г���ָ�����ַ�,���ؿմ�
	  */
    TString SpanIncluding(LPCSTR lpszCharSet) const;
	/** \brief ��ȡ���ַ�����ʼ,����һ�γ���ָ���ַ�ָ�����ַ������ַ��ĵط��������ַ���
	 *  \param lpszCharSet �ַ���,ָ��Ҫ���ҵ��ַ�
	 *  \return ���س�ȡ�õ����ַ���,����ַ�����һ���ַ�����ָ���ַ�����,���ؿմ�
	 */
    TString SpanExcluding(LPCSTR lpszCharSet) const;

////////////////////////////////////////////////////////////////////////////////////////
// �ַ�������
public:
	/** \brief ���ַ�������Сд�ַ���ɴ�д�ַ�
	  */
    void MakeUpper();
	/** \brief ���ַ������д�д�ַ����Сд�ַ�
	  */
    void MakeLower();
	/** \brief ���ַ�����ת
	  */
    void MakeReverse();
	/** \brief ȥ���ַ���β���ո�
	  */
	void TrimRight();
	/** \brief ȥ���ַ���ͷ���ո�
	  */
    void TrimLeft();
	/** \brief ȥ���ַ���β����ָ���ַ�
	  * \param chTarget ָ��ȥ�����ַ�
	  */
	void TrimRight(char chTarget);
	/** \brief ȥ����ָ���ַ���������ַ���ɵ��ַ���β��
	  * \param lpszTargets ��ָ���ַ���ɵ��ַ���
	  */
    void TrimRight(LPCSTR lpszTargets);
	/** \brief ȥ���ַ���ͷ����ָ���ַ�
	  * \param chTarget ָ��ȥ�����ַ�
	  */
    void TrimLeft(char chTarget);
	/** \brief ȥ����ָ���ַ���������ַ���ɵ��ַ���ͷ��
	  * \param lpszTargets ��ָ���ַ���ɵ��ַ���
	  */
    void TrimLeft(LPCSTR lpszTargets);
	/** \brief ȥ���ַ���ͷβ�ո� */
    void TrimAll();
	/** \brief �ж�������ַ����Ƿ�ȫ�ǿո�
	  * \param p �жϵ��ַ���
	  */
    int Space(char *p);
	/** \brief ȥ�������ַ���ͷ���ո�
	  * \param s ������ַ���
	  * \return ȥ��ͷ���ո����ַ���
	  */
    char * Ltrim(char *s);
	/** \brief ȥ�������ַ���β���ո�
	  * \param s ������ַ���
	  * \return ȥ��β���ո����ַ���
	  */
    char * Rtrim(char *s);
	/** \brief ȥ�������ַ���ͷβ�ո�
	  * \param s ������ַ���
	  * \return ȥ��ͷβ�ո����ַ���
	  */
    char * Alltrim(char *s);


////////////////////////////////////////////////////////////////////////////////////////
// �ַ�������
public:
	/** \brief �ַ��û�����
	  * \param chOld ��Ҫ���û����ַ�
	  * \param chNew Ҫ�û��ɵ��ַ�
	  * \return ���ط������û����ַ�����
	  */
    long Replace(char chOld, char chNew);
	/** \brief �ַ����û�����
	  * \param lpszOld ��Ҫ���û����ַ���
	  * \param lpszNew Ҫ�û��ɵ��ַ�
	  * \return ���ط������û����ַ�����
	  */
    long Replace(LPCSTR lpszOld, LPCSTR lpszNew);
	/** \brief ɾ������ָ���ַ�
	  * \param chRemove ��Ҫ��ɾ����ָ���ַ�
	  * \return ��ɾ�����ַ�����
	  */
    long Remove(char chRemove);
	/** \brief ���ַ���ָ����λ�ò����ַ�
	  * \param nIndex �ַ���ָ��λ��
	  * \param ch ָ����Ҫ������ַ�
	  * \return ���ز���֮���ַ����ĳ���
	  */
    long Insert(long nIndex, char ch);
	/** \brief ���ַ���ָ����λ�ò����ַ���
	  * \param nIndex �ַ���ָ��λ��
	  * \param pstr  Ҫ������ַ���
	  * \return ���ز���֮���ַ����ĳ���
	  */
    long Insert(long nIndex, LPCSTR pstr);
	/** \brief ɾ���ַ���ָ��λ�ÿ�ʼ��ָ�������ַ�
	  * \param nIndex  ��ʼɾ����λ��
	  * \param nCount  ɾ���ĳ���
	  * \return  ɾ���ַ���֮���ַ����ĳ���
	  */
    long Delete(long nIndex, long nCount = 1);

////////////////////////////////////////////////////////////////////////////////////////
// �ַ�������
public:
    /** \brief �����ַ����е�һ�γ���ĳ�ַ���λ��
      * \param ch ָ���Ĳ����ַ�
      * \return ���ز��ҵ���λ��,����ʧ�ܷ���-1
      */
    long Find(char ch) const;
	/** \brief �ƺ��Ǵ�β����ʼ���ҵ�һ������ĳ�ַ���λ�õ���ʵ���ƺ�������
	  * \TODO ʵ���ƺ�������*/
    long ReverseFind(char ch) const;
	/** \brief ���ַ���ָ��λ�ÿ�ʼ,���ҵ�һ������ĳ�ַ���λ��
	  * \param ch ���ҵ��ַ�
	  * \param nStart ��ʼ���ҵ�λ��
	  * \return ���ز��ҵ���λ��,����ʧ�ܷ���-1
	  */
    long Find(char ch, long nStart) const;
    	/** \brief �����ַ����е�һ�γ���ָ���ַ����е��ַ���λ��
	  * \param lpszCharSet ָ�����ַ���
	  * \return ���ز��ҵ���λ��,����ʧ�ܷ���-1
	  */
    long FindOneOf(LPCSTR lpszCharSet) const;
	/** \brief �����ַ����е�һ�γ���ָ���ַ�����λ��
	  * \param lpszSub ָ�����ַ���
	  * \return ���ز��ҵ���λ��,����ʧ�ܷ���-1
	  */
    long Find(LPCSTR lpszSub) const;
	/** \brief ��ָ����λ�ÿ�ʼ �����ַ����е�һ�γ���ָ���ַ�����λ��
	  * \param lpszSub ָ�����ַ���
	  * \return ���ز��ҵ���λ��,����ʧ�ܷ���-1
	  */
    long Find(LPCSTR lpszSub, long nStart) const;

////////////////////////////////////////////////////////////////////////////////////////
// �߼�����,ֱ���ڴ����
public:
    LPSTR GetBuffer(long nMinBufLength);
    LPSTR GetBufferSetLength(long nNewLength);
	LPSTR LockBuffer();
	void UnlockBuffer();
	void ReleaseBuffer(long nNewLength = -1);
    void FreeExtra();

////////////////////////////////////////////////////////////////////////////////////////
// �ַ���ת��
public:
	/** \brief ת�� Int Ϊ TString ����
	  * \param i �����int���� 
	  * \return ����ת������ַ�������,ת��ʧ�ܷ���С��0
	  */
	long SetIntToStr(long i);						// ת�� Int Ϊ TString ����
	/** \brief �ַ���ת��long
	  * \return �ַ���ת����long�Ľ��
	  */
	long ToInt(void) const;							// �� TString ����ֵת��Ϊ long
	/** \brief �ַ���ת����double
	  * \return �ַ���ת����double�Ľ��
	  */
	double ToFloat(void) const;						// �� TString ����ֵת��Ϊ double

////////////////////////////////////////////////////////////////////////////////////////
// �ַ�������
protected:
	/** \brief �ڲ��ַ������ݵ�ָ��*/
    LPSTR m_pchData;   
    	/** \brief ����ڲ��ַ������ݾ�ͷ���ṹ��ָ��
	  * \return �����ڲ��ַ������ݾ�ͷ���ṹ��ָ��
	  */
    inline TStringData* GetData() const;

public:	
	/** \brief ��ȡ�ַ�������
	  * \return �����ַ�������
	  */
    long GetLength() const;
	/** \brief ���ִ��ÿղ��ͷ���Դ*/
    void Empty();
    	/** \brief �ж��ַ����Ƿ�Ϊ�մ�
	  * \return ����ַ����ǿմ�����true,���򷵻�false
	  */
	BOOL IsEmpty() const;

	/** \brief ��ȡ�ַ���ָ��λ�õ��ַ�
	  * \param nIndex ָ����λ��,ָ����λ�ò�ӦС��0���߳����ַ��ĳ���
	  * \return ָ��λ�õ��ַ�
	  */
    char GetAt(long nIndex) const;
	/** \brief ���ַ���ָ��λ�õ��ַ�����Ϊָ�����ַ�
	  * \param nIndex ָ����λ��
	  * \param ch ָ�����ַ�
	  */
    void SetAt(long nIndex, char ch);
	/** \brief ֱ�ӷ����ڴ���C�����ַ���
	  * \return ����C����ַ�����ָ��
	  */
    operator LPCSTR() const;
	/** \brief ֱ�ӷ����ڴ���C�����ַ���
	  * \return ����C����ַ�����ָ��
	  */
    operator char *() const;
	/** \brief ����[],���ָ��λ�õ��ַ�
	  * \param nIndex ָ��λ��,ָ����λ�ò�ӦС��0���߳����ַ��ĳ���
	  * \return ���ػ�ȡ���ַ�
	  */
    char operator[](long nIndex) const;

////////////////////////////////////////////////////////////////////////////////////////
// �ַ�������
protected:
	/** \brief ��ʼ���ַ���*/
    void Init();
	/** \brief �ӵ�ǰ�ַ���ָ��λ�ÿ�ʼ,����ָ�����ȵ�Ŀ���ַ��� 
	 *  \param dest ����Ŀ���ַ���
	 *  \param nCopyLen �����ĳ���
	 *  \param nCopyIndex ��ʼ����λ��
	 *  \param nExtraLen ������ⳤ��,һ��Ϊ0
	 */
    void AllocCopy(TString& dest, long nCopyLen, long nCopyIndex, long nExtraLen) const;
	/** \brief ����ʵ���ڴ�ռ�
	  * \param nLen �ַ�������,ʵ���ϻ����nLen+1����һ�� \ref TStringData �Ĵ�С�Ŀռ�
	  */
    void AllocBuffer(long nLen);
	/** \brief ����������ַ�������ǰ�ַ���,�����ǰ�ַ����ռ䲻��������·����㹻�ռ�
	 *  \param nSrcLen �������ַ�������
	 *  \param lpszSrcData �������ַ���
	 */
    void AssignCopy(long nSrcLen, LPCSTR lpszSrcData);
    	/** \brief ��������������ַ�������ǰ�ַ���,�����ǰ�ַ����ռ䲻��������·����㹻�ռ�
	  * \param nSrc1Len  ��һ���ַ����ĳ���
	  * \param lpszSrc1Data ��һ���ַ���
	  * \param nSrc2Len �ڶ����ַ����ĳ���
	  * \param lpszSrc2Data �ڶ����ַ���
	  */
    void ConcatCopy(long nSrc1Len, LPCSTR lpszSrc1Data, long nSrc2Len, LPCSTR lpszSrc2Data);
	/** \brief �ڵ�ǰ�ַ�����β������ָ�����ַ���,�����ǰ�ַ����ռ䲻��������·����㹻�ռ�
	  * \param nSrcLen �����ַ����ĳ���
	  * \param lpszSrcData ���ϵ��ַ���
	  */
    void ConcatInPlace(long nSrcLen, LPCSTR lpszSrcData);
	/** \brief ��鵱ǰTString�����Ƿ������TString������һ��TStringDataʵ��+�ַ���,
	           ����ǵĻ�,�޸ĵ�ǰTString����֮ǰ�������ǰ������һ��TStringData+�ַ���
		   �����������й��ô�TStringData�����TString���ı���
	 */
    void CopyBeforeWrite();
	/** \brief ����ַ�������Ŀռ��Ƿ�С��ָ������,�ǵĻ����·����㹻�ռ�
	  * \param nLen ��Ҫ�Ŀռ䳤��
	  */
    void AllocBeforeWrite(long nLen);
	/** \brief ���ٵ�ǰ�ַ��������ü���,������ַ��������һ������,���ͷ��ַ���*/
    void Release();
	/** \brief ��÷�����ַ����ĳ���
	  * \return ���ط�����ַ����ĳ���
	  */
    long GetAllocLength() const;

protected:
	/** \brief ���ٴ����ַ������ݵ����ü���,������ַ��������һ������,���ͷ��ַ���
	  * \param TStringData ������ַ�������
	  */
    static void Release(TStringData* pData);
	/** \brief ����ȫ���ж��ַ�������,�Կ�ָ�����⴦��
	  * \param lpsz ��Ҫ�жϳ��ȵ��ַ���
	  * \return �����ַ�������,����ǿ�ָ�뷵��0 
	  */
    static long SafeStrlen(LPCSTR lpsz);
	/** \brief ɾ��ʵ�ʴ洢�Ŀռ�
	  * \param ʵ�ʴ洢�ռ��ָ��
	  */
    static void FreeData(TStringData* pData);
};

////////////////////////////////////////////////////////////////////////////////////////
/*
 * @describe : ֱ�ӷ����ڴ� C-style �ַ���
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
 * @describe : ֱ�ӷ����ڴ� C-style �ַ���
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
 * @describe : ���ز�����, ��ϵ�жϲ����� == 
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. �Ƚ��ַ���
 *          const TString &s2 : TString objects to compare. �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator==(const TString& s1, const TString& s2)
    { return s1.Compare(s2) == 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� == 
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. �Ƚ��ַ���
 *          LPCSTR s2 :         �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator==(const TString& s1, LPCSTR s2)
    { return s1.Compare(s2) == 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� == 
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s2 : TString objects to compare. �Ƚ��ַ���
 *          LPCSTR s1 :         �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator==(LPCSTR s1, const TString& s2)
    { return s2.Compare(s1) == 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� != 
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. �Ƚ��ַ���
 *          const TString &s2 : TString objects to compare. �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator!=(const TString& s1, const TString& s2)
    { return s1.Compare(s2) != 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� != 
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. �Ƚ��ַ���
 *          LPCSTR s2         : �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator!=(const TString& s1, LPCSTR s2)
    { return s1.Compare(s2) != 0; }
/*
 * @describe : ���ز�����, ��ϵ�жϲ����� != 
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s2 : TString objects to compare. �Ƚ��ַ���
 *          LPCSTR s1         : �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator!=(LPCSTR s1, const TString& s2)
    { return s2.Compare(s1) != 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� <
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s2 : TString objects to compare. �Ƚ��ַ���
 *          const TString &s1 : TString objects to compare. �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator<(const TString& s1, const TString& s2)
    { return s1.Compare(s2) < 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� <
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. �Ƚ��ַ���
 *          LPCSTR s2         : �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator<(const TString& s1, LPCSTR s2)
    { return s1.Compare(s2) < 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� <
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. �Ƚ��ַ���
 *          LPCSTR s1         : �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator<(LPCSTR s1, const TString& s2)
    { return s2.Compare(s1) > 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� >
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. �Ƚ��ַ���
 *          const TString &s2 : TString objects to compare. �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator>(const TString& s1, const TString& s2)
    { return s1.Compare(s2) > 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� >
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. �Ƚ��ַ���
 *          LPCSTR s2         : �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator>(const TString& s1, LPCSTR s2)
    { return s1.Compare(s2) > 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� >
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s2 : TString objects to compare. �Ƚ��ַ���
 *          LPCSTR s1         : �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator>(LPCSTR s1, const TString& s2)
    { return s2.Compare(s1) < 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� <=
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. �Ƚ��ַ���
 *          const TString &s2 : TString objects to compare. �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator<=(const TString& s1, const TString& s2)
    { return s1.Compare(s2) <= 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� <=
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. �Ƚ��ַ���
 *          LPCSTR s2         : �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator<=(const TString& s1, LPCSTR s2)
    { return s1.Compare(s2) <= 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� <=
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s2 : TString objects to compare. �Ƚ��ַ���
 *          LPCSTR s1         : �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator<=(LPCSTR s1, const TString& s2)
    { return s2.Compare(s1) >= 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� >=
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. �Ƚ��ַ���
 *          const TString &s1 : TString objects to compare. �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator>=(const TString& s1, const TString& s2)
    { return s1.Compare(s2) >= 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� >=
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s1 : TString objects to compare. �Ƚ��ַ���
 *          LPCSTR s2         : �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator>=(const TString& s1, LPCSTR s2)
    { return s1.Compare(s2) >= 0; }

/*
 * @describe : ���ز�����, ��ϵ�жϲ����� >=
 *   Nonzero if the strings meet the comparison condition; otherwise 0. These 
 * comparison operators compare two strings. The operators are a convenient 
 * substitute for the case-sensitive Compare member function.
 *
 * @param : const TString &s2 : TString objects to compare. �Ƚ��ַ���
 *          LPCSTR s1         : �Ƚ��ַ���
 * @return: ������������ TRUE,ʧ�ܷ��� FALSE
 */
inline BOOL operator>=(LPCSTR s1, const TString& s2)
    { return s2.Compare(s1) <= 0; }

/*
 * @describe : ���ظ�ֵ������
 *
 *    The TString assignment (=) operator reinitializes an existing TString object 
 * with new data. If the destination string (that is, the left side) is already large 
 * enough to store the new data, no new memory allocation is performed. You should be 
 * aware that memory exceptions may occur whenever you use the assignment operator 
 * because new storage is often allocated to hold the resulting TString object.
 *
 * @param : const unsigned char * lpsz ��Ҫ���и�ֵ���ַ���
 * @return: TString &   ��������
 */
inline const TString& TString::operator=(const unsigned char* lpsz)
    { *this = (LPCSTR)lpsz; return *this; }

////////////////////////////////////////////////////////////////////////////////////////
/*
 * @describe : �ַ����Ƚ�
 *     �����ַ���������������. ���� < , <= , > , >= , != , == �ȵ�.
 *
 * @param : LPCSTR lpsz �ַ���
 * @return: �Ƚ�ֵ 
 *          Zero if the strings are identical, < 0 if this TString object is less than 
 *      lpsz, or > 0 if this TString object is greater than lpsz.
 */
inline long TString::Compare(LPCSTR lpsz) const
{ 
    ASSERT(lpsz!=NULL); 
    return strcmp(m_pchData, lpsz); 
}

/*
 * @describe : �ַ����Ƚ� CompareNoCase 
 *	 �ַ����ıȽϹ���.ʵ���ϵ����˱�׼�� strcasecmp ����,������Щƽ̨�� strcasecmp û��
 * �ṩ,�����Ҫ�ṩ�˹���.
 *
 * @param : LPCSTR lpsz
 * @return: �ȽϷ���ֵ.
 */
inline long TString::CompareNoCase(LPCSTR lpsz) const
{ 
	/* __VER@01.01.001 by devis */
    ASSERT(lpsz!=NULL);
	return strcasecmp(m_pchData, lpsz); 
}

/*
 * @describe : �ַ����Ƚ� CompareNoCase 
 *	 �ַ����ıȽϹ���.ʵ���ϵ����˱�׼�� strcasecmp ����,������Щƽ̨�� strcasecmp û��
 * �ṩ,�����Ҫ�ṩ�˹���.
 *
 * @param : LPCSTR lpsz: �Ƚ��ַ���
 *          long nLen  : ����ֵ.
 * @return: �ȽϷ���ֵ.
 */
inline long TString::CompareNoCase(LPCSTR lpsz,long nLen) const
{ 
	/* __VER@01.01.001 by devis */
    ASSERT(lpsz!=NULL); 
    return strncasecmp(m_pchData,lpsz,nLen); 
}

/*
 * @describe : �ַ����Ƚ�.
 *
 * @param : LPCSTR lpsz : The other string used for comparison
 * @return: �ȽϷ���ֵ
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
 * @describe : �ַ����Ƚ�, strcasecoll 
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
 * @describe : ȡָ��λ�õ��ַ�.
 * 	  A TCHAR containing the character at the specified position in the string.
 * You can think of a CString object as an array of characters. The GetAt member 
 * function returns a single character specified by an index number. The overloaded 
 * subscript ([]) operator is a convenient alias for GetAt.
 * 
 * @param : long nIndex �����������ֵ
 *          Zero-based index of the character in the TString object. The nIndex 
 *  parameter must be greater than or equal to 0 and less than the value returned 
 *  by GetLength.
 *
 * @return: char ���ַ�
 *    A TCHAR containing the character at the specified position in the string.
 */
inline char TString::operator[](long nIndex) const
{
    ASSERT(nIndex >= 0);
    ASSERT(nIndex < GetData()->nDataLength);
    return m_pchData[nIndex];
}

#endif
