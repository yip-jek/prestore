//------------------------------------------------------------------------------
//                               Socket.h
//------------------------------------------------------------------------------
//  This class is a direct derivative from my Unix Network Programming
//  class work on generalizing OO network interfaces.
//------------------------------------------------------------------------------

#ifndef SOCKET_H
#define SOCKET_H

#include <sys/stat.h>
#include <sys/time.h>
#include <limits.h>		// for INT_MAX
#include <stdio.h>		// for EOF
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>		// for fcntl(2)

#ifdef linux
#  include <sys/ioctl.h>	// ioctl(2)
#endif
#ifdef sun			// ioctl(2)
#  include <unistd.h>
#  include <stropts.h>
#  include <sys/filio.h>
#endif
#ifdef aix
#include <stropts.h>
#endif

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif

#include "Address.h"

/*! \example echos.cpp
 * \brief echo服务器
 */

BEGIN_SUNRISE_NAMESPACE

class Streambuf;		// Forward declaration

/*! \file Socket.h 
 * 套接字封装类
 */

/*! \class Socket
 * \brief 套接字封装类
 */
class Socket {
public:
	static const int PGSIZE; /*!< 内核页的大小 */

	/*! \enum io_state_t 
	 *  \brief io状态位枚举: goodbit, eofbit, failbit, badbit.
	 */
	enum io_state_t { 
		goodbit = 0,  /*!< 标志socket准备好使用 */
		eofbit  = 1,  /*!< 标志输入操作到达输入流的末端 */
		failbit = 2,  /*!< 标志输入操作读入期望的字符失败，或者输出操作产生要求的字符失败 */
		badbit  = 4   /*!< 标志输入或者输出序列的数据失去完整性(例如一个不可恢复的文件读错误). */
	};

	typedef int iostate;
	typedef unsigned char IOState;

	/*! \enum opt_t
	 *  \brief 套接字选项
	 */
	enum opt_t { 
		reuseaddr,   /*!< 允许本地地址重用. */
		rcvlowat,    /*!< 至使select(3)返回可读("readable"), 套接字接收缓冲被要求的最低水位量.
							TCP和UDP套接字缺省是1(注: POSIX.1g并没有要求这个选项) */
		sndlowat,    /*!< 至使select返回可写("writable"), 套接字发送缓冲被要求的最低水位量.
							对于TCP,缺省值是2048, 对于UDP则总是处于writable状态. (注: POSIX.1g
							并没有要求这个选项) */
		blocking,    /*!< 对于io操作, 阻塞是缺省的. 输入输出操作在各自的缓冲区没有数据时，将
							进入睡眠状态，知道有数据到达。 */
		nonblocking  /**< 设置套接字使用非阻塞模式.
						对于输入操作，如果条件还没有满足(对于TCP，至少有1个字节的数据.对于UDP,则是
						一个完整的数据报), 操作不会被阻塞，而是立即返回，同时error被设为EWOULDBLOCK.
	
						对于输出操作, 如果条件还没有满足, 情况也是这样，不阻塞而是立即返回，并且
						error被设为EWOULDBLOCK */
	};

	/*! \brief 构造函数
	 */
	Socket();

	/*! \brief 析构函数
	 */
	virtual ~Socket();

	/*! \brief 打开一个套接字
	 */
	virtual bool open(const int domain_) =0;

	/*! \brief 关闭套接字
	 */
	virtual bool close() =0;

	/*! \brief 连接
	    @param address_ 连接的地址
	 */
	virtual bool connect (const Address& address_);

	/*! \brief 绑定
	    @param my_address_ 要绑定的地址
	    @return 绑定成功返回true. 否则返回false.
	 */
	virtual bool bind (const Address& my_address_) =0;

	/*! \brief 写指定字节数的数据到套接字
	    @param buf_  要发送的数据包指针
	    @param size_ 要发送的字节数
	 */
	virtual int write (const char* buf_, const unsigned int size_); 

	/*! \brief 获取在接收缓冲内可用的数据字节数.
	 */
	int getBytesAvail (void) const;

	/*! \brief 在套接字上读入期望数目的数据.
	    @param buf_ 保存接收数据的缓冲
	    @param size_ 期望接收的字节数
	 */
	virtual int read (char* buf_, const unsigned int size_);

	/*! \brief 提取指定数目的数据并丢弃之.如果没有参数，则持续读入直到EOF.
	 * 提取方式如下: <P>

	    <table border width=75% cellpadding=3>
	    <td><b> n_</b></td> <td><b> delim_ </b></td>
	    <td><b> Action </b></td>
	    <tr>
	    <td aling=center> 1 </td><td> EOF </td>
	    <td> 读入并丢弃1个字节 </td> 
	    </tr>
	    <tr>
	    <td align=center> k </td><td> EOF </td>
	    <td> 读入并丢弃最多1个字节 </td></tr>
	    <tr>
	    <td align=center> INT_MAX </td><td> EOF </td>
	    <td> 读入并丢弃直到EOF</td></tr>
	    <tr>
	    <td align=center> INT_MAX </td><td> 'c' </td>
	    <td> 读入并丢弃直到EOF或者遇到'c' </td>
	    </tr>
	    <tr>
	    <td align=center>    k    </td><td> 'c' </td>
	    <td> 读入并丢弃最多k个字节，如果遇到'c'则停止. </td>
	    </tr>
	    </table><p>

	   @param n_ 要丢弃的字节数(缺省为INT_MAX)
	   @param delim_ 截止符 (缺省为EOF)
	   @return 返回丢弃的字节数
	 */
	int ignore (int n_ = INT_MAX, int delim_ = EOF);

	/*! \brief 获取文件描述字
	 */
	virtual const int getHandler() const = 0;

	/*! \brief 获取套接字类型(AF_INET或者AF_UNIX)
	 */
	virtual const int getDomain() const = 0;

	/*! \brief 返回关联于套接字的<B> Streambuf </B>指针
	 * 这个函数用于想要从Socket对象中取得<B>Streambuf</B>直接操作的情景.
	 * 缺省是返回NULL
	 *
	 * @return NULL
	 */
	virtual Streambuf* rdbuf () { return 0; }

	/*! \brief 设置新的套接字流, 返回原有的套接字流.缺省是返回NULL. 
	    @return 原有套接字流(Socketbuf).
	*/
	virtual Streambuf* rdbuf (Streambuf* /*sb_*/) { return 0; }

	/*! \brief 在Socet带有缓冲I/O的情况下，这个函数并不真正调用socket api
	 * ，而是直接返回缓冲区中可用的字节数, 确保读取该字节不会出错并保证
	 * 不会访问底层套接字.
	 */
	virtual int in_avail () const = 0;

	/*! \brief 调用synchronizing函数(<B>rdbuf->pubsync()</B>). 这个操作
	 * 把套接字输出缓冲区剩余的输出数据冲刷掉.
	 */
	virtual Socket& flush ();

	/*! \brief 套接字选项使能.
	    @param opt_ 选项名
	    @return 成功返回true,否则返回false.
	*/
	bool turnOptionOn (opt_t opt_);

	/*! \brief 关闭某套接字选项
	    @param opt_ 选项名
	    @return 成功返回true,否则返回false.
	*/
	bool turnOptionOff (opt_t opt_);

	/*! \brief 设定套接字某选项值

	    @param opt_ 选项名
	    @param arg_ 要设定的值 (对于布尔值: 0 - 关闭, 1 - 使能).
	    @return true on success_; false if error
	*/
	bool setOption (opt_t opt_, int arg_);

	/*! \brief 获取套接字选项当前值
	    @param opt_ 选项名
	    @return 如果成功,返回选项值(对于布尔值: 0 - 关闭, 1 - 使能); 否则返回-1.
	*/
	int  getOption (opt_t opt_) const;

	operator void* () const;

	/*! \brief fail()方法的别名.
	 */
 	bool operator! () const;

	/*! \brief 获取套接字状态
	    @return 返回套接字的控制状态
	 */
	iostate rdstate () const { return m_state; }

	/*! \brief 重置套接字状态.关闭处于失败状态的套接字
	 */
	void clear (iostate state_ = Socket::goodbit);

	/*! \brief 设置套接字状态
	 *  @param flag_ 新的状态值
	 */
	void setstate (iostate flag_);

	/*! \brief 检测套接字上有没有失败操作.
	    @return 如果没有失败操作，返回true, 否则返回false.
	 */
	bool good () const { return m_state == 0; }

	/*! \brief 检测是否到达输入流的EOF(对方关闭了套接字)
	    @return 如果对方关闭了套接字，返回true.否则返回false.
	 */
	bool eof ()  const { return m_state & Socket::eofbit; }

	/*! \brief 检测套接字操作是否失败.
	    @return 如果failbit或者badbit被设置, 返回true. 否则返回false.
	 */
	bool fail () const 
		{ 
			return m_state & (Socket::failbit | Socket::badbit);
		}

	/*! \brief 检测套接字fd==-1或读写错误
	    @return 如果badbit被设置，返回true, 否则返回false.
	 */
	bool bad ()  const { return m_state & Socket::badbit; }

	/*! \brief 把套接字状态写入日志文件
	 */
	void dumpState () const;

	/*! \brief XDR编码的内置char类型值输入
	 */
	Socket& operator>> (char& c);

	/*! \brief XDR编码的内置u_char类型值输入
	 */
	Socket& operator>> (unsigned char& c_) 
		{ 
			return operator>>((char&) c_); 
		}

	/*! \brief XDR编码的内置signed char类型值输入
	 */
	Socket& operator>> (signed char& c_) 
		{ 
			return operator>>((char&) c_); 
		}

	/*! \brief XDR编码的内置short类型值输入
	 */
	Socket& operator>> (short& n_);

	/*! \brief XDR编码的内置u_short类型值输入
	 */
	Socket& operator>> (unsigned short& n_);

	/*! \brief XDR编码的内置int类型值输入
	 */
	Socket& operator>> (int& n_);

	/*! \brief XDR编码的内置u_int类型值输入
	 */
	Socket& operator>> (unsigned int& n_);

	/*! \brief XDR编码的内置long类型值输入
	 */
	Socket& operator>> (long& n_);

	/*! \brief XDR编码的内置u_long类型值输入
	 */
	Socket& operator>> (unsigned long& n_);

	/*! \brief XDR编码的内置float类型值输入
	 */
	Socket& operator>> (float& n_);

	/*! \brief XDR编码的内置double类型值输入
	 */
	Socket& operator>> (double& n_);

	/*! \brief XDR编码的内置char类型输出
	 */
	Socket& operator<< (char c);

	/*! \brief XDR编码的内置u_char类型输出
	 */
	Socket& operator<< (unsigned char c_) 
		{ 
			return (*this) << (char) c_; 
		}

	/*! \brief XDR编码的内置signed char类型输出
	 */
	Socket& operator<< (signed char c_) 
		{ 
			return (*this) << (char) c_; 
		}

	/*! \brief XDR编码的内置short类型输出
	 */
	Socket& operator<< (short n_);

	/*! \brief XDR编码的内置u_short类型输出
	 */
	Socket& operator<< (unsigned short n_);

	/*! \brief XDR编码的内置int类型输出
	 */
	Socket& operator<< (int n_);

	/*! \brief XDR编码的内置unsigned int类型输出
	 */
	Socket& operator<< (unsigned int n_);

	/*! \brief XDR编码的内置long类型输出
	 */
	Socket& operator<< (long n_);

	/*! \brief XDR编码的内置u_long类型输出
	 */
	Socket& operator<< (unsigned long n_);

	/*! \brief XDR编码的内置float类型值输出
	 */
	Socket& operator<< (float n_);

	/*! \brief XDR编码的内置double类型值输出
	 */
	Socket& operator<< (double n_);

	/*! \brief  操控符
	 */
	Socket& operator<< (Socket& (*f) (Socket&))
		{
			return (f (*this));
		}

protected:
	/*! \brief 设定套接字选项.
	    @return 0 on success, -1 on error (setsockopt(2) failed)
	*/
	int set_option (int level_, int optname_, int val_);

	/*! \brief 设定文件描述字选项
	    @return 0 on success, -1 on error (fcntl(2) failed)
	*/
	int set_fd_options (int flags_);

	/*! \brief 清除文件描述字选项.
	    @return 0 on success, -1 on error (fcntl(2) failed)
	*/
	int clear_fd_options (int flags_);

protected:
	int m_fd; /*!< 文件描述字 */
	int m_type; /*!< socket类型, AF_INET或者AF_UNIX */
	IOState m_state; /*!< socket控制状态 */ 


private:
	/*! \brief 拷贝构造函数
	 * 这个类的拷贝构造函数语义难以定义，所以放置在private区，保护拷贝操作.
	 */
	Socket (const Socket&);

	/*! \brief 拷贝构造函数
	 * 这个类的赋值构造函数语义难以定义，所以放置在private区，保护赋值操作.
	 */
	Socket& operator= (const Socket&);
};

END_SUNRISE_NAMESPACE

#include "Streambuf.h"

#endif // SOCKET_H



