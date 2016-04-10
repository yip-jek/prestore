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
    \brief ά������id��, ����ϵͳ�����ּ�, ���ֵ��1024.
 */

/*! \class IdSet IdSet.h
 * \brief ����id��������
 */
class IdSet
{
public:
	/*! \brief ȱʡ���캯��
	 * ���е�id��Ϊ0
	 */
	IdSet ();

	/*! \brief ����һ������id, ��������Ϊ�ѱ�ʹ��,������δ��
	 * ʹ�õ�id������С����Ϊ��һ�η���ֵ.
	 */
	int newid ();

	/*! \brief ��id_��Ϊ���ã������´�newid()����ֵ.
	*/
	int recycle (int id_);

	/* \brief ����һ������id, ���������´�ȡ��ʱ�ķ���ֵ.
	*/
	int currid () const;

private:
	int m_next_available_id; /*!< ��ǰ���õ�id */

	fd_set m_id_set_map; /*!< id�� */
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
