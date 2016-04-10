//------------------------------------------------------------------------------
//                                 MemDump.h
//------------------------------------------------------------------------------
#ifndef MEM_DUMP_H
#define MEM_DUMP_H

#include <stdio.h>
#include <string.h>

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "Logger.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file MemDump.h
	\brief Hex/Ascii�ڴ�ת����.
	���ڴ�Ӱ��ת��Ϊhex-ascii�Գ��֡�
*/

/*! \class MemDump MemDump.h
 * \brief �ڴ�ת����
 *	���ڴ�Ӱ��ת��Ϊhex-ascii�Գ��֡�
 */
class MemDump 
{
private:
	char* m_dump; /*!< Ҫ��ת�����ڴ�ָ��. */

	static const char m_empty_str[];
	
public:
	/*! \brief ���캯��
	 *  ת���ڴ�Ӱ��Ϊ16���ƺ�ascii��������
	 *  MemDump����ӵ������ڴ棬���������ڽ���ǰ�ͷ�����ڴ�
	    
	    @param msg_  ԭ�ڴ�Ӱ������
	    @param len_  �ڴ�Ӱ�񳤶�
	*/
	MemDump (const char* msg_, int len_);

	/*! \brief ��������
	 * �ͷ��ڴ�Ӱ��
	 */
	~MemDump();

	/*! \brief ����ڴ�Ӱ���16���ƻ���ascii�ַ���.
	    @return ��ת���˵��ַ���ָ��.
	*/
	const char* getMemDump() const;

	/*! \brief ���ڴ�Ӱ���hex/ascii��д����־.
	 *
	 * @param mask_ Debug����
	 * @param info_ hex�ڴ�Ӱ���ע���־�.
	 * @param msg_  �ڴ���ָ��
	 * @param len_  �ֽ���
	 */
	static void dump_to_log (u_long mask_, 
							 const char* info_, 
							 const char* msg_, 
							 int len_);
};


inline 
MemDump::
~MemDump()
{
	if (m_dump && m_dump != m_empty_str) {
		delete [] m_dump;
	}
	m_dump = NULL;
}

inline const char*
MemDump::
getMemDump () const
{
	return (m_dump ? (const char*) m_dump : m_empty_str);
}

END_SUNRISE_NAMESPACE

#endif /* MEM_DUMP_H */
