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

/** \addtogroup libsr 基础工具库(libsr) 
@{ 
*/

/*! \file Semaphore.h 
  
  \brief 封装System V信号量系统调用.

  信号量常常使用在主进程和子进程之间，用于同步子进程的初始化.
*/

/*! \example semaphore_test.cpp
 * \brief 测试Semaphore.
 */

/*! \class Semaphore
 * 信号量包裹类, 封装了System V关于信号量的系统调用
 */
class Semaphore
{
public:
	/*! \brief 构造函数
	 */
	Semaphore ();

	/*! \brief 析构函数
	 */
	virtual ~Semaphore ();

	int create (key_t key_, int initval_ = 1);

	int open (key_t key_);

	void close ();

	void remove ();

	void wait ();

	void signal ();

	void op (int val_);

	/*! \brief 获取key
	 */
	key_t key () const { return m_key; }

	/*! \brief 获取id
	 */
	int id () const { return m_id; }

	/*! \brief 把信号量对象的状态写入日志文件
	*/
	void dump (void) const;

protected:
	/*! \brief 初始化
	 */
	void init ();

protected:
	/*! \brief 信号量键值
	 */
	key_t   m_key;

	/*! \brief 信号量id
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
