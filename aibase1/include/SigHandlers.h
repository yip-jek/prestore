//------------------------------------------------------------------------------
//                            SigHandlers.h
//------------------------------------------------------------------------------

#ifndef _SigHandlers_h
#define _SigHandlers_h

// System includes
//
#include <signal.h>
#include <errno.h>
#include <sys/time.h>		// select
#include <sys/types.h>		// select

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "SigHandler.h"
#include "SigHandlersList.h"
#include "Handlers.h"

BEGIN_SUNRISE_NAMESPACE
/** \defgroup libsr  �źŴ���ģ��(libsr)  
@{ 
*/


/*! \example sighands_test.cpp
 * \brief SigHandlers, SigHandlersList, Semaphore�Ĳ��Գ���
 */

/*! \file SigHandlers.h 

  \brief �źŴ�����������.�����û�Ϊһ���ź�ע�����¼�������(EventHandler).
  �źŴ�����������ʵ���ĳ�ʼ״̬�Ǳ����ģ���û���ض��ķ��ɶ�������.
  
  �û����ø��¼���������ͬ��ʵ��(����SigAction. @see SigAction)��Ϊ
  �źŴ�����ע��.

	�����ڵ�һ���¼���������register()����������, SigHandlers����������
	��״̬. SigHandlresȡ����η����źŵĿ���Ȩ�����źŴ�OS���ݹ�������
	�б��ϵ������¼����������ᱻ֪ͨ.
*/

/*! \class SigHandlers SigHandlers.h
 * \brief �źŴ�����������.
 */
class SigHandlers : public SigHandler
{
public:
	/*! \brief SigHandlres::dispatch()�����ľ�̬����������
	    @param signum_ �ź�
	*/
	static void sighandlers_dispatcher (int signum_);

	/*! \brief ע���¼�������(EventHandler)

	    @param signum_   (In   ) �ź�
	    @param new_hand_ (In   ) �µ��źŴ�����ָ��
	    @param new_disp_ (In=0 ) �µ��źŴ�����
	    @param old_hand_ (Out=0) ���ԭ�е��¼�������
	    @param old_disp_ (Out=0) ���ԭ�е��źŲ���

	    @return �ɹ�����0, ���򷵻�-1.
	 */
	virtual int install (int            signum_,
						 EventHandler*  new_hand_,
						 SigAction*     new_disp_ = 0,
						 EventHandler** old_hand_ = 0,
						 SigAction*     old_disp_ = 0);
			     
	/*! \brief ����ע��Ĺ���signum_�Ĵ������б����Ƴ��¼�������
	    ���eh_ΪNULL, ��������signum_�������źŴ�������
		 �����Ƴ�, ��ʱSigHandlers�ֳ��ڱ���״̬��û���κη���
		 ����. ���new_disp_ is omitted, ���źŴ���ʽ��ΪSIG_DFL.

	    @param signum_   (In   ) �ź�
	    @param eh_       (In   ) ���Ƴ����¼�������
	    @param new_disp_ (In=0 ) �µ��źŲ���
	    @param old_disp_ (Out=0) ���ԭ�е��źŲ���

	    @return �ɹ�����0, ���򷵻�-1.
	*/
	virtual int remove (int           signum_, 
						EventHandler* eh_,
						SigAction*    new_disp_ = 0, 
						SigAction*    old_disp_ = 0);
private:
	/*! \brief ���ɺ���
	 	 dispatch()��׽�ź�, ֪ͨ��ص�EventHandler���󲢵�����ص�����.
	*/
	static void dispatch (int signum_);
};

/** @} */

END_SUNRISE_NAMESPACE

#endif /* _SigHandlers_h */
