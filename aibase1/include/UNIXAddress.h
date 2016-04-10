//------------------------------------------------------------------------------
//                             UNIXAddress.h
//------------------------------------------------------------------------------
#ifndef UNIX_ADDRESS_H
#define UNIX_ADDRESS_H

#include "Prelude.h"
#include "Address.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file UNIXAddress.h 
 * UNIX domain�׽��ַ�װ
 */

/*! \class UNIXAddress
 * \brief UNIX domain�׽��ַ�װ
 */
class UNIXAddress : public Address {
public:
	/*! \brief ���캯��
	 * @param socket_name_ UNIX·��
	 */
	UNIXAddress (const char * socket_name_);

	/*! \brief �������캯��
	 * @param socket_address_ ��ֵ
	 */
	UNIXAddress (SA* socket_address_);

	/*! \brief ��������
	 */
	virtual ~UNIXAddress ();

	/*! \brief ��ȡ��ַ�ṹ�ߴ�
	 */
	const int getLength () const;

	/*! \brief ��ȡ��ַ�ṹʵ��
	 */
	SA* getAddress () const;
	
private:
	/*! \brief UNIX��ַ�ṹʵ��
	 */
	SA_UN m_address;
};

END_SUNRISE_NAMESPACE

#endif /* UNIX_ADDRESS_H */
