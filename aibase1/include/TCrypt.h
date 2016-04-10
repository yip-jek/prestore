#if !defined(__CRYPT_H__)
#define __CRYPT_H__

/** \example TCrypt_Test.cpp
 *  \brief TCrypt��ʾ������ */

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \file TCrypt.h
 *  \brief TCrypt���ͷ�ļ� */
 
/** \class TCrypt
 *  \brief ���ܽ�����,ʹ����Des��Base64�ı������ */
/** \}*/
class TCrypt {
	public:
		/** \brief ���ܣ�����Des���ܣ�Ȼ��Խ����Base64����
		 * ����ĳ�����������:   (((strlen(in) + 7)*8)/8+2)/3*4
		 * \param in - ���������
		 * \param out - ���ܺ�õ�������, Ҫ�ȷ����㹻�ĳ���(���湫ʽ����õ��ĳ��ȸ��ֽڵ��ڴ�.
		 * \param outlen - ��Ϊ����ļ������ݵ��ֽ���.
		 * \return �ɹ�����true,ʧ�ܷ���false */
		static bool Encrypt(const  char * in, char* out, long outlen);
		
		/** \brief ���ܣ�����Base64���룬Ȼ����Des����
		 *  ����ĳ����������㣺 ((strlen(in) + 3)/4)*3 ������ȿ����Գ��������ʵ�ʳ���
		 *  \param in - ��������
		 *  \param out - ���ܺ�õ������ݣ�Ҫ�ȷ����㹻�ĳ���(���湫ʽ����õ��ĳ��ȸ��ֽڵ��ڴ档
		 *  \param outlen - ��Ϊ����Ľ������ݵ��ֽ���
		 *  \return �ɹ�����true��ʧ�ܷ���false */
		static bool Decrypt(const char * in, char* out, long outlen);
};

#else
class TCrypt;
#endif
