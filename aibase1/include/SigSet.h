//------------------------------------------------------------------------------
//                            SigSet.h
//------------------------------------------------------------------------------
#ifndef _SigSet_h
#define _SigSet_h

// System includes
//
#include <signal.h>
#include <errno.h>

#include "Prelude.h"

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr  �źŴ���ģ��(libsr)  
@{ 
*/

/*! \example sighand_test.cpp
 * SigSet���Գ���
 */

/*! \file SigSet.h 

\brief UNIXϵͳsigset_t�ṹ��İ�����, �ṩά��sigset_t�ṹ��ĺ���

��<tt>SitSet</tt>ת��Ϊ<tt>sigset_t</tt>ָ��������ʾ :

<pre>
   SigSet  source;
   sigset_t*  targetp;

   targetp = source;
</pre>

SigSet�ඨ����һ��ת����������<tt>SigSet</tt>ת��������Ϊ<tt>sigset_t*</tt>��
<tt>lvalue</tt>(��ֵ)
<p>
��һ������:

<pre>
   struct sigaction action;
   SigSet siganls_to_block;

   // manipulat signal set in some meaningful way

   action.sa_mask = *signals_to_block;
</pre>
*/

/*! \class SigSet SigSet.h
 * \brief UNIXϵͳsigset_t�ṹ��İ�����.
 */
class SigSet
{
public:
	/*! \brief ȱʡ���캯��
	*/
	SigSet();

	/*! \brief �������캯��
	 */
	SigSet(sigset_t* source_);
	
	/*! \brief ��������
	 */
	~SigSet();

	/*! \brief �����źż�
	    @return �ɹ��򷵻�0, ���򷵻�-1������errnoֵ
	*/
	int empty (void);

	/*! \brief ��ʼ���źż�Ϊ������POSIX.1�ﶨ����ź�.
	    @return �ɹ��򷵻�0, ���򷵻�-1������errnoֵ
	*/
	int fill(void);

	/*! \brief �����źŵ��źż�
	    @return �ɹ��򷵻�0, ���򷵻�-1������errnoֵ
	*/
	int add(int signo_);

	/*! \brief ���źż����Ƴ��ź�
	    @return �ɹ��򷵻�0, ���򷵻�-1������errnoֵ
	*/
	int del(int signo_);

	/*! \brief ���ĳ�ź��Ƿ����źż���
	    @return �ɹ��򷵻�0, ���򷵻�-1������errnoֵ
	*/
	int is_member(int signo_);

	/*! \brief ת��������
	 */
	operator sigset_t *();

private:
	sigset_t m_sigset;  /*!< POSIX�źż� */
};

inline
SigSet::
SigSet() { 	(int) sigemptyset(&m_sigset); }

inline
SigSet::
SigSet(sigset_t* s_) { m_sigset = *s_; }

inline 
SigSet::
~SigSet() { /* no-op */ }

inline int
SigSet::
empty(void) { return sigemptyset(&m_sigset); }

inline int
SigSet::
fill(void) { return sigfillset(&m_sigset); }

inline int
SigSet::
add(int signo_) { return sigaddset(&m_sigset,signo_); }

inline int
SigSet::
del(int signo_) { return sigdelset(&m_sigset,signo_); }

inline int
SigSet::
is_member(int signo_) { return sigismember(&m_sigset,signo_); }

inline 
SigSet::
operator sigset_t *() {	return &m_sigset; }

/** @} */

END_SUNRISE_NAMESPACE

#endif /* _SigSet_h */
