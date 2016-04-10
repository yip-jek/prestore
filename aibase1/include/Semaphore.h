//------------------------------------------------------------------------------
//                               Semaphore.h
//------------------------------------------------------------------------------

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "Assert.h"	// trace() & Assert family

BEGIN_SUNRISE_NAMESPACE

/** \addtogroup libsr �������߿�(libsr) 
@{ 
*/

/*! \file Semaphore.h 
  
  \brief ��װSystem V�ź���ϵͳ����.

  �ź�������ʹ���������̺��ӽ���֮�䣬����ͬ���ӽ��̵ĳ�ʼ��.
*/

/*! \example semaphore_test.cpp
 * \brief ����Semaphore.
 */

/*! \class Semaphore
 * �ź���������, ��װ��System V�����ź�����ϵͳ����
 */
class Semaphore
{
public:
	/*! \brief ���캯��
	 */
	Semaphore ();

	/*! \brief ��������
	 */
	virtual ~Semaphore ();

	int create (key_t key_, int initval_ = 1);

	int open (key_t key_);

	void close ();

	void remove ();

	void wait ();

	void signal ();

	void op (int val_);

	/*! \brief ��ȡkey
	 */
	key_t key () const { return m_key; }

	/*! \brief ��ȡid
	 */
	int id () const { return m_id; }

	/*! \brief ���ź��������״̬д����־�ļ�
	*/
	void dump (void) const;

protected:
	/*! \brief ��ʼ��
	 */
	void init ();

protected:
	/*! \brief �ź�����ֵ
	 */
	key_t   m_key;

	/*! \brief �ź���id
	 */
	int     m_id;

protected:
	static const int BIGCOUNT;

	static sembuf m_op_lock      [2];

	static sembuf m_op_endcreate [2];

	static sembuf m_op_open      [2];

	static sembuf m_op_close     [3];

	static sembuf m_op_unlock    [1];

	static sembuf m_op_op        [1];
};

inline 
Semaphore::
Semaphore ()
{
	trace_with_mask("Semaphore::Semaphore", SEM);

	init ();
}

inline
Semaphore::
~Semaphore ()
{
	trace_with_mask("Semaphore::~Semaphore", SEM);
	
	if (m_id > 0) {
		this->close ();
	}
}

inline void
Semaphore::
init ()
{
	m_key = (key_t) -1;
	m_id  = -1;
}	

inline void
Semaphore::
wait ()
{
	trace_with_mask("Semaphore::wait", SEM);
	op (-1);
}

inline void
Semaphore::
signal ()
{
	trace_with_mask("Semaphore::signal", SEM);
	op (1);
}
/** @} */

END_SUNRISE_NAMESPACE

#endif /* SEMAPHORE_H */  
