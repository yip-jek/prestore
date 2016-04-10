//////////////////////////////////////////////////////////////////////////
/*
   Provided by 王俊川, Northeastern University (www.neu.edu.cn)
Email: blackdrn@sohu.com
This product is free for use.
 */
//////////////////////////////////////////////////////////////////////////

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \file Des.h
 *  \brief 定义Des算法的加码解密函数 */
/** \}*/

/** \example Des_Test.cpp
 *  \brief Des加密解密的示例程序  
 */


/** \enum 
 *  \brief 用于指定操作类型的枚举 */
enum    
{
	ENCRYPT, /**< 加密操作*/
	DECRYPT  /**< 解密操作 */
};

//enum    bool{false,true}; // if bool is not supported,use this or just replace with char
// and use 1 for true,0 for false;
//////////////////////////////////////////////////////////////////////////

// Type—ENCRYPT:加密,DECRYPT:解密
// 输出缓冲区(Out)的长度 >= ((datalen+7)/8)*8,即比datalen大的且是8的倍数的最小正整数
// In 可以= Out,此时加/解密后将覆盖输入缓冲区(In)的内容
// 当keylen>8时系统自动使用3次DES加/解密,否则使用标准DES加/解密.超过16字节后只取前16字节

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/

/** \brief Des加密/解密函数
 *  \param Out - 加密/解密结果输出缓冲区指针
 *  \param In - 加密/解密输入缓冲区指针,In 可以= Out,此时加/解密后将覆盖输入缓冲区(In)的内容
 *  \param datalen - 输入字符串的长度
 *  \param Key - 密钥
 *  \param keylen - 密钥长度
 *  \param Type - 操作类型,通过枚举值ENCRYPT指定加密操作,或者通过DECRYPT指定解密操作 
 *  \return 加密/解密结果是否成功 */
bool Des_Go(char *Out,char *In,long datalen,const char *Key,int keylen,bool Type = ENCRYPT);

/** \brief 计算Des加密之后的长度,可以用于分配输出缓冲 
 *  \param src_len - 输入的长度  
 *  \return 返回输入值上取整到最近的一个8的整数倍的值 */
long  Des_Length(long src_len);



/** \}*/

//////////////////////////////////////////////////////////////////////////

