//------------------------------------------------------------------------------
//                             IPv4Socket.h
//------------------------------------------------------------------------------
#ifndef IPV4_SOCKET_Hh
#define IPV4_SOCKET_Hh

#include "Prelude.h"
#include "Socket.h"
#include "Socketbuf.h"

/*! \file IPv4Socket.h
	\brief IPv4Socket类的封装含盖了AF_INET和AF_UNIX套接口, 底层的缓冲使用
	Socketbuf类实现.
*/

BEGIN_SUNRISE_NAMESPACE

/*! \class IPv4Socket
 * \brief IPv4Socket套接口类
 */
class IPv4Socket : public Socket 
{
public:
	static const int MAXTCPBUFSZ; /*!< TCP数据帧的最大值 */

	/// Default constructor
	IPv4Socket() 
		: m_path (0), m_rdbuf (new Socketbuf (this)) {
		trace_with_mask("IPv4Socket::IPv4Socket()",SOCKTRACE);
	}

	/*! \brief 构造函数
	 *  @param fd_ 文件描述字
	 */
	IPv4Socket(const int fd_) 
		: m_path (0), m_rdbuf (new Socketbuf (this)) {
		trace_with_mask("IPv4Socket::IPv4Socket(fd_)",SOCKTRACE);

		m_fd = fd_;
	}

	/*! \brief 析构函数
	 * 关闭套接口
	 */
	~IPv4Socket() {
		trace_with_mask("IPv4Socket::~IPv4Socket",SOCKTRACE);
		this->close ();

 		if (m_rdbuf != 0) {
			delete m_rdbuf;
		}
	}

	/*! \brief 刻隆函数
	 * 调用系统调用dup(2)创建底层描述字别名，并拷贝本对象内部状态, 返回
	 * 这个新的对象 .
	 */
	IPv4Socket* clone () const;
	

	/*!  \brief 创建socket.
	 * socket域类型是AF_INET(表示internet套接口)或者AF_UNIX(表示UNIX域套接口)
	 *
	 *  @param domain_  域类型(AF_INET或者AF_UNIX)
	 *  @return 如果创建成功返回true, 否则返回false.
	 */
	bool open(const int domain_);

	/*! \brief 关闭套接口
	 *  @return 成功返回true, 否则返回false.
	 */
	bool close();

	/*! \brief 创建连接
	 * 如果套接口配置为非阻塞方式, connect()方法的调用很可能返回false, 
	 * 并且errno被设定为EINPROGRESS.
	 * 请参考connect(2)的man pages.
	 *
	 *  @param address_ 要连接的地址
	 *  @return 连接成功返回true, 否则返回false.
	 */
	bool connect(const Address& address_);

	/*! \brief 绑定端口
	 * 这个方法在open()和accept()之间调用
	 *
	 *  @param my_address_ 要绑定的地址
	 *  @return 如果成功返回true, 否则返回false.
	 */
	virtual bool bind (const Address& my_address_);

	/*! \brief 在监听的端口上接受一个连接
	 *  返回的是一个'完全'的连接(表示对方套接口已经准备好数据传输，不需要调用
	 *  ::open()了). 这个方法将阻塞在连接上，直到有连接请求到来.
	 *  如果要避免阻塞，要先调用::select().
	 *
	 *  @return 如果成功，返回新的IPv4套接口对象指针.否则返回0.
	 */
	IPv4Socket* accept ();

	/*! \brief 读入指定字节数的数据, 存放在缓冲区
	 *
	 *  @param buf_ 接受数据的缓冲区
	 *  @param size_ 期望接收的字节数
	 *
	 *  @return 返回实际接收到的字节数, 如果出现错误返回-1, 并设定errno.
	 *  			如果对方关闭了连接，则返回0.
	 */
	int read (char* buf_, const unsigned int size_);

	/*! \brief 阻塞方式写入指定字节数的数据.
	 *
	 *  @param buf_ 发送缓冲区
	 *  @param size_ 指定的字节数
	 *  @return 返回实际写入的字节数，如果失败返回-1.
	 */
	int write (const char* buf_, const unsigned int size_);


	/*! \brief 获取套接字
	 */
	const int getHandler () const { return m_fd; }

	/*! \brief 获取套接口域类型
	 */
	const int getDomain () const { return m_type; }

	/*! \brief 返回关联于套接字的<B> Socketbuf </B>指针
	 * 这个函数用于想要从Socket对象中取得<B>Socketbuf</B>直接操作的情景.
	 *
	 * @return 返回关联于该对象的Streambuf(Socketbuf转换为Streambuf)指针.
	 */
	virtual Streambuf* rdbuf () { return m_rdbuf; }

	/*! \brief 设置新的套接字流, 返回原有的套接字流.缺省是返回NULL. 
	 *
	 *  @return 原有套接字流(Socketbuf).
	*/
	virtual Streambuf* rdbuf (Streambuf* sb_);

	/*! \brief 在Socet带有缓冲I/O的情况，这个函数并不真正调用socket api
	 * ，而是直接返回缓冲区中可用的字节数, 确保读取该字节不会出错并保证
	 * 不会访问底层套接字.
	 */
	virtual int in_avail () const { return m_rdbuf->in_avail (); }

private:
	IPv4Socket (const IPv4Socket&); /*!< 屏蔽拷贝构造函数 */
	IPv4Socket& operator= (const IPv4Socket&); /*!< 屏蔽赋值操作符 */

private:
	char*       m_path; /*!< UNIX域套接口 */
	Streambuf*  m_rdbuf; /*!< 底层的缓冲实现 */
};

END_SUNRISE_NAMESPACE

#endif // IPV4_SOCKET_Hh



