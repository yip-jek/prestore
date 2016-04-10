//------------------------------------------------------------------------------
//                                IdSet.h
//------------------------------------------------------------------------------
#ifndef ID_SET_H
#define ID_SET_H

#include <string.h>

#include <sys/time.h>		/* select(3) */

#if defined(Linux)		/* select(3) */
#    include <sys/types.h>
#    include <unistd.h>
#endif

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif

BEGIN_SUNRISE_NAMESPACE

/* \file IdSet.h 
    \brief 维护可用id集, 类似系统描述字集, 最大值是1024.
 */

/*! \class IdSet IdSet.h
 * \brief 可用id集管理类
 */
class IdSet
{
public:
	/*! \brief 缺省构造函数
	 * 所有的id设为0
	 */
	IdSet ();

	/*! \brief 返回一个可用id, 将其设置为已被使用,并设置未被
	 * 使用的id集中最小者作为下一次返回值.
	 */
	int newid ();

	/*! \brief 是id_成为可用，调整下次newid()返回值.
	*/
	int recycle (int id_);

	/* \brief 返回一个可用id, 但不调整下次取用时的返回值.
	*/
	int currid () const;

private:
	int m_next_available_id; /*!< 当前可用的id */

	fd_set m_id_set_map; /*!< id集 */
};

inline
IdSet::
IdSet()
	: m_next_available_id (0)
{
	::memset (&m_id_set_map, 0, sizeof (m_id_set_map));
}
 
inline int
IdSet::
currid() const
{
	return m_next_available_id;
}

END_SUNRISE_NAMESPACE
#endif /* ID_SET_H */
