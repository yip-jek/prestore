//------------------------------------------------------------------------------
//                         UDPSocket.h
//------------------------------------------------------------------------------
#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include "Prelude.h"
#include "Socket.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file UDPSocket.h 
 *
 * \brief UDPSocket是UNIX domain套接字基础类.
 * 特定类型的domain套接字(ConUDPSocket和UnConUDPSocket)从这个类衍生.
 */

/*! \class UDPSocket
 * \brief UDPSocket实现UNIX domain套接字基础接口
 */
class UDPSocket : public Socket {
public:
	/*! \brief 缺省构造函数
	 */
	UDPSocket () ;

	/*! \brief 构造函数
	 *  @param fd_ 文件描述字
	 */
	UDPSocket (const int fd_) ;

	/*! \brief 析构函数
	 * 关闭连接
	 */
	virtual ~UDPSocket ();

	/*! \brief 创建socket. internet socket的类型用AF_INET表示，UNIX域socket
	 * 则用AF_UNIX表示
	 *
	 *  @param domain_ 域(AF_INET或者AF_UNIX)
	 *  @return 如果创建成功，返回true, 否则返回false.
	 */
	bool open (const int domain_);

	/*! \brief 关闭socket连接
	 *  @return 如果成功返回true, 否则返回false.
	 */
	bool close ();

	/*! \brief 绑定
	 *
	 *  @param my_address_ 要绑定的地址
	 */
	bool bind (const Address& my_address_);

	/*! \brief 获取socket文件描述字
	 */
	const int getHandler() const;

	/*! \brief 获取socket域类型(AF_INET或者AF_UNIX)
	 */
	const int getDomain() const;

protected:
	/*! \brief 设置文件描述字
	 */
	void setHandler(const int fd_);

	/*! \brief 设置socket域类型(AF_INET或者AF_UNIX)
	 */
	void setDomain(const int type_);
};

END_SUNRISE_NAMESPACE

#endif // UDP_SOCKET_H


