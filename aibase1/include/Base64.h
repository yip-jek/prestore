
#ifndef _BASE64_
#define _BASE64_

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \file Base64.h
  * \brief 定义了Base64编码解码的函数，
  * \ref CCoder 类定义的函数和这个文件的两个函数是一样的，该类的功能更完善，建议使用CCoder类*/
  

/** \brief Base64编码 
  * \param buftoenc 要被编码的缓冲区指针
  * \param bufsize  要被编码的缓冲区长度
  * \param encbuf   编码后的结果输出缓冲区指针
  * \return 返回编码结果的长度,如果失败,返回-1 */
int Base64Encod(char *buftoenc,int bufsize,char *encbuf);


/** \brief Base64解码
  * \param buftodec 要被解码的缓冲区指针
  * \param bufsize  要被解码的缓冲区长度
  * \param decbuf   解码结果的输出缓冲区指针
  * \return 返回解码结果的长度,如果失败,返回-1 */
int Base64Decod(char *buftodec,int bufsize,char *decbuf);

/** \}*/

#endif
