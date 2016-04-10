//------------------------------------------------------------------------------
//                              Socketbuf.h
//------------------------------------------------------------------------------
#ifndef SOCKET_BUF_H
#define SOCKET_BUF_H

#include "Prelude.h"
#include "Streambuf.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file Socketbuf.h 
 *
 * 继承Streambuf，提供socket文件描述字的I/O缓冲特性.
 */

class Socket;

/*! \class Socketbuf
 * \brief 套接口缓冲类
 * 为socket提供缓冲机制.
 */
class Socketbuf : public Streambuf
{
public:
	Socketbuf (Socket* s_);
	virtual ~Socketbuf ();

protected:
	virtual int  sync ();
	virtual int  underflow ();
	virtual int  overflow (int c_ = EOF);
	virtual int  showmanyc ();
	virtual int  doallocate ();

private:
	int  flush_output ();
	int  sys_read  (char* b_, int len_);
	int  sys_write (char* b_, int len_);
	void xput_char (char c_);

private:
	/** Reference to the Socket stream.
	 */
	Socket* m_s;
};

END_SUNRISE_NAMESPACE

#endif /* SOCKET_BUF_H */  
