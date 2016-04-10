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
 * \brief echo������
 */

BEGIN_SUNRISE_NAMESPACE

class Streambuf;		// Forward declaration

/*! \file Socket.h 
 * �׽��ַ�װ��
 */

/*! \class Socket
 * \brief �׽��ַ�װ��
 */
class Socket {
public:
	static const int PGSIZE; /*!< �ں�ҳ�Ĵ�С */

	/*! \enum io_state_t 
	 *  \brief io״̬λö��: goodbit, eofbit, failbit, badbit.
	 */
	enum io_state_t { 
		goodbit = 0,  /*!< ��־socket׼����ʹ�� */
		eofbit  = 1,  /*!< ��־�������������������ĩ�� */
		failbit = 2,  /*!< ��־������������������ַ�ʧ�ܣ����������������Ҫ����ַ�ʧ�� */
		badbit  = 4   /*!< ��־�������������е�����ʧȥ������(����һ�����ɻָ����ļ�������). */
	};

	typedef int iostate;
	typedef unsigned char IOState;

	/*! \enum opt_t
	 *  \brief �׽���ѡ��
	 */
	enum opt_t { 
		reuseaddr,   /*!< �����ص�ַ����. */
		rcvlowat,    /*!< ��ʹselect(3)���ؿɶ�("readable"), �׽��ֽ��ջ��屻Ҫ������ˮλ��.
							TCP��UDP�׽���ȱʡ��1(ע: POSIX.1g��û��Ҫ�����ѡ��) */
		sndlowat,    /*!< ��ʹselect���ؿ�д("writable"), �׽��ַ��ͻ��屻Ҫ������ˮλ��.
							����TCP,ȱʡֵ��2048, ����UDP�����Ǵ���writable״̬. (ע: POSIX.1g
							��û��Ҫ�����ѡ��) */
		blocking,    /*!< ����io����, ������ȱʡ��. ������������ڸ��ԵĻ�����û������ʱ����
							����˯��״̬��֪�������ݵ�� */
		nonblocking  /**< �����׽���ʹ�÷�����ģʽ.
						����������������������û������(����TCP��������1���ֽڵ�����.����UDP,����
						һ�����������ݱ�), �������ᱻ�����������������أ�ͬʱerror����ΪEWOULDBLOCK.
	
						�����������, ���������û������, ���Ҳ�������������������������أ�����
						error����ΪEWOULDBLOCK */
	};

	/*! \brief ���캯��
	 */
	Socket();

	/*! \brief ��������
	 */
	virtual ~Socket();

	/*! \brief ��һ���׽���
	 */
	virtual bool open(const int domain_) =0;

	/*! \brief �ر��׽���
	 */
	virtual bool close() =0;

	/*! \brief ����
	    @param address_ ���ӵĵ�ַ
	 */
	virtual bool connect (const Address& address_);

	/*! \brief ��
	    @param my_address_ Ҫ�󶨵ĵ�ַ
	    @return �󶨳ɹ�����true. ���򷵻�false.
	 */
	virtual bool bind (const Address& my_address_) =0;

	/*! \brief дָ���ֽ��������ݵ��׽���
	    @param buf_  Ҫ���͵����ݰ�ָ��
	    @param size_ Ҫ���͵��ֽ���
	 */
	virtual int write (const char* buf_, const unsigned int size_); 

	/*! \brief ��ȡ�ڽ��ջ����ڿ��õ������ֽ���.
	 */
	int getBytesAvail (void) const;

	/*! \brief ���׽����϶���������Ŀ������.
	    @param buf_ ����������ݵĻ���
	    @param size_ �������յ��ֽ���
	 */
	virtual int read (char* buf_, const unsigned int size_);

	/*! \brief ��ȡָ����Ŀ�����ݲ�����֮.���û�в��������������ֱ��EOF.
	 * ��ȡ��ʽ����: <P>

	    <table border width=75% cellpadding=3>
	    <td><b> n_</b></td> <td><b> delim_ </b></td>
	    <td><b> Action </b></td>
	    <tr>
	    <td aling=center> 1 </td><td> EOF </td>
	    <td> ���벢����1���ֽ� </td> 
	    </tr>
	    <tr>
	    <td align=center> k </td><td> EOF </td>
	    <td> ���벢�������1���ֽ� </td></tr>
	    <tr>
	    <td align=center> INT_MAX </td><td> EOF </td>
	    <td> ���벢����ֱ��EOF</td></tr>
	    <tr>
	    <td align=center> INT_MAX </td><td> 'c' </td>
	    <td> ���벢����ֱ��EOF��������'c' </td>
	    </tr>
	    <tr>
	    <td align=center>    k    </td><td> 'c' </td>
	    <td> ���벢�������k���ֽڣ��������'c'��ֹͣ. </td>
	    </tr>
	    </table><p>

	   @param n_ Ҫ�������ֽ���(ȱʡΪINT_MAX)
	   @param delim_ ��ֹ�� (ȱʡΪEOF)
	   @return ���ض������ֽ���
	 */
	int ignore (int n_ = INT_MAX, int delim_ = EOF);

	/*! \brief ��ȡ�ļ�������
	 */
	virtual const int getHandler() const = 0;

	/*! \brief ��ȡ�׽�������(AF_INET����AF_UNIX)
	 */
	virtual const int getDomain() const = 0;

	/*! \brief ���ع������׽��ֵ�<B> Streambuf </B>ָ��
	 * �������������Ҫ��Socket������ȡ��<B>Streambuf</B>ֱ�Ӳ������龰.
	 * ȱʡ�Ƿ���NULL
	 *
	 * @return NULL
	 */
	virtual Streambuf* rdbuf () { return 0; }

	/*! \brief �����µ��׽�����, ����ԭ�е��׽�����.ȱʡ�Ƿ���NULL. 
	    @return ԭ���׽�����(Socketbuf).
	*/
	virtual Streambuf* rdbuf (Streambuf* /*sb_*/) { return 0; }

	/*! \brief ��Socet���л���I/O������£��������������������socket api
	 * ������ֱ�ӷ��ػ������п��õ��ֽ���, ȷ����ȡ���ֽڲ��������֤
	 * ������ʵײ��׽���.
	 */
	virtual int in_avail () const = 0;

	/*! \brief ����synchronizing����(<B>rdbuf->pubsync()</B>). �������
	 * ���׽������������ʣ���������ݳ�ˢ��.
	 */
	virtual Socket& flush ();

	/*! \brief �׽���ѡ��ʹ��.
	    @param opt_ ѡ����
	    @return �ɹ�����true,���򷵻�false.
	*/
	bool turnOptionOn (opt_t opt_);

	/*! \brief �ر�ĳ�׽���ѡ��
	    @param opt_ ѡ����
	    @return �ɹ�����true,���򷵻�false.
	*/
	bool turnOptionOff (opt_t opt_);

	/*! \brief �趨�׽���ĳѡ��ֵ

	    @param opt_ ѡ����
	    @param arg_ Ҫ�趨��ֵ (���ڲ���ֵ: 0 - �ر�, 1 - ʹ��).
	    @return true on success_; false if error
	*/
	bool setOption (opt_t opt_, int arg_);

	/*! \brief ��ȡ�׽���ѡ�ǰֵ
	    @param opt_ ѡ����
	    @return ����ɹ�,����ѡ��ֵ(���ڲ���ֵ: 0 - �ر�, 1 - ʹ��); ���򷵻�-1.
	*/
	int  getOption (opt_t opt_) const;

	operator void* () const;

	/*! \brief fail()�����ı���.
	 */
 	bool operator! () const;

	/*! \brief ��ȡ�׽���״̬
	    @return �����׽��ֵĿ���״̬
	 */
	iostate rdstate () const { return m_state; }

	/*! \brief �����׽���״̬.�رմ���ʧ��״̬���׽���
	 */
	void clear (iostate state_ = Socket::goodbit);

	/*! \brief �����׽���״̬
	 *  @param flag_ �µ�״ֵ̬
	 */
	void setstate (iostate flag_);

	/*! \brief ����׽�������û��ʧ�ܲ���.
	    @return ���û��ʧ�ܲ���������true, ���򷵻�false.
	 */
	bool good () const { return m_state == 0; }

	/*! \brief ����Ƿ񵽴���������EOF(�Է��ر����׽���)
	    @return ����Է��ر����׽��֣�����true.���򷵻�false.
	 */
	bool eof ()  const { return m_state & Socket::eofbit; }

	/*! \brief ����׽��ֲ����Ƿ�ʧ��.
	    @return ���failbit����badbit������, ����true. ���򷵻�false.
	 */
	bool fail () const 
		{ 
			return m_state & (Socket::failbit | Socket::badbit);
		}

	/*! \brief ����׽���fd==-1���д����
	    @return ���badbit�����ã�����true, ���򷵻�false.
	 */
	bool bad ()  const { return m_state & Socket::badbit; }

	/*! \brief ���׽���״̬д����־�ļ�
	 */
	void dumpState () const;

	/*! \brief XDR���������char����ֵ����
	 */
	Socket& operator>> (char& c);

	/*! \brief XDR���������u_char����ֵ����
	 */
	Socket& operator>> (unsigned char& c_) 
		{ 
			return operator>>((char&) c_); 
		}

	/*! \brief XDR���������signed char����ֵ����
	 */
	Socket& operator>> (signed char& c_) 
		{ 
			return operator>>((char&) c_); 
		}

	/*! \brief XDR���������short����ֵ����
	 */
	Socket& operator>> (short& n_);

	/*! \brief XDR���������u_short����ֵ����
	 */
	Socket& operator>> (unsigned short& n_);

	/*! \brief XDR���������int����ֵ����
	 */
	Socket& operator>> (int& n_);

	/*! \brief XDR���������u_int����ֵ����
	 */
	Socket& operator>> (unsigned int& n_);

	/*! \brief XDR���������long����ֵ����
	 */
	Socket& operator>> (long& n_);

	/*! \brief XDR���������u_long����ֵ����
	 */
	Socket& operator>> (unsigned long& n_);

	/*! \brief XDR���������float����ֵ����
	 */
	Socket& operator>> (float& n_);

	/*! \brief XDR���������double����ֵ����
	 */
	Socket& operator>> (double& n_);

	/*! \brief XDR���������char�������
	 */
	Socket& operator<< (char c);

	/*! \brief XDR���������u_char�������
	 */
	Socket& operator<< (unsigned char c_) 
		{ 
			return (*this) << (char) c_; 
		}

	/*! \brief XDR���������signed char�������
	 */
	Socket& operator<< (signed char c_) 
		{ 
			return (*this) << (char) c_; 
		}

	/*! \brief XDR���������short�������
	 */
	Socket& operator<< (short n_);

	/*! \brief XDR���������u_short�������
	 */
	Socket& operator<< (unsigned short n_);

	/*! \brief XDR���������int�������
	 */
	Socket& operator<< (int n_);

	/*! \brief XDR���������unsigned int�������
	 */
	Socket& operator<< (unsigned int n_);

	/*! \brief XDR���������long�������
	 */
	Socket& operator<< (long n_);

	/*! \brief XDR���������u_long�������
	 */
	Socket& operator<< (unsigned long n_);

	/*! \brief XDR���������float����ֵ���
	 */
	Socket& operator<< (float n_);

	/*! \brief XDR���������double����ֵ���
	 */
	Socket& operator<< (double n_);

	/*! \brief  �ٿط�
	 */
	Socket& operator<< (Socket& (*f) (Socket&))
		{
			return (f (*this));
		}

protected:
	/*! \brief �趨�׽���ѡ��.
	    @return 0 on success, -1 on error (setsockopt(2) failed)
	*/
	int set_option (int level_, int optname_, int val_);

	/*! \brief �趨�ļ�������ѡ��
	    @return 0 on success, -1 on error (fcntl(2) failed)
	*/
	int set_fd_options (int flags_);

	/*! \brief ����ļ�������ѡ��.
	    @return 0 on success, -1 on error (fcntl(2) failed)
	*/
	int clear_fd_options (int flags_);

protected:
	int m_fd; /*!< �ļ������� */
	int m_type; /*!< socket����, AF_INET����AF_UNIX */
	IOState m_state; /*!< socket����״̬ */ 


private:
	/*! \brief �������캯��
	 * �����Ŀ������캯���������Զ��壬���Է�����private����������������.
	 */
	Socket (const Socket&);

	/*! \brief �������캯��
	 * �����ĸ�ֵ���캯���������Զ��壬���Է�����private����������ֵ����.
	 */
	Socket& operator= (const Socket&);
};

END_SUNRISE_NAMESPACE

#include "Streambuf.h"

#endif // SOCKET_H



