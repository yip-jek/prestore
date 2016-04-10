//------------------------------------------------------------------------------
//                         Streambuf.h
//------------------------------------------------------------------------------
#ifndef STREAM_BUF_H
#define STREAM_BUF_H

#include <stdio.h>		// EOF
#include "Prelude.h"
#include "Assert.h"		// trace() & Assert family

BEGIN_SUNRISE_NAMESPACE

/*! \file Streambuf.h 
 * 基于Standard C++ iostream streambuf的流缓冲类，实现套接口类底层的缓冲特性.
 */

/** 
 * io_ptrs structure.
 */
class io_ptrs
{
public:
	char* m_read_base;
	char* m_read_ptr;
	char* m_read_end;

	char* m_write_base;
	char* m_write_ptr;
	char* m_write_end;

	char* m_buf_base;
	char* m_buf_end;

	enum { USER_BUF = 1, UNBUFFERED = 2, EOF_SEEN = 4, ERR_SEEN = 8 };

	int   m_flags;
	char  m_shortbuf[1];

	io_ptrs () : m_read_base (0), m_read_ptr (0), m_read_end (0),
		     m_write_base (0), m_write_ptr (0), m_write_end (0),
		     m_buf_base (0), m_buf_end (0), m_flags (0)
		{ 
			m_shortbuf [0] = 0; 
		}

	void dump () const;
};

/*! \class Streambuf 
 *
 * \brief 缓冲流对象。
 *
 * Streambuf类基于Standard C++ iostream streambuf.实现套接口类
 * 底层的缓冲机制.
 *
 * Streambuf类内部容纳三个区域: get区域,put区域和保留区域.
 *
 * get区域保存立即可用的输入缓冲数据.
 * put区域保存还没有刷新到输出的缓冲数据.
 * 保留区域与get区域部分重叠. get区域和put区域可以扩展到
 * 保留区域的剩余部分. 在输入和输出的操作过程中，get区域和
 * put区域将扩展或者缩小, 但总在总缓存大小范围内.
 */
class Streambuf : public io_ptrs
{
public:
	static const int MAXTCPFRAMESZ; /*!< TCP帧的最大尺寸(TCP选项) */

	/*! \brief 析构函数
	 */
	virtual ~Streambuf ();

	/*! \brief 设置缓冲区
	 *  @return 返回缓冲流对象指针.
	 */
	Streambuf* pubsetbuf (char* s_, int n_);

	/*! \brief 刷新数据到输出.
	 * @see sync
	 * @return 返回刷新数据的字节数.
	 */
	int        pubsync ();

	/*! \brief 在Socet带有缓冲I/O的情况下，这个函数并不真正调用socket api
	 * ，而是直接返回缓冲区中可用的字节数, 确保读取该字节不会出错并保证
	 * 不会访问底层套接字.
	 */
	int        in_avail ();

	/*! \brief 移动get区域指针到下一个位置，返回下一次读操作的位置.
	    如果再调用这个函数之前或者之后get区域指针已经到达EOF, 则返回EOF.
	*/
	int        snextc ();

	/*! \brief 移动get区域指针到下一个字符的位置，并返回之前的字符
	 * 如果当前的get区域指针已经在EOF, 返回EOF.
	 */
	int        sbumpc ();

	/*! \brief 返回当前get区域指针的下一个字符. 或者如果get区域
	 * 指针已经到达EOF，返回EOF.(get区域指针不会被移动)
	 */
	int        sgetc ();

	/*! \brief 取出下len_个字符,拷贝到b_里面. get区域指针移动len_个
	 * 字节.如果可取的字节少与len_，则取出所有可取的，返回实际取出
	 * 的字符数.
	 * @return 返回实际取出的字符数.
	*/
	int        sgetn (char* b_, int len_);

	/*! \brief 储存c到put区域指针的下一个位置,put区域指针移动一个
	 * 字节.扩展put区域边界.  返回c，如果失败，返回EOF. 如何构成
	 * 错误决定于衍生类.
	 */
	int        sputc (char c_);

	/*! \brief 储存len_个字符到put区域指针之后. 移动put区域指针len_
	 * 个字节.如果可存储的空间少于len_，则存储可用长度的数据。
	 *
	 * @return 返回实际存储的字节数.
	 */
	int        sputn (char* b_, int len_);


	/*! \brief 如果i_非零，所有的IO操作都要有缓冲.否则不IO操作都没有缓冲.
	*/
	void       unbuffered (int i_);
	
	/*! \brief  检测是否无缓冲.
	 * @return 如果是无缓冲，返回true, 否则返回false.
	 */
	int        unbuffered ();

protected:
	/*! \brief 缺省构造函数.
	 * 放在protected，确保衍生类才能构造.
	 */
	Streambuf ();

	Streambuf (const Streambuf&);
	Streambuf& operator= (const Streambuf&);

	/*! \brief 返回get区域指针的底值.
	*/
	char*      base () const;

	/*! \brief 返回get区域指针的值.
	 * 下一次的粹取操作就能通过egptr()-1-gptr()而得知能取多少数据。
	 * 如果egptr()<=gptr()，表示没有数据可取.
	 */
	char*      gptr ()  const;

	/*! \brief 返回get区域指针的底值
	*/
	char*      egptr () const;

	/*! \brief 设置get区域指针.
	 */
	void       setg (char* gbeg_, char* gnext_, char* gend_);

	/*! \brief 返回put区域指针的底值
	 * put区域指针值可通过pptr()-1方法得到。pptr()-1-pbase()可以知道
	 * put区域存储了多少数据。
	*/
	char*      pbase () const;

	/*! \brief 返回put区域指针的值.
	 */
	char*      pptr ()  const;

	/*! \brief 返回put区域指针底值
	 */
	char*      epptr () const;

	/*! \brief 设置put区域指针
	*/
	void       setp (char* pbeg_, char* pend_);

	/*! \brief put区域指针移动n_位.
	 */
	void       pbump (int n_);

	/*! \brief 建立保留缓冲. 
	 * 区域指针基值(base())设置为b_, 底值(ebase())设置为eb_, 
	 * 如果del_非零，则无论何时基值改变都会把缓冲删除.否则,
	 * 缓冲不会自动被删除.
	 *
	 *   @param b_ 缓冲第一个字节的地址
	 *   @param eb_ 缓冲最后一个字节的地址
	 *   @param del_ 0 - 外部的内存管理，流缓冲对象不会自己管理内存.
	 *   				1 - 内部自动的内存管理，在交换或者析构的时候删除缓冲.
	*/
	void       setb (char* b_, char* eb_, int del_);

	void       init ();

protected:
	/*! \brief 设置缓冲指针和缓冲区长度.
	 *
	    @param p_ 缓冲区指针
	    @param len_ 缓冲区长度
	*/
	virtual Streambuf* setbuf (char* p_, int len_);

	/*! \brief 流缓冲同步
	 * 衍生类把put区域的数据刷新到目的地(对于socket缓冲，就是发送到对方
	 * 套接口), 同时在输入缓冲获取数据。
	 *
	 * @return 如果出现错误返回EOF, 成功则返回0. 对于这个基类，
	 * 缺省的行为是: 如果没有悬而为决的数据在输入缓冲或者输出缓冲,
	 * 返回0, 否则返回EOF.
	 */
	virtual int        sync ();
	
	/*  \brief 返回缓冲中可用的数据量评估值. 
		 
		 @return 如果返回大于0的值，那么调用underflow()方法将不会
		 返回EOF, 如果返回-1, 调用underflow()或者uflow()将失败.
	*/
 	virtual int        showmanyc ();

	/*! \brief 从输入序列分配len_个字节的数据到b_.
	    @return 分配的字节数.
	*/
	virtual int        xsgetn (char* b_, int len_);

	/*! \brief 当get区域为空时提供给输入区域.
	 */
	virtual int        underflow ();

	/*! \brief 从输入序列读入数据, 移动流指针位置如下：

	    1) 如果输入序列有一个读位置值，则返回(unsigned char)*gnext++值标志成功.

	    2) 否则, 如果能从输入序列直接读入, 则返回(unsigned char) x值标志成功. 

	*/
	virtual int        uflow ();

	/*! \brief  向输出缓冲写len_个字节的数据(通过循环调用sputc(c)).
	 *
	 * @param b_ 输出数据指针
	 * @param len_ 数据长度
	 * @return 返回实际写入的字节数.
	 */
	virtual int        xsputn (const char* b_, int len_);

	/*! \brief 在put区域满并且要存储更多的数据时消耗数据(把它们刷新到输出),
	 * 这个基类的缺省行为没有定义。所以衍生类必需定义自己的overflow方法。
	 * 通常的动作是消耗put区域(pbase()和pptr()之间)的数据,  同时调用setp()
	 * 方法建立一个新的put区域，然后如果c不是EOF的话存储c(使用sputc()方法).
	 *
	 * @return 发生错误时返回EOF，否则返回其他值.
	 */
	virtual int        overflow (int c = EOF);

	/*! \brief 允许在unbuffered()和base()返回为0时，建立一个适当尺寸
	 * 的保留区域。
	 *
	 * @return 如果成功建立保留区，返回大于0的值，否则返回EOF.
	 * 缺省的策略是用new创建缓冲区.
	 */
	virtual int        doallocate ();
};


END_SUNRISE_NAMESPACE

#endif /* STREAM_BUF_H */  
