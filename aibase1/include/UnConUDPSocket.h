//------------------------------------------------------------------------------
//                        UnConUDPSocket.h
//------------------------------------------------------------------------------
#ifndef UNCONUPD_SOCKET_H
#define UNCONUPD_SOCKET_H

#include "Prelude.h"
#include "UDPSocket.h"

BEGIN_SUNRISE_NAMESPACE

/** \file UnConUDPSocket.h 
 *  \brief ��װ��������ӵ�UDP�׽ӿ�
 */

/*! \class UnConUDPSocket
 * \brief ��������ӵ�UDP�׽ӿ�
 */
class UnConUDPSocket : public UDPSocket {
public:
	/*! \brief ���캯��
	 */
	UnConUDPSocket();
    
	/*! \brief ��������
	 */
	~UnConUDPSocket();

	/*! \brief recvfrom()�����ڷ�����ģʽ�׽ӿ��Ͻ�����Ϣ. 
	 *
	 *  @param buf_ �洢������Ϣ�Ļ���
	 *  @param size_ ����������
	 *  @param peer_addr_ �Է���ַ
	 *  @return ����ʵ��ȡ�õ��ֽ���
	 */
	int recvfrom (char* buf_, int size_, Address* peer_addr_);
	
	/*! \brief sendto()�����ڷ�����ģʽ�׽ӿ��Ϸ�����Ϣ
	 *
	 *  @param buf_ ������Ϣ�Ļ�����
	 *  @param size_ �������ĳ���
	 *  @param dest_addr_ Ŀ�ĵ�ַ
	 *  @return ���ͳɹ����򷵻�ʵ�ʷ��͵��ֽ���, ���򷵻�-1.
	 */
	int sendto (const char* buf_, 
				const unsigned int size_, 
				const Address* dest_addr_);

	/*! \brief ��Socet���л���I/O��������������������������socket api
	 * ������ֱ�ӷ��ػ�����(Socketbuf)�п��õ��ֽ���, ȷ����ȡ���ֽڲ��������֤
	 * ������ʵײ��׽���. ��ΪUnConUDPSocket��������ӵģ�û�л�����, ����������
	 * ʵ�������Ƿ���0.
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



