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
 * \brief UDPSocket��UNIX domain�׽��ֻ�����.
 * �ض����͵�domain�׽���(ConUDPSocket��UnConUDPSocket)�����������.
 */

/*! \class UDPSocket
 * \brief UDPSocketʵ��UNIX domain�׽��ֻ����ӿ�
 */
class UDPSocket : public Socket {
public:
	/*! \brief ȱʡ���캯��
	 */
	UDPSocket () ;

	/*! \brief ���캯��
	 *  @param fd_ �ļ�������
	 */
	UDPSocket (const int fd_) ;

	/*! \brief ��������
	 * �ر�����
	 */
	virtual ~UDPSocket ();

	/*! \brief ����socket. internet socket��������AF_INET��ʾ��UNIX��socket
	 * ����AF_UNIX��ʾ
	 *
	 *  @param domain_ ��(AF_INET����AF_UNIX)
	 *  @return ��������ɹ�������true, ���򷵻�false.
	 */
	bool open (const int domain_);

	/*! \brief �ر�socket����
	 *  @return ����ɹ�����true, ���򷵻�false.
	 */
	bool close ();

	/*! \brief ��
	 *
	 *  @param my_address_ Ҫ�󶨵ĵ�ַ
	 */
	bool bind (const Address& my_address_);

	/*! \brief ��ȡsocket�ļ�������
	 */
	const int getHandler() const;

	/*! \brief ��ȡsocket������(AF_INET����AF_UNIX)
	 */
	const int getDomain() const;

protected:
	/*! \brief �����ļ�������
	 */
	void setHandler(const int fd_);

	/*! \brief ����socket������(AF_INET����AF_UNIX)
	 */
	void setDomain(const int type_);
};

END_SUNRISE_NAMESPACE

#endif // UDP_SOCKET_H


