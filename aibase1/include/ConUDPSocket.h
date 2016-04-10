#ifndef CONUDP_SOCKET_H
#define CONUDP_SOCKET_H

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif

#include "UDPSocket.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file ConUDPSocket.h
 * \brief 封装面向连接的UDP套接口
*/

/*! \class ConUDPSocket
 * \brief 面向连接的UDP套接口
 */
class ConUDPSocket : public UDPSocket {
public:
	/*! \brief 构造函数
	 */
	ConUDPSocket () : UDPSocket() {
		char self[] = "ConUDPSocket::ConUDPSocket"; trace(self);
	}
    
	/*! \brief 析构函数
	 */
	virtual ~ConUDPSocket () {
		char self[] = "ConUDPSocket::~ConUDPSocket"; trace(self);
	} 

	/*! \brief 连接对方地址
	 *  @param peer_addr_ 对方地址
	 */
	bool connect (const Address& peer_addr_);

	/*! \brief 断开连接
	 */
	void unconnect ();

	/*! \brief 从套接字读取指定字节数的数据
	 *  注意:这个方法不能在基类上调用，因为IPv4的read()和UDP的read()截然
	 *  不同.
	 *
	 *  @param buf_ 读取数据的缓冲区
	 *  @param size_ 期望读取的字节数
	 *  @return 返回读取的字节数，如果出现错误，返回-1, 并设置errno。读到0个字节是可能的.
	 */
	int read (char* buf_, const unsigned int size_);
	
	/*! \brief 阻塞式从套接字读入指定字节数的数据
	 *
	 *  @param buf_ 写缓冲区
	 *  @param size_ 期望写入的字节数.
	 */
	int write (const char* buf_ = NULL, const unsigned int size_ = 0);
};

END_SUNRISE_NAMESPACE

#endif // CONUDP_SOCKET_H



