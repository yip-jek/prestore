/*
 *    定义了在系统中使用的一些通用函数.
 * 
 * @Author  : 
 * @Date    : 2002.10.06
 * @Version : 01.01.002
 * @History : 
 */

/** \example TUtility_test.cpp
 *  \brief TUtility.h定义的函数的测试程序 */

/** \defgroup libsr 基础工具库(libsr)
 *  \{*/
/** \file TUtility.h
 *  \brief 提供了一些通用的工具,包括了
            简单的日志处理,
            时间处理,
            字符串处理
            调试信息打印,
            进程转后台,
            命令行参数处理*/
/** \}*/
            
#ifndef __TUTILITY_H__
#define __TUTILITY_H__

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>


#ifdef __cplusplus
extern "C"
{
#endif

/** \struct CALL_MSG
 *  \brief 保存特定格式的命令行输入参数的结构 */
typedef struct CALL_MSG
{
	char 		name[128];
	int 		taskid ;
	int 		ccm_id ;	
	char		date[14+1];
	char		module[20+1] ;
	int		seq;
	char		version[20+1];
	int		run ;
} ;

#ifndef WIN32
/** \brief 把当前进程转入后台进程*/
void toDaemon();
#endif


/** \brief 打开日志
 *  \param filename 指定日志文件名
 *  \return 1-打开成功,0-打开失败
 */
int log_open(const char *filename);
/** \brief 关闭日志 */
void log_close(void);
/** \brief 写日志
 *  \param code 需要log的额外信息
 *  \param fmt 需要打印log的格式化字符串,和printf相同的用法 */
void log_print(const long code,const char *fmt,...);

/** \brief 判断字符串是否全部是数字
 *  \param s 被判断的字符串
 *  \return 1-是，0-不是 */
int isnumstr(char *s);

/**
 * @brief 按特定字符分割字符串
 * @param c 为分割的字符
 * @param s 为需要进行分割的字符串
 * @param buf 用于存放分割后的数据的临时空间
 * @param buf_len buf的大小，需要 >= strlen(s)
 * @param sp 用于存放每一个分割后的字符串的地址的数组. 如:可以通过sp[0]得到第一个字符串.
 * @param sp_len sp的个数
 * @return  分割后的元素个数.
 */
int srbc_split(char c, const char *s, char *buf, int buf_len, char **sp, int sp_len = 0);

/** \brief 将时间(hhmiss)转换为秒数
 *  \param time hhmiss格式的时间
 *  \return 输入时间转换成的秒钟数
 */
long time_to_sec(const char* time);

/** \brief 将秒数转换为时间(hhmiss)
 *  \param s 秒钟数
 *  \param time 转换之后hhmiss格式的时间,需要调用前预先分配至少7byte空间*/
void sec_to_time(long s,char* time);

/** \brief 获取日期"yymmdd"所在星期号
 *  \param date yymmdd格式的日期
 *  \param week_day 判断date所在的星期号, 0为星期天,1为星期一,...,6为星期六 
 *  \return 成功返回1,失败返回0
 */
int get_weekday(const char* date,int &week_day);

/** \brief 返回当前日期时间yyyy-mm-dd hh:mi:ss格式
 *  \param sdatetime 用于保存yyyy-mm-dd hh:mi:ss格式当前日期的缓冲区指针,
                     调用前必须预先分配至少20byte的空间
 */
void curdate_time(char *sdatetime);

/** \brief 返回当前日期时间yyyymmddhhmiss格式
 *  \param sdatetime 用于保存yyyymmddhhmiss格式的当前日期时间的缓冲区指针,
                     调用前必须预先分配至少15byte的空间
 */
void curdatetime(char *sdatetime);

/** \brief 返回当前日期yyyymmdd
 *  \param sdate 用于保存yyyymmdd格式当前日期的缓冲区指针,
                 调用前必须预先分配至少9byte的空间
 */
void curdate(char *sdate);

/** \brief 去掉原字符串的头部某个指定字符,把处理结果保存到目标字符串
 *  \param src 原字符串
 *  \param des 目标字符串,必须分配足够的空间,不要少于原字符串的空间,否则可能越界.
 *  \return 成功返回0 
 */
int strltrim_x(const char* src, char* des, char trim_ch);

/** \brief 去掉原字符串的尾部某个指定字符,把处理结果保存到目标字符串
 *  \param src 原字符串
 *  \param des 目标字符串,必须分配足够的空间,不要少于原字符串的空间,否则可能越界.
 *  \return 成功返回0 
 */
int strrtrim_x(const char* src, char* des, char trim_ch);

/** \brief 去掉原字符串的头尾部某个指定字符,把处理结果保存到目标字符串
 *  \param src 原字符串
 *  \param des 目标字符串,必须分配足够的空间,不要少于原字符串的空间,否则可能越界.
 *  \return 成功返回0 
 */
int strtrim_x(const char* src, char* des, char trim_ch);

/** \brief 去掉字符串的尾部空格
 *  \return 成功返回0 */
int strrtrim(char *s1);

/** \brief 去掉字符串的头部空格
 *  \return 成功返回0 */
int strltrim(char *s1);

/** \brief 去掉字符串的头尾空格
 *  \return 成功返回0 */
int strtrim(char *s1);

char *ltrim(char *, char = ' ');
char *rtrim(char *, char = ' ');
char *trim(char *, char = ' ');

/** \brief 忽略大小写比较两个字符串 */
int strcasecoll(const char *s1, const char *s2);

/** \brief 输入字符串的所有大写字母变小写 
 *  \param s 输入字符串
 *  \return 输出字符串*/
char *strupr(char *s);

/** \brief 输入字符串的所有小写字母变大写 
 *  \param s 输入字符串
 *  \return 输出字符串*/
char *strlwr(char *s);

/** \brief 输入字符串反转 
 *  \param s 输入字符串
 *  \return 输出字符串*/
char *strrev(char *s);
#ifndef __linux__
/** \brief 在原字符串中,忽略大小写的字符串匹配查找指定的字符串,并返回匹配到的位置指针(strstr的忽略大小写版本)
 *  \param s1 原字符串
 *  \param s2 查找字符串
 *  \return 匹配到的话,返回原字符串中匹配到位置的指针,否则返回NULL
 */
char * strcasestr( char *s1, char *s2 );
#endif

/** \brief 在指定的缓冲区范围内,用指定的一个字符替换所有的另一个字符 */
void memreplace(void*,int len,const char sc,const char dc);

/** \brief 把yyyymmddhhmiss格式的日期串,转换成yyyy-mm-dd hh:mi:ss格式的日期串
 *  \param TDateTime 转换后的字符串,调用前必须分配至少20byte空间
 *  \param SDateTime yyyymmddhhmiss格式的输入字符串
 */
int GetDateTimeString( char * TDateTime, char * SDateTime );

/** \brief 处理命令行参数,保存到一个 \ref CALL_MSG 结构中 */
void GetCallArg(int argc,char* argv[],CALL_MSG *call_msg);

/** \brief 打印命令行提示信息,仅在_DEBUG宏定义时有用 */
void print_help(CALL_MSG* call_msg) ;

/** \brief 获取指定yyyymmdd格式日期的星期号
 *  \param date yyyymmdd格式的日期
 *  \return 星期号 0-星期一,6-星期天
 */
int get_weekno(const char* date);

/** \brief 把yyyymmddhhmiss格式的日期时间,分成yyyymmdd格式的日期串和hhmiss格式的时间串
 *  \param dt 输入的yyyymmddhhmis格式的日期时间串
 *  \param date 拆分后的yyyymmdd日期串,调用前必须先分配至少9byte空间
 *  \param time 拆分后的hhmiss时间串,调用前必须分配至少7bytes空间 
 */
void split_date_time(const char* dt, char* date, char* time) ;

#ifndef WIN32
//int strcoll(const char *s1, const char *s2);	使用宏定义
#define strcoll strcmp
char *strupr(char *s);
char *strlwr(char *s);
char *strrev(char *s);
int filelength(int filedes);
void date_convert(const char* src, char* dest, int len);
void datetime_convert(const char* src, char* dest, int len);
void datetime_split(const char* src, char* dest, char* dest2, int len);
long long mystrtoll(const char* src);
double mystrtod(const char* str) ;
#else
int strncasecmp(const char *s1, const char *s2,size_t nLen);
#endif

#ifdef _SCO
int strncasecmp(const char *s1, const char *s2,size_t nLen);
#endif

#ifdef _DEBUG
    #define g_printf printf
#else
    #define g_printf 
#endif

#ifdef __cplusplus
}
#endif

#endif

