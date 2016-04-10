
#ifndef _BASE64_
#define _BASE64_

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \file Base64.h
  * \brief ������Base64�������ĺ�����
  * \ref CCoder �ඨ��ĺ���������ļ�������������һ���ģ�����Ĺ��ܸ����ƣ�����ʹ��CCoder��*/
  

/** \brief Base64���� 
  * \param buftoenc Ҫ������Ļ�����ָ��
  * \param bufsize  Ҫ������Ļ���������
  * \param encbuf   �����Ľ�����������ָ��
  * \return ���ر������ĳ���,���ʧ��,����-1 */
int Base64Encod(char *buftoenc,int bufsize,char *encbuf);


/** \brief Base64����
  * \param buftodec Ҫ������Ļ�����ָ��
  * \param bufsize  Ҫ������Ļ���������
  * \param decbuf   �����������������ָ��
  * \return ���ؽ������ĳ���,���ʧ��,����-1 */
int Base64Decod(char *buftodec,int bufsize,char *decbuf);

/** \}*/

#endif
