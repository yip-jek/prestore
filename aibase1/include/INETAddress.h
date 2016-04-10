#ifndef INET_ADDRESS_H
#define INET_ADDRESS_H

#include <vector>
using std::vector;

#include "Prelude.h"
#include "Address.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file INETAddress.h 

	An incapsulation of TCP/UDP Internet Protocol socket address structure.
*/

/*! \class INETAddress
 * \brief inet��ַ��
 * ��װinet��ַ
 */
class INETAddress : public Address {
public:
	/*! \enum Protocol
	 * \brief Э������ö��
	 */
	enum Protocol { 
		TCP,					/*!< TCP */
		UDP					/*!< UDP */
	};

public:
	/*! \brief ȱʡ���캯��
	 */
	INETAddress ();

	/*! \brief ���캯��
	 * ��in_addr��ַ�ṹ��Ͷ˿ںŹ�������, �ͻ���ʹ��.
	 *
	 *  @param haddr_ ��������ַ
	 *  @param port_ �����������Ķ˿�
	 */
	INETAddress (struct in_addr * haddr_, int port_);

	/*! \brief ���캯��
	 * ���������Ͷ˿ںŹ�������, �ͻ���ʹ��.
	 *
	 *  @param host_ ������������
	 *  @param port_ �����������Ķ˿�
	 */
	INETAddress(const char* host_, int port_);

	/*! \brief ���캯��
	 * ��������, ��������ָ����Э�����͹�������
	 * ���з�����������/etc/services�ļ��ж���
	 *
	 *  @param host_ ������������
	 *  @param service_ ������
	 *  @param protocol_ Э������: TCP (ȱʡ) ���� UDP
	 */
	INETAddress (const char* host_, const char* service_, 
				 Protocol protocol_ = TCP);

	/*! \brief ���캯��
	 * �ö˿ںŹ�������
	 *
	 *  @param port_ �������˿�
	 */
	INETAddress (int port_);

	/*! \brief ���캯��
	 * �ͻ��˺ͷ�������INET��ַ�๹�캯��
		��ʽ:
	       - [host:]service 
	       - service[@host]

	    ���û��дhost, ����INADDR_ANY��ַ.
		 service����Ҫ��/etc/services�ļ�������Ӧ����.

	    @param address_ ��ַ��
	    @param protocol_ Э������: TCP (ȱʡ) �� UDP.
	 */
	INETAddress (const char* address_, Protocol protocol_ = TCP);

	/*! \brief �������캯��
	 */
	INETAddress (SA_IN* address_);

	/*! \brief �������캯��
	 */
	INETAddress (SA* address_);

	/*! \brief ��������
	 */
	~INETAddress () { 
		trace_with_mask("INETAddress::~INETAddress",SOCKTRACE);
	}

	/*! \brief ��ȡ��ַʵ���ĳߴ�
	 */
	const int getLength () const { return sizeof (m_address); }

	/*! \brief ��ȡ�����ַʵ��ָ��
	 */
	SA* getAddress () const { return (SA*) &m_address; }

	/*! \brief ��ȡ������
	 */
	string getHostName ();

	/*! \brief ��ȡ�˿ں�
	 */
	int getPort () const { return ntohs (m_address.sin_port); }

	/*! \brief �ѵ�ַ״̬д����־�ļ�
	 */
	void dump ();

	/*! \brief ��ȡ����������, �������������������vector���淵�ء�

		@param aliases_ ���������б�
		@return ��������������.
	*/
	static string 
	get_fully_qualified_domain_name (vector<string>& aliases_);

private:
	/*! \brief  ʹ������ת��Ϊ�����ָ�ʽ���˿ں�ת��Ϊ����������ֽ���.

	    ���ת��ʧ�ܣ��趨����״̬Ϊbad. ͬʱ���趨errno.
	 */
	void createHostPort (const char* host_, int port_);

	/*! \brief ��/etc/services�ļ����ҳ��˿ںţ���ת��Ϊ���������
	 * �ֽ���.
	 *
	   @param serv_ ������
	   @param prot_ Э������: tcp (ȱʡ) �� udp.
	   @return ���ض�����������ֽ���˿ں�, ���ʧ���򷵻�0.
	 */
	int getServiceByName (string serv_, Protocol prot_ = TCP);

	/*! \brief ������ʼ������, �����캯����Ҫ����
	 */
	void init ();

private:
	/*! \brief ���������Ļ���.
	 */
	static string m_fqdn_cache;

private:
	/*! \brief �����ַ�ṹ��sockaddr_in
	 */
	SA_IN m_address;
};

END_SUNRISE_NAMESPACE

#endif /* INET_ADDRESS_H */
