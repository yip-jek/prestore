//------------------------------------------------------------------------------
//                            SigHandler.h
//------------------------------------------------------------------------------
#ifndef _SigHandler_h
#define _SigHandler_h

// System includes

#include <signal.h>
#include <errno.h>

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "Logger.h"
#include "IdSet.h"
#include "SigSet.h"
#include "SigAction.h"
#include "EventHandler.h"

BEGIN_SUNRISE_NAMESPACE
/** \defgroup libsr  �źŴ���ģ��(libsr)  
@{ 
*/

/*! \example sighand_test.cpp
 * SigHandler���Գ���
 */


/** \file SigHandler.h 

   \brief UNIX �źŴ������Ĺ���/����(dispatch)��, ����Ӧ�ó���������źŷ�����
	�Ľ�ɫ.ʹ�ûص�ģʽ(callback pattern)�Ϻô����˴�ͳ���ڳ�����ֱ��ʹ��
	ϵͳ����API��������ȱ�ݣ�

     - ȱ����װ
	  - �źŴ����������޵ķ�����

   Ϊ����Ӧ�ó����UNIX�źŵķ�Ӧ, SigHandler������װEventHandler����, 
	����ע����ź���OS���ݵ�Ӧ�ó���EventHandler��handle_signal()������
	�ᱻ����.
*/

/*! \class SigHandler SigHandler.h
 * \brief UNIX �źŴ������Ĺ���/����(dispatch)��.
 */
class SigHandler
{
public:
	/*! \brief �����µ��źŴ��������µ��źŲ���.
	    ע�⣬��Ȼnew_disp_���д�������new_hand_����������
		 �ڴ����źš�Ҳ����˵��new_disp_.sa_handler�����ԡ�

	    @param signum_ �ź�
	    @param new_hand_ ���ڴ����ź�EventHandlerʵ��ָ��
	    @param new_disp_ �û������źŲ���
	    @param old_hand_ ����ԭ�е�EventHandlerָ��
	    @param old_disp_ ����ԭ�еĲ���

	    @return �ɹ�����0, ���򷵻�-1.
	*/
	virtual int install (int            signum_,
			     EventHandler*  new_hand_,
			     SigAction*     new_disp_ = 0,
			     EventHandler** old_hand_ = 0,
			     SigAction*     old_disp_ = 0);

	/*! \brief �Ƴ�signum_��EventHandler.ͬʱ��װ�µĲ���ͷ��ؾɵĲ���.

	    @param signum_ �ź�
	    @param eh_ �Ƴ���EventHandlerָ��. ���eh_Ϊ0(ȱʡ), ���й���
		 				����ź�signum_��EventHandler���ᱻ�Ƴ�.
	    @param new_disp_ �����źŵ��²���
	    @param old_disp_ �����źŵ�ԭ�в���.

	    @return �ɹ�����0, ���򷵻�-1.
	*/
	virtual int remove (int signum_,
			    EventHandler* eh_       = 0,
			    SigAction*    new_disp_ = 0,
			    SigAction*    old_disp_ = 0);

	/*! \brief ����
	 	 dispatch()��׽�ź�, ֪ͨ��ص�EventHandler���󲢵�����ص�����.

	    @param signum_ �ź�
	    @return �ɹ�����0, ���򷵻�-1.
	*/
	static void dispatch (int signum_);
    
	/*! \brief �����µ�EventHandler������ԭ�е��¼�������.

	    @return ����ָ��ԭ���¼�������(event handler)����signum_��û��ע����Ļ�������0.
	*/
	EventHandler* handler (int signum_, EventHandler* new_);

	/*! \brief ��ȡ�ź�signum_��ǰ���¼�������.
	 */
	EventHandler* handler (int signum_);
    
protected:
	/*! \brief ���signum_�Ƿ�Ƿ�(����POSIX.1�淶����ķ�Χ��)
	    @return �Ϸ�����0,���򷵻�-1.
	*/
	int in_range(int signum_);
    
private:
	/*! \brief ��̬���¼�����������
	*/
	static EventHandler* m_signal_handlers [NSIG];
};

/** @} */

END_SUNRISE_NAMESPACE

#endif /* _SigHandler_h */
