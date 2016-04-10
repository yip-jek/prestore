#ifndef CONUDP_SOCKET_H
#define CONUDP_SOCKET_H

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif

#include "UDPSocket.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file ConUDPSocket.h
 * \brief ��װ�������ӵ�UDP�׽ӿ�
*/

/*! \class ConUDPSocket
 * \brief �������ӵ�UDP�׽ӿ�
 */
class ConUDPSocket : public UDPSocket {
public:
	/*! \brief ���캯��
	 */
	ConUDPSocket () : UDPSocket() {
		char self[] = "ConUDPSocket::ConUDPSocket"; trace(self);
	}
    
	/*! \brief ��������
	 */
	virtual ~ConUDPSocket () {
		char self[] = "ConUDPSocket::~ConUDPSocket"; trace(self);
	} 

	/*! \brief ���ӶԷ���ַ
	 *  @param peer_addr_ �Է���ַ
	 */
	bool connect (const Address& peer_addr_);

	/*! \brief �Ͽ�����
	 */
	void unconnect ();

	/*! \brief ���׽��ֶ�ȡָ���ֽ���������
	 *  ע��:������������ڻ����ϵ��ã���ΪIPv4��read()��UDP��read()��Ȼ
	 *  ��ͬ.
	 *
	 *  @param buf_ ��ȡ���ݵĻ�����
	 *  @param size_ ������ȡ���ֽ���
	 *  @return ���ض�ȡ���ֽ�����������ִ��󣬷���-1, ������errno������0���ֽ��ǿ��ܵ�.
	 */
	int read (char* buf_, const unsigned int size_);
	
	/*! \brief ����ʽ���׽��ֶ���ָ���ֽ���������
	 *
	 *  @param buf_ д������
	 *  @param size_ ����д����ֽ���.
	 */
	int write (const char* buf_ = NULL, const unsigned int size_ = 0);
};

END_SUNRISE_NAMESPACE

#endif // CONUDP_SOCKET_H



