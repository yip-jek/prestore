//------------------------------------------------------------------------------
//                               MaskSet.h
//------------------------------------------------------------------------------
#ifndef MASK_SET_H
#define MASK_SET_H

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "FdSet.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file MaskSet.h
 * 系统调用select()用的文件描述字掩码集
 */

/*! \class MaskSet
 */
class MaskSet
{
public:
	FdSet m_rset; /*!< 读描述字集 */
	FdSet m_wset; /*!< 写描述字集 */
	FdSet m_eset; /*!< 异常描述字集 */

public:
	/*! \brief 对所有描述字集结构体清零 */
	void reset (void);

	/*! \brief 把MaskSet对象的当前状态写入日志文件 */
	void dump (void);
};

inline void 
MaskSet::
reset (void) 
{
	m_rset.reset ();
	m_wset.reset ();
	m_eset.reset ();
}

inline void 
MaskSet::
dump (void) 
{
	DL((REACTTRACE,"--------------------\n"));
	DL((REACTTRACE,"Read FDs set:\n"));
	m_rset.dump ();

	DL((REACTTRACE,"Write FDs set:\n"));
	m_wset.dump ();

	DL((REACTTRACE,"Except FDs set:\n"));
	m_eset.dump ();
	DL((REACTTRACE,"--------------------\n"));
}

END_SUNRISE_NAMESPACE

#endif /* MASK_SET_H */  
