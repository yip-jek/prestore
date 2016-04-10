//------------------------------------------------------------------------------
//                             IPv4Socket.h
//------------------------------------------------------------------------------
#ifndef IPV4_SOCKET_Hh
#define IPV4_SOCKET_Hh

#include "Prelude.h"
#include "Socket.h"
#include "Socketbuf.h"

/*! \file IPv4Socket.h
	\brief IPv4Socket��ķ�װ������AF_INET��AF_UNIX�׽ӿ�, �ײ�Ļ���ʹ��
	Socketbuf��ʵ��.
*/

BEGIN_SUNRISE_NAMESPACE

/*! \class IPv4Socket
 * \brief IPv4Socket�׽ӿ���
 */
class IPv4Socket : public Socket 
{
public:
	static const int MAXTCPBUFSZ; /*!< TCP����֡�����ֵ */

	/// Default constructor
	IPv4Socket() 
		: m_path (0), m_rdbuf (new Socketbuf (this)) {
		trace_with_mask("IPv4Socket::IPv4Socket()",SOCKTRACE);
	}

	/*! \brief ���캯��
	 *  @param fd_ �ļ�������
	 */
	IPv4Socket(const int fd_) 
		: m_path (0), m_rdbuf (new Socketbuf (this)) {
		trace_with_mask("IPv4Socket::IPv4Socket(fd_)",SOCKTRACE);

		m_fd = fd_;
	}

	/*! \brief ��������
	 * �ر��׽ӿ�
	 */
	~IPv4Socket() {
		trace_with_mask("IPv4Socket::~IPv4Socket",SOCKTRACE);
		this->close ();

 		if (m_rdbuf != 0) {
			delete m_rdbuf;
		}
	}

	/*! \brief ��¡����
	 * ����ϵͳ����dup(2)�����ײ������ֱ������������������ڲ�״̬, ����
	 * ����µĶ��� .
	 */
	IPv4Socket* clone () const;
	

	/*!  \brief ����socket.
	 * socket��������AF_INET(��ʾinternet�׽ӿ�)����AF_UNIX(��ʾUNIX���׽ӿ�)
	 *
	 *  @param domain_  ������(AF_INET����AF_UNIX)
	 *  @return ��������ɹ�����true, ���򷵻�false.
	 */
	bool open(const int domain_);

	/*! \brief �ر��׽ӿ�
	 *  @return �ɹ�����true, ���򷵻�false.
	 */
	bool close();

	/*! \brief ��������
	 * ����׽ӿ�����Ϊ��������ʽ, connect()�����ĵ��úܿ��ܷ���false, 
	 * ����errno���趨ΪEINPROGRESS.
	 * ��ο�connect(2)��man pages.
	 *
	 *  @param address_ Ҫ���ӵĵ�ַ
	 *  @return ���ӳɹ�����true, ���򷵻�false.
	 */
	bool connect(const Address& address_);

	/*! \brief �󶨶˿�
	 * ���������open()��accept()֮�����
	 *
	 *  @param my_address_ Ҫ�󶨵ĵ�ַ
	 *  @return ����ɹ�����true, ���򷵻�false.
	 */
	virtual bool bind (const Address& my_address_);

	/*! \brief �ڼ����Ķ˿��Ͻ���һ������
	 *  ���ص���һ��'��ȫ'������(��ʾ�Է��׽ӿ��Ѿ�׼�������ݴ��䣬����Ҫ����
	 *  ::open()��). ��������������������ϣ�ֱ��������������.
	 *  ���Ҫ����������Ҫ�ȵ���::select().
	 *
	 *  @return ����ɹ��������µ�IPv4�׽ӿڶ���ָ��.���򷵻�0.
	 */
	IPv4Socket* accept ();

	/*! \brief ����ָ���ֽ���������, ����ڻ�����
	 *
	 *  @param buf_ �������ݵĻ�����
	 *  @param size_ �������յ��ֽ���
	 *
	 *  @return ����ʵ�ʽ��յ����ֽ���, ������ִ��󷵻�-1, ���趨errno.
	 *  			����Է��ر������ӣ��򷵻�0.
	 */
	int read (char* buf_, const unsigned int size_);

	/*! \brief ������ʽд��ָ���ֽ���������.
	 *
	 *  @param buf_ ���ͻ�����
	 *  @param size_ ָ�����ֽ���
	 *  @return ����ʵ��д����ֽ��������ʧ�ܷ���-1.
	 */
	int write (const char* buf_, const unsigned int size_);


	/*! \brief ��ȡ�׽���
	 */
	const int getHandler () const { return m_fd; }

	/*! \brief ��ȡ�׽ӿ�������
	 */
	const int getDomain () const { return m_type; }

	/*! \brief ���ع������׽��ֵ�<B> Socketbuf </B>ָ��
	 * �������������Ҫ��Socket������ȡ��<B>Socketbuf</B>ֱ�Ӳ������龰.
	 *
	 * @return ���ع����ڸö����Streambuf(Socketbufת��ΪStreambuf)ָ��.
	 */
	virtual Streambuf* rdbuf () { return m_rdbuf; }

	/*! \brief �����µ��׽�����, ����ԭ�е��׽�����.ȱʡ�Ƿ���NULL. 
	 *
	 *  @return ԭ���׽�����(Socketbuf).
	*/
	virtual Streambuf* rdbuf (Streambuf* sb_);

	/*! \brief ��Socet���л���I/O��������������������������socket api
	 * ������ֱ�ӷ��ػ������п��õ��ֽ���, ȷ����ȡ���ֽڲ��������֤
	 * ������ʵײ��׽���.
	 */
	virtual int in_avail () const { return m_rdbuf->in_avail (); }

private:
	IPv4Socket (const IPv4Socket&); /*!< ���ο������캯�� */
	IPv4Socket& operator= (const IPv4Socket&); /*!< ���θ�ֵ������ */

private:
	char*       m_path; /*!< UNIX���׽ӿ� */
	Streambuf*  m_rdbuf; /*!< �ײ�Ļ���ʵ�� */
};

END_SUNRISE_NAMESPACE

#endif // IPV4_SOCKET_Hh



