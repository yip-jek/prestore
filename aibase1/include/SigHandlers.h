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
/** \defgroup libsr  信号处理模块(libsr)  
@{ 
*/


/*! \example sighands_test.cpp
 * \brief SigHandlers, SigHandlersList, Semaphore的测试程序
 */

/*! \file SigHandlers.h 

  \brief 信号处理器管理类.允许用户为一个信号注册多个事件处理器(EventHandler).
  信号处理器管理类实例的初始状态是被动的，还没有特定的分派动作可做.
  
  用户可用跟事件处理器不同的实例(例如SigAction. @see SigAction)作为
  信号处理器注册.

	当对于第一个事件处理器的register()方法被调用, SigHandlers立即处于主
	动状态. SigHandlres取得如何分派信号的控制权。当信号从OS传递过来，在
	列表上的所有事件处理器都会被通知.
*/

/*! \class SigHandlers SigHandlers.h
 * \brief 信号处理器管理类.
 */
class SigHandlers : public SigHandler
{
public:
	/*! \brief SigHandlres::dispatch()方法的静态包裹函数。
	    @param signum_ 信号
	*/
	static void sighandlers_dispatcher (int signum_);

	/*! \brief 注册事件处理器(EventHandler)

	    @param signum_   (In   ) 信号
	    @param new_hand_ (In   ) 新的信号处理器指针
	    @param new_disp_ (In=0 ) 新的信号处理部署
	    @param old_hand_ (Out=0) 存放原有的事件处理器
	    @param old_disp_ (Out=0) 存放原有的信号部署

	    @return 成功返回0, 否则返回-1.
	 */
	virtual int install (int            signum_,
						 EventHandler*  new_hand_,
						 SigAction*     new_disp_ = 0,
						 EventHandler** old_hand_ = 0,
						 SigAction*     old_disp_ = 0);
			     
	/*! \brief 从已注册的关于signum_的处理器列表中移除事件处理器
	    如果eh_为NULL, 则关于这个signum_的所有信号处理器都
		 将被移除, 这时SigHandlers又出于被动状态，没有任何分派
		 动作. 如果new_disp_ is omitted, 则信号处理方式变为SIG_DFL.

	    @param signum_   (In   ) 信号
	    @param eh_       (In   ) 被移除的事件处理器
	    @param new_disp_ (In=0 ) 新的信号部署
	    @param old_disp_ (Out=0) 存放原有的信号部署

	    @return 成功返回0, 否则返回-1.
	*/
	virtual int remove (int           signum_, 
						EventHandler* eh_,
						SigAction*    new_disp_ = 0, 
						SigAction*    old_disp_ = 0);
private:
	/*! \brief 分派函数
	 	 dispatch()捕捉信号, 通知相关的EventHandler对象并调用其回调函数.
	*/
	static void dispatch (int signum_);
};

/** @} */

END_SUNRISE_NAMESPACE

#endif /* _SigHandlers_h */
