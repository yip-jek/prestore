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
 * ϵͳ����select()�õ��ļ����������뼯
 */

/*! \class MaskSet
 */
class MaskSet
{
public:
	FdSet m_rset; /*!< �������ּ� */
	FdSet m_wset; /*!< д�����ּ� */
	FdSet m_eset; /*!< �쳣�����ּ� */

public:
	/*! \brief �����������ּ��ṹ������ */
	void reset (void);

	/*! \brief ��MaskSet����ĵ�ǰ״̬д����־�ļ� */
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
