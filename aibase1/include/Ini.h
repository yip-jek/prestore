/* ==================================================
 *  Copyright(C), 1996-2001  WITCOM Tech. Co., Ltd.
 *
 *  FileName    : config.cpp
 *  Anthor      : Daemon
 *  Version     : 01.001
 *  Date        : 2002.01.07
 *
 *  Description : �����ļ���seg��ɣ� seg��item��ɡ�
 *
 *  History     :
 * ================================================== */
#if !defined (__INI_H__)
#define __INI_H__

//sunrise headers
#include "Prelude.h"

#include <stdio.h>
#include <ctype.h>
#include <strings.h>


BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr �������߿�(libsr)
 * \{*/
/** \file Ini.h
 *  \brief ��ȡ�����ļ����ͷ�ļ� */
/** \}*/

/* ReadConfig �ķ���ֵ
 * 
 */
#define E_SUCC      0
#define E_FILE      1
#define E_PARA      2
#define E_ITEM      4
#define E_SEG       5
#define E_NEED      6

/*
 * error code for cfgerror variable, only the latest error is holded
 */
#define E_VALUE     0x10        /* invalid value */
#define E_ARRAY     0x11        /* array overflow */
#define E_SIZE      0x12        /* variable overflow */
#define E_DUP       0x13        /* duplicated */
#define E_ACCES     0x14        /* file/directory access error */
#define E_NOENT     0x15        /* file/directory not exist */

/*
 * type of phrasing of config file line
 */
#define E_NORMAL    0x20
#define E_SEGFLAG   0x21
#define E_ENDEOF    0x22
#define E_SPACE     0x23
#define E_COMMENT   0x24
#define E_DEFAULT   0x25
#define E_NOVAL     0x26
#define E_SYNTAX    0x27

/*
 * ����һЩ����
 */
#define ITEM_NAME_LEN       64    /* ��Ŀ����󳤶� */
#define ITEM_VALUE_LEN      512   /* ��Ŀֵ��󳤶� */


/** \defgroup libsr �������߿�(libsr)
 * \{*/
/** \struct paraItem
 *  \brief Name-Valueż�� ���ĳ��section�����������.ͬһ��section����
 * ��Name-Valueż�γɵ�������
 */
/** \}*/
typedef struct paraItem
{
	char             m_szItemName[ITEM_NAME_LEN + 1];  /* parameter name */
	char             m_szItemValue[ITEM_VALUE_LEN + 1];/* parameter value */
	struct paraItem *m_pNext;                          /* point the next parameter item */
} ParaItem;

/** \defgroup libsr �������߿�(libsr)
 * \{*/
/** \struct paraSeg
 *  \brief section�ṹ��, ���ĳsection����Ϣ��ͬһ���ļ��������section�γ�
 * ��������
 */
/** \}*/
typedef struct paraSeg
{
    char            m_szSegName[ITEM_NAME_LEN + 1];
    ParaItem       *m_pItemHead;
    struct paraSeg *m_pNext;
} ParaSeg;

/** \defgroup libsr �������߿�(libsr)
 * \{*/
/** \class CConfig
 *  \brief ��ȡ�����ļ���
 */
/** \example ini_test.cpp
 *  \brief CConfig��ʾ������
 */
/** \}*/
class CConfig
{
public:
    CConfig();
    ~CConfig();

public:
    /** \brief ��һ��ini�����ļ�
     *  \param szCfgFile ��Ҫ����ȡ�������ļ�·��
     *  \return �ɹ�:0 ,��0��ʾʧ��
     */
    int  ReadConfig(const char *szCfgFile);
    /** \brief ��ȡini�ļ���ĳsection��ĳ������ֵ,���ַ������ͻ�ȡ
     *  \param szSegame - section����
     *  \param szItemName - ĳ�����������
     *  \param szItemValue - �������ֵ
     *  \return �ɹ�����0,ʧ�ܷ���-1
     */
    int  GetItemValue(const char *szSegame, const char *szItemName, char *szItemValue);
    /** \brief ��ȡini�ļ���ĳsection��ĳ������ֵ,��int���ͻ�ȡ
     *  \param szSegame - section����
     *  \param szItemName - ĳ�����������
     *  \param iItemValue - �������ֵ 
     *  \return �ɹ�����0,ʧ�ܷ���-1
     */
    int  GetItemValue(const char *szSegame, const char *szItemName, int * iItemValue);
    /** \brief ��ȡini�ļ���section����Ŀ  
     *  \return Section����
     */
    int  GetSegCount() const;
    /** \brief ��˳��Ŷ�λsection 
     *  \param iSeqno ��0��ʼ��section���
     *  \return  
     */
    int  LocateSegbyPos(int iSeqno);
    
    /** \brief ���յ����ֶ�λsection
     *  \param szSegName Ҫ��λ����section������
     *  \return 0��ʾ�ɹ�,-1��ʾʧ��
     */
    int  LocateSegbyName(const char* szSegName);
    /** \brief ���������������,��ȡ��ǰsection��ĳ������ֵ
     *  \param szItemName �����������
     *  \param szItemValue �������ֵ
     *  \return 0��ʾ�ɹ�,-1��ʾʧ�� 
     */ 
    int  GetItemValue(const char *szItemName, char *szItemValue);
     /** \brief ���������������,��ȡ��ǰsection��ĳ������ֵ
     *  \param szItemName �����������
     *  \param iItemValue �������ֵ
     *  \return 0��ʾ�ɹ�,-1��ʾʧ�� 
     */ 
    int  GetItemValue(const char *szItemName, int *iItemValue);

    /** \brief �������section���������ֵ */
    void ParaPrint(void);

private:
    /*
     * �ַ����Ƚϣ������ִ�Сд
     */
    int StrCaseCmp(const char *s1, const char *s2)
    {
/*
        const unsigned char *p1 = (const unsigned char *) s1;
        const unsigned char *p2 = (const unsigned char *) s2;
        unsigned char c1, c2;
*/
        /* ͬһ��ַ */
/*
        if (p1 == p2)
            return 0;
*/
		return (strcasecmp(s1, s2));
/*
        do
        {
            c1 = tolower(*p1++);
            c2 = tolower(*p2++);
            if (c1 == '\0')
                break;
        } while (c1 == c2);

        return c1 - c2;
*/
    }

    int   NextLine(FILE *fp, char *szName, char *szValue);
	void  MultiLine(FILE *fp, char *szBuff, int BufSize);

    char *FindAfter(char *pStart);
    char *FindBefore(char *pStart, char *pEnd);

    int   InsertItem(ParaItem *pNewItem);
    int   AddItem(const char *szName, const char *szValue);

    int   AddSeg(const char *szSegName);

    void  ParaItemClean(ParaItem *pRootItem);
    void  ParaSegClean(ParaSeg *pRootSeg);

private:
    ParaSeg *m_pSegHead;
    ParaSeg *m_pSegCurrent;
    int      m_iSegCount;
};

END_SUNRISE_NAMESPACE

#endif /* _INI_H_ */
