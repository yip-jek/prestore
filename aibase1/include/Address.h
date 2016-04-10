//------------------------------------------------------------------------------
//                           Address.h
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifndef ADDRESS_H
#define ADDRESS_H

#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>		// addresses handling
#include <sys/un.h>
#include <string.h>
#include <errno.h>

#include "Prelude.h"
#include "Logger.h"
#include "Assert.h"

BEGIN_SUNRISE_NAMESPACE 

typedef struct sockaddr SA;	// stolen from R.Stevens
typedef struct sockaddr_in SA_IN;	
typedef struct sockaddr_un SA_UN;	

/*! \file Address.h 
 * \brief INET��UNIX-domain��ַ�������͵ĳ�����.
*/

/*! \class Address
 * \brief ��ַ��. �����˳���ĵ�ַ(����INET��UNIX-domain)������, ������ַ����.
 */
class Address {
public:
	/*! \enum addr_state_t
	 * \brief ��ַ״̬(�Ϸ����Ƿ�)ö��
	 */
	enum addr_state_t { 
		goodbit=0,				/*!< �Ϸ���ַ */
		badbit=1				/*!< �Ƿ���ַ */
	};
	typedef int addrstate;

private:
	unsigned char m_state;

public:
	/*! \brief ���캯��
	 */
	Address ();

	/*! \brief ��������
	 */
	virtual ~Address();

	/*! \brief ����ַ�ĺϷ���
	    @return �����ַ�Ϸ����򷵻�true. ���򷵻�false.
	 */
	bool good() const;

	/*! \brief ��ȡ�ڵ�ַ����������Ƿ�����˴���(����������/�˿ڲ���ʧ�ܻ��߸�ʽ����ȷ��)
	    @return �����ַ���󣬷���true, ���򷵻�true.
	 */
	bool bad() const;

	operator void* () const;

	/*! \brief bad()�����ķǺ���.
	    @return �����ַ�Ƿ�������true, ���򷵻�true.
	*/
	bool operator! () const;
	
	/*! \brief ��ȡ��ַʵ����size
	 */
	virtual const int getLength() const = 0;

	/*! \brief ��ȡ��ַʵ��ָ��
	 */
	virtual SA* getAddress() const = 0;
	
	/*! \brief �Ѷ���״̬д����־�ļ�
	 */
	virtual void dump () ;

protected:
	/*! \brief ����״̬
	 *  @param flag_ ״ֵ̬
	 */
	void setstate (addrstate flag_) { m_state |= flag_; }
};

END_SUNRISE_NAMESPACE

#endif /* ADDRESS_H */
