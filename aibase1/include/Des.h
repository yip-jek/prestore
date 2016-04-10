//////////////////////////////////////////////////////////////////////////
/*
   Provided by ������, Northeastern University (www.neu.edu.cn)
Email: blackdrn@sohu.com
This product is free for use.
 */
//////////////////////////////////////////////////////////////////////////

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \file Des.h
 *  \brief ����Des�㷨�ļ�����ܺ��� */
/** \}*/

/** \example Des_Test.cpp
 *  \brief Des���ܽ��ܵ�ʾ������  
 */


/** \enum 
 *  \brief ����ָ���������͵�ö�� */
enum    
{
	ENCRYPT, /**< ���ܲ���*/
	DECRYPT  /**< ���ܲ��� */
};

//enum    bool{false,true}; // if bool is not supported,use this or just replace with char
// and use 1 for true,0 for false;
//////////////////////////////////////////////////////////////////////////

// Type��ENCRYPT:����,DECRYPT:����
// ���������(Out)�ĳ��� >= ((datalen+7)/8)*8,����datalen�������8�ı�������С������
// In ����= Out,��ʱ��/���ܺ󽫸������뻺����(In)������
// ��keylen>8ʱϵͳ�Զ�ʹ��3��DES��/����,����ʹ�ñ�׼DES��/����.����16�ֽں�ֻȡǰ16�ֽ�

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/

/** \brief Des����/���ܺ���
 *  \param Out - ����/���ܽ�����������ָ��
 *  \param In - ����/�������뻺����ָ��,In ����= Out,��ʱ��/���ܺ󽫸������뻺����(In)������
 *  \param datalen - �����ַ����ĳ���
 *  \param Key - ��Կ
 *  \param keylen - ��Կ����
 *  \param Type - ��������,ͨ��ö��ֵENCRYPTָ�����ܲ���,����ͨ��DECRYPTָ�����ܲ��� 
 *  \return ����/���ܽ���Ƿ�ɹ� */
bool Des_Go(char *Out,char *In,long datalen,const char *Key,int keylen,bool Type = ENCRYPT);

/** \brief ����Des����֮��ĳ���,�������ڷ���������� 
 *  \param src_len - ����ĳ���  
 *  \return ��������ֵ��ȡ���������һ��8����������ֵ */
long  Des_Length(long src_len);



/** \}*/

//////////////////////////////////////////////////////////////////////////

