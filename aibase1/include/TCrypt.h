#if !defined(__CRYPT_H__)
#define __CRYPT_H__

/** \example TCrypt_Test.cpp
 *  \brief TCrypt类示例程序 */

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \file TCrypt.h
 *  \brief TCrypt类的头文件 */
 
/** \class TCrypt
 *  \brief 加密解密类,使用了Des和Base64的编码解码 */
/** \}*/
class TCrypt {
	public:
		/** \brief 加密，先用Des加密，然后对结果用Base64编码
		 * 输出的长度这样计算:   (((strlen(in) + 7)*8)/8+2)/3*4
		 * \param in - 输入的数据
		 * \param out - 加密后得到的数据, 要先分配足够的长度(上面公式计算得到的长度个字节的内存.
		 * \param outlen - 作为输出的加密数据的字节数.
		 * \return 成功返回true,失败返回false */
		static bool Encrypt(const  char * in, char* out, long outlen);
		
		/** \brief 解密，先用Base64解码，然后用Des解密
		 *  输出的长度这样计算： ((strlen(in) + 3)/4)*3 这个长度可能稍长于输出的实际长度
		 *  \param in - 输入数据
		 *  \param out - 解密后得到的数据，要先分配足够的长度(上面公式计算得到的长度个字节的内存。
		 *  \param outlen - 作为输出的解密数据的字节数
		 *  \return 成功返回true，失败返回false */
		static bool Decrypt(const char * in, char* out, long outlen);
};

#else
class TCrypt;
#endif
