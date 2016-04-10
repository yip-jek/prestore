/* ==================================================
 *  Copyright(C), 1996-2001  WITCOM Tech. Co., Ltd.
 *
 *  FileName    : config.cpp
 *  Anthor      : Daemon
 *  Version     : 01.001
 *  Date        : 2002.01.07
 *
 *  Description : 配置文件由seg组成， seg由item组成。
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

/** \defgroup libsr 基础工具库(libsr)
 * \{*/
/** \file Ini.h
 *  \brief 读取配置文件类的头文件 */
/** \}*/

/* ReadConfig 的返回值
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
 * 定义一些长度
 */
#define ITEM_NAME_LEN       64    /* 项目名最大长度 */
#define ITEM_VALUE_LEN      512   /* 项目值最大长度 */


/** \defgroup libsr 基础工具库(libsr)
 * \{*/
/** \struct paraItem
 *  \brief Name-Value偶， 存放某个section里面的配置项.同一个section里面
 * 的Name-Value偶形成单向链表。
 */
/** \}*/
typedef struct paraItem
{
	char             m_szItemName[ITEM_NAME_LEN + 1];  /* parameter name */
	char             m_szItemValue[ITEM_VALUE_LEN + 1];/* parameter value */
	struct paraItem *m_pNext;                          /* point the next parameter item */
} ParaItem;

/** \defgroup libsr 基础工具库(libsr)
 * \{*/
/** \struct paraSeg
 *  \brief section结构体, 存放某section的信息，同一个文件里的所有section形成
 * 单向链表。
 */
/** \}*/
typedef struct paraSeg
{
    char            m_szSegName[ITEM_NAME_LEN + 1];
    ParaItem       *m_pItemHead;
    struct paraSeg *m_pNext;
} ParaSeg;

/** \defgroup libsr 基础工具库(libsr)
 * \{*/
/** \class CConfig
 *  \brief 读取配置文件类
 */
/** \example ini_test.cpp
 *  \brief CConfig类示例程序
 */
/** \}*/
class CConfig
{
public:
    CConfig();
    ~CConfig();

public:
    /** \brief 读一个ini配置文件
     *  \param szCfgFile 需要被读取的配置文件路径
     *  \return 成功:0 ,非0表示失败
     */
    int  ReadConfig(const char *szCfgFile);
    /** \brief 获取ini文件中某section的某项配置值,按字符串类型获取
     *  \param szSegame - section名称
     *  \param szItemName - 某配置项的名称
     *  \param szItemValue - 配置项的值
     *  \return 成功返回0,失败返回-1
     */
    int  GetItemValue(const char *szSegame, const char *szItemName, char *szItemValue);
    /** \brief 获取ini文件中某section的某项配置值,按int类型获取
     *  \param szSegame - section名称
     *  \param szItemName - 某配置项的名称
     *  \param iItemValue - 配置项的值 
     *  \return 成功返回0,失败返回-1
     */
    int  GetItemValue(const char *szSegame, const char *szItemName, int * iItemValue);
    /** \brief 获取ini文件中section的数目  
     *  \return Section个数
     */
    int  GetSegCount() const;
    /** \brief 按顺序号定位section 
     *  \param iSeqno 从0开始的section序号
     *  \return  
     */
    int  LocateSegbyPos(int iSeqno);
    
    /** \brief 按照的名字定位section
     *  \param szSegName 要定位到的section的名字
     *  \return 0表示成功,-1表示失败
     */
    int  LocateSegbyName(const char* szSegName);
    /** \brief 按照配置项的名字,获取当前section的某项配置值
     *  \param szItemName 配置项的名字
     *  \param szItemValue 配置项的值
     *  \return 0表示成功,-1表示失败 
     */ 
    int  GetItemValue(const char *szItemName, char *szItemValue);
     /** \brief 按照配置项的名字,获取当前section的某项配置值
     *  \param szItemName 配置项的名字
     *  \param iItemValue 配置项的值
     *  \return 0表示成功,-1表示失败 
     */ 
    int  GetItemValue(const char *szItemName, int *iItemValue);

    /** \brief 输出所有section和配置项的值 */
    void ParaPrint(void);

private:
    /*
     * 字符串比较，不区分大小写
     */
    int StrCaseCmp(const char *s1, const char *s2)
    {
/*
        const unsigned char *p1 = (const unsigned char *) s1;
        const unsigned char *p2 = (const unsigned char *) s2;
        unsigned char c1, c2;
*/
        /* 同一地址 */
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
