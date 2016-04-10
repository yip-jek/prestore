//------------------------------------------------------------------------------
//                        UnConUDPSocket.h
//------------------------------------------------------------------------------
#ifndef UNCONUPD_SOCKET_H
#define UNCONUPD_SOCKET_H

#include "Prelude.h"
#include "UDPSocket.h"

BEGIN_SUNRISE_NAMESPACE

/** \file UnConUDPSocket.h 
 *  \brief 封装面向非连接的UDP套接口
 */

/*! \class UnConUDPSocket
 * \brief 面向非连接的UDP套接口
 */
class UnConUDPSocket : public UDPSocket {
public:
	/*! \brief 构造函数
	 */
	UnConUDPSocket();
    
	/*! \brief 析构函数
	 */
	~UnConUDPSocket();

	/*! \brief recvfrom()方法在非连接模式套接口上接收信息. 
	 *
	 *  @param buf_ 存储接收信息的缓冲
	 *  @param size_ 缓冲区长度
	 *  @param peer_addr_ 对方地址
	 *  @return 返回实际取得的字节数
	 */
	int recvfrom (char* buf_, int size_, Address* peer_addr_);
	
	/*! \brief sendto()方法在非连接模式套接口上发送信息
	 *
	 *  @param buf_ 发送信息的缓冲区
	 *  @param size_ 缓冲区的长度
	 *  @param dest_addr_ 目的地址
	 *  @return 发送成功，则返回实际发送的字节数, 否则返回-1.
	 */
	int sendto (const char* buf_, 
				const unsigned int size_, 
				const Address* dest_addr_);

	/*! \brief 在Socet带有缓冲I/O的情况，这个函数并不真正调用socket api
	 * ，而是直接返回缓冲区(Socketbuf)中可用的字节数, 确保读取该字节不会出错并保证
	 * 不会访问底层套接字. 因为UnConUDPSocket面向非连接的，没有缓冲区, 所以这个类的
	 * 实现是总是返回0.
	 */
	virtual int in_avail () const { return 0; }
};

UnConUDPSocket::
UnConUDPSocket () : UDPSocket() 
{
	trace_with_mask ("UnConUDPSocket::UnConUDPSocket", SOCKTRACE);
}
    
UnConUDPSocket::
~UnConUDPSocket() 
{
	trace_with_mask ("UnConUDPSocket::~UnConUDPSocket", SOCKTRACE);
}

END_SUNRISE_NAMESPACE

#endif // UNCONUPD_SOCKET_H



