//------------------------------------------------------------------------------
//                            SigAction.h
//------------------------------------------------------------------------------

#ifndef _SigAction_h
#define _SigAction_h

// System includes
//
#include <signal.h>
#include <errno.h>

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "Assert.h"
#include "SigSet.h"

// some convenient typedefs
//
extern "C" {
typedef struct sigaction SIGACTION;
typedef void (*C_SIG_HANDLER)( int );
}

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr  �źŴ���ģ��(libsr)  
@{ 
*/


/*! \example sighand_test.cpp
 * SigAction�Ĳ��Գ���
 */

/*! \file SigAction.h

\brief SigAction��UNIXϵͳ��sigaction�ṹ��C++������

SigAction��ʵ��sigaction�ṹ�İ���,���ṩ���п�����sigaction��
�Ĳ���������sigaction(2)ϵͳ����

sigaction�ṹ�����ǣ�

<pre>
   struct sigaction {
       void      (*sa_handler) ();
       sigset_t  sa_mask;
       int       sa_flags;
       void      (*sa_sigaction) (int, siginfo_t*, void*);
   };
</pre>

�����趨��Ľ�����ĳ�źŵ����ʱ��ִ��ʲô������SigAction���װ
���ض��źŷ���ʱ�Ķ�����

����Ҫ�ĳ�Ա�Ǵ�ź���ָ���sa_handler,�������յ�ĳ�ź����������
�������á��źŶ�����POSIX.1�淶��.

SigAction�๹�캯���Ĳ���C_SIG_HANDLER
�������µ�ѡ��֮һ:
   - "C" �źŴ�����(sigaction�ṹ��sa_handler):\n
      void sig_handler (int signum)
   - SIG_ERR 
   - SIG_DFL ʹ��ϵͳȱʡ���źŴ�����,ͨ�����Ǽ򵥵���abort��ʹ�����˳���
   - SIG_IGN �����ź�(��Ȼ, signal 9���ܱ�����)

sa_mask����ָ�����źŴ��������ڴ����ʱ����Щ�źŽ�������.
���⣬һЩ�ź�Ҳ��ʹ����ִ�е��źŴ���������(SIGSTOP��SIGKILL
���ܱ�����).

sa_flags��־��(�ɸ�flag��λ�����)ָ�����Ӱ���źŴ�����
��ִ��ʱ����ִ�к����Ϊ.�����OS����Щ����
��ͨ��man 2 sigaction����鿴.

SigAction��Ϊsigaction(2)�İ����࣬Ҫ��Ӧ��������OS.
��sig_handler������(���ߴ�����),����ϵͳ�����°�װ��������
��ǰ���źŴ�����.Ҫ����ΪSIG_DFL, SUN Solarisʹ��SA_RESETHAND
��־��FreeBSD��Linux OS��ʹ��SA_ONESHOT��־����AIXʹ��SA_RESETHAND.
�鷳����ֲ����. :)
*/
/*! \class SigAction SigAction.h
 * \brief SigAction��UNIXϵͳ��sigaction�ṹ��C++������
 */
class SigAction
{
public:
	/*! \brief ȱʡ���캯��.
	*/
	SigAction();

	/*! \brief ���캯��
	    ���źŴ���������SigAction����.

	    @param handler_ �źŴ�����ָ��
	    @param sig_mask_ ���źŴ�����ִ��ʱ�������źż�
	    @param flags_ �����źŴ�������Ϊ�ı�־��(�ض��ڲ���ϵͳ, �ο�manpages)
	*/
	SigAction (C_SIG_HANDLER handler_, 
			   SigSet*       sig_mask_ = 0, 
			   int           flags_ = 0);

	/*! \brief ���캯��
	  	 ���źŴ�����ָ�빹��SigAction�������������ź�signum_.<p>
	    ��һ�������Ǻ���ָ��.�����ǷǾ�̬��C++���Ա������@see SigHandler

	    @param handler_ �źŴ�����ָ��
	    @param signum_  ������ź�
	    @param sig_mask_ �źŴ�����ִ�е�ʱ�����������źż�
	    @param flags_ �����źŴ�������Ϊ�ı�־��(�ض��ڲ���ϵͳ, �ο�manpages)
	*/
	SigAction (C_SIG_HANDLER handler_,
			   int           signum_,
			   SigSet*       sig_mask_ = 0, 
			   int           flags_ = 0);

	/*! \brief ��Ϊsignum_��ǰ���źŲ�����ע��, �ɵĲ�������oaction_��

	    @param signum_ �ź�
	    @param oaction_ �ɲ���Ĵ�Ŵ�
	    @return �ɹ��Ƿ���0, ���򷵻�-1,���趨errnoֵ
	*/
	int register_action (int signum_, SigAction* oaction_ = 0);

	/*! \brief ��signum_���źŲ����޸�Ϊoaction_,

	    @param signum_ ���޸ĵ��ź�
	    @param oaction_ �źŲ���
	    @return �ɹ��Ƿ���0, ���򷵻�-1,���趨errnoֵ
	*/
	int restore_action (int signum_, SigAction& oaction_);

	/*! \brief ȡ���ź�signum_��ǰ�Ĳ���
	    @param signum_ �ź�
	    @return �ɹ��Ƿ���0, ���򷵻�-1,���趨errnoֵ
	*/
	int retrieve_action (int signum_);

	/*! \brief ����sigaction
	    @param sa_ �µ�sigactionֵ
	*/
	void action (SIGACTION * sa_);

	/*! \brief ���ص�ǰ��sigactionʵ��ָ��
	    @return ���ص�ǰ��sigactionʵ��ָ��
	*/
	SIGACTION * action ();

	/*! \brief ���ñ�־��(Ӱ���źŴ�������Ϊ�ı�־��)
	    @param new_flags_ �µı�־��
	*/
	void flags (int new_flags_);

	/*! \brief ��ȡ��ǰ�ı�־��
	    @return  ���ص�ǰ�ı�־��ֵ
	*/
	int flags ();

	/*! \brief �����µ��ź�����
	 */
	void mask (SigSet & mask_set_);

	/*! \brief ��ȡ��ǰ���ź�����
	 */
	SigSet mask ();

	/*! \brief �����µ��źŴ�����
	 */
	void handler (C_SIG_HANDLER sha_);

	/*! \brief ��ȡ��ǰ���źŴ�����ָ��
	 */
	C_SIG_HANDLER handler ();

	/*! \brief ����ת��������
	  ��SigAction����ת��Ϊ�ڲ�sigaction�ṹ�ĺ���ָ��
	*/
	operator SIGACTION *();
  
private: 
	SIGACTION m_sa;  /*!< sigaction�ṹʵ��  */
};

/** @} */

END_SUNRISE_NAMESPACE

#endif /* _SigAction_h */
