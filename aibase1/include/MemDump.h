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
	\brief Hex/Ascii内存转储类.
	把内存影像转换为hex-ascii以呈现。
*/

/*! \class MemDump MemDump.h
 * \brief 内存转储类
 *	把内存影像转换为hex-ascii以呈现。
 */
class MemDump 
{
private:
	char* m_dump; /*!< 要被转换的内存指针. */

	static const char m_empty_str[];
	
public:
	/*! \brief 构造函数
	 *  转换内存影像为16进制和ascii码来呈现
	 *  MemDump对象拥有这块内存，并在生命期结束前释放这段内存
	    
	    @param msg_  原内存影像内容
	    @param len_  内存影像长度
	*/
	MemDump (const char* msg_, int len_);

	/*! \brief 析构函数
	 * 释放内存影像
	 */
	~MemDump();

	/*! \brief 获得内存影像的16进制或者ascii字符串.
	    @return 被转换了的字符串指针.
	*/
	const char* getMemDump() const;

	/*! \brief 把内存影像的hex/ascii串写入日志.
	 *
	 * @param mask_ Debug掩码
	 * @param info_ hex内存影像的注释字句.
	 * @param msg_  内存区指针
	 * @param len_  字节数
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
