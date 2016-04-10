/*
 *    ��������ϵͳ��ʹ�õ�һЩͨ�ú���.
 * 
 * @Author  : 
 * @Date    : 2002.10.06
 * @Version : 01.01.002
 * @History : 
 */

/** \example TUtility_test.cpp
 *  \brief TUtility.h����ĺ����Ĳ��Գ��� */

/** \defgroup libsr �������߿�(libsr)
 *  \{*/
/** \file TUtility.h
 *  \brief �ṩ��һЩͨ�õĹ���,������
            �򵥵���־����,
            ʱ�䴦��,
            �ַ�������
            ������Ϣ��ӡ,
            ����ת��̨,
            �����в�������*/
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
 *  \brief �����ض���ʽ����������������Ľṹ */
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
/** \brief �ѵ�ǰ����ת���̨����*/
void toDaemon();
#endif


/** \brief ����־
 *  \param filename ָ����־�ļ���
 *  \return 1-�򿪳ɹ�,0-��ʧ��
 */
int log_open(const char *filename);
/** \brief �ر���־ */
void log_close(void);
/** \brief д��־
 *  \param code ��Ҫlog�Ķ�����Ϣ
 *  \param fmt ��Ҫ��ӡlog�ĸ�ʽ���ַ���,��printf��ͬ���÷� */
void log_print(const long code,const char *fmt,...);

/** \brief �ж��ַ����Ƿ�ȫ��������
 *  \param s ���жϵ��ַ���
 *  \return 1-�ǣ�0-���� */
int isnumstr(char *s);

/**
 * @brief ���ض��ַ��ָ��ַ���
 * @param c Ϊ�ָ���ַ�
 * @param s Ϊ��Ҫ���зָ���ַ���
 * @param buf ���ڴ�ŷָ������ݵ���ʱ�ռ�
 * @param buf_len buf�Ĵ�С����Ҫ >= strlen(s)
 * @param sp ���ڴ��ÿһ���ָ����ַ����ĵ�ַ������. ��:����ͨ��sp[0]�õ���һ���ַ���.
 * @param sp_len sp�ĸ���
 * @return  �ָ���Ԫ�ظ���.
 */
int srbc_split(char c, const char *s, char *buf, int buf_len, char **sp, int sp_len = 0);

/** \brief ��ʱ��(hhmiss)ת��Ϊ����
 *  \param time hhmiss��ʽ��ʱ��
 *  \return ����ʱ��ת���ɵ�������
 */
long time_to_sec(const char* time);

/** \brief ������ת��Ϊʱ��(hhmiss)
 *  \param s ������
 *  \param time ת��֮��hhmiss��ʽ��ʱ��,��Ҫ����ǰԤ�ȷ�������7byte�ռ�*/
void sec_to_time(long s,char* time);

/** \brief ��ȡ����"yymmdd"�������ں�
 *  \param date yymmdd��ʽ������
 *  \param week_day �ж�date���ڵ����ں�, 0Ϊ������,1Ϊ����һ,...,6Ϊ������ 
 *  \return �ɹ�����1,ʧ�ܷ���0
 */
int get_weekday(const char* date,int &week_day);

/** \brief ���ص�ǰ����ʱ��yyyy-mm-dd hh:mi:ss��ʽ
 *  \param sdatetime ���ڱ���yyyy-mm-dd hh:mi:ss��ʽ��ǰ���ڵĻ�����ָ��,
                     ����ǰ����Ԥ�ȷ�������20byte�Ŀռ�
 */
void curdate_time(char *sdatetime);

/** \brief ���ص�ǰ����ʱ��yyyymmddhhmiss��ʽ
 *  \param sdatetime ���ڱ���yyyymmddhhmiss��ʽ�ĵ�ǰ����ʱ��Ļ�����ָ��,
                     ����ǰ����Ԥ�ȷ�������15byte�Ŀռ�
 */
void curdatetime(char *sdatetime);

/** \brief ���ص�ǰ����yyyymmdd
 *  \param sdate ���ڱ���yyyymmdd��ʽ��ǰ���ڵĻ�����ָ��,
                 ����ǰ����Ԥ�ȷ�������9byte�Ŀռ�
 */
void curdate(char *sdate);

/** \brief ȥ��ԭ�ַ�����ͷ��ĳ��ָ���ַ�,�Ѵ��������浽Ŀ���ַ���
 *  \param src ԭ�ַ���
 *  \param des Ŀ���ַ���,��������㹻�Ŀռ�,��Ҫ����ԭ�ַ����Ŀռ�,�������Խ��.
 *  \return �ɹ�����0 
 */
int strltrim_x(const char* src, char* des, char trim_ch);

/** \brief ȥ��ԭ�ַ�����β��ĳ��ָ���ַ�,�Ѵ��������浽Ŀ���ַ���
 *  \param src ԭ�ַ���
 *  \param des Ŀ���ַ���,��������㹻�Ŀռ�,��Ҫ����ԭ�ַ����Ŀռ�,�������Խ��.
 *  \return �ɹ�����0 
 */
int strrtrim_x(const char* src, char* des, char trim_ch);

/** \brief ȥ��ԭ�ַ�����ͷβ��ĳ��ָ���ַ�,�Ѵ��������浽Ŀ���ַ���
 *  \param src ԭ�ַ���
 *  \param des Ŀ���ַ���,��������㹻�Ŀռ�,��Ҫ����ԭ�ַ����Ŀռ�,�������Խ��.
 *  \return �ɹ�����0 
 */
int strtrim_x(const char* src, char* des, char trim_ch);

/** \brief ȥ���ַ�����β���ո�
 *  \return �ɹ�����0 */
int strrtrim(char *s1);

/** \brief ȥ���ַ�����ͷ���ո�
 *  \return �ɹ�����0 */
int strltrim(char *s1);

/** \brief ȥ���ַ�����ͷβ�ո�
 *  \return �ɹ�����0 */
int strtrim(char *s1);

char *ltrim(char *, char = ' ');
char *rtrim(char *, char = ' ');
char *trim(char *, char = ' ');

/** \brief ���Դ�Сд�Ƚ������ַ��� */
int strcasecoll(const char *s1, const char *s2);

/** \brief �����ַ��������д�д��ĸ��Сд 
 *  \param s �����ַ���
 *  \return ����ַ���*/
char *strupr(char *s);

/** \brief �����ַ���������Сд��ĸ���д 
 *  \param s �����ַ���
 *  \return ����ַ���*/
char *strlwr(char *s);

/** \brief �����ַ�����ת 
 *  \param s �����ַ���
 *  \return ����ַ���*/
char *strrev(char *s);
#ifndef __linux__
/** \brief ��ԭ�ַ�����,���Դ�Сд���ַ���ƥ�����ָ�����ַ���,������ƥ�䵽��λ��ָ��(strstr�ĺ��Դ�Сд�汾)
 *  \param s1 ԭ�ַ���
 *  \param s2 �����ַ���
 *  \return ƥ�䵽�Ļ�,����ԭ�ַ�����ƥ�䵽λ�õ�ָ��,���򷵻�NULL
 */
char * strcasestr( char *s1, char *s2 );
#endif

/** \brief ��ָ���Ļ�������Χ��,��ָ����һ���ַ��滻���е���һ���ַ� */
void memreplace(void*,int len,const char sc,const char dc);

/** \brief ��yyyymmddhhmiss��ʽ�����ڴ�,ת����yyyy-mm-dd hh:mi:ss��ʽ�����ڴ�
 *  \param TDateTime ת������ַ���,����ǰ�����������20byte�ռ�
 *  \param SDateTime yyyymmddhhmiss��ʽ�������ַ���
 */
int GetDateTimeString( char * TDateTime, char * SDateTime );

/** \brief ���������в���,���浽һ�� \ref CALL_MSG �ṹ�� */
void GetCallArg(int argc,char* argv[],CALL_MSG *call_msg);

/** \brief ��ӡ��������ʾ��Ϣ,����_DEBUG�궨��ʱ���� */
void print_help(CALL_MSG* call_msg) ;

/** \brief ��ȡָ��yyyymmdd��ʽ���ڵ����ں�
 *  \param date yyyymmdd��ʽ������
 *  \return ���ں� 0-����һ,6-������
 */
int get_weekno(const char* date);

/** \brief ��yyyymmddhhmiss��ʽ������ʱ��,�ֳ�yyyymmdd��ʽ�����ڴ���hhmiss��ʽ��ʱ�䴮
 *  \param dt �����yyyymmddhhmis��ʽ������ʱ�䴮
 *  \param date ��ֺ��yyyymmdd���ڴ�,����ǰ�����ȷ�������9byte�ռ�
 *  \param time ��ֺ��hhmissʱ�䴮,����ǰ�����������7bytes�ռ� 
 */
void split_date_time(const char* dt, char* date, char* time) ;

#ifndef WIN32
//int strcoll(const char *s1, const char *s2);	ʹ�ú궨��
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

