//------------------------------------------------------------------------------
//                             UNIXAddress.h
//------------------------------------------------------------------------------
#ifndef UNIX_ADDRESS_H
#define UNIX_ADDRESS_H

#include "Prelude.h"
#include "Address.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file UNIXAddress.h 
 * UNIX domain套接字封装
 */

/*! \class UNIXAddress
 * \brief UNIX domain套接字封装
 */
class UNIXAddress : public Address {
public:
	/*! \brief 构造函数
	 * @param socket_name_ UNIX路径
	 */
	UNIXAddress (const char * socket_name_);

	/*! \brief 拷贝构造函数
	 * @param socket_address_ 右值
	 */
	UNIXAddress (SA* socket_address_);

	/*! \brief 析构函数
	 */
	virtual ~UNIXAddress ();

	/*! \brief 获取地址结构尺寸
	 */
	const int getLength () const;

	/*! \brief 获取地址结构实例
	 */
	SA* getAddress () const;
	
private:
	/*! \brief UNIX地址结构实例
	 */
	SA_UN m_address;
};

END_SUNRISE_NAMESPACE

#endif /* UNIX_ADDRESS_H */
