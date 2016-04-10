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
/** \defgroup libsr  信号处理模块(libsr)  
@{ 
*/

/*! \example sighand_test.cpp
 * SigHandler测试程序
 */


/** \file SigHandler.h 

   \brief UNIX 信号处理函数的管理/分派(dispatch)类, 担当应用程序的中央信号分派器
	的角色.使用回调模式(callback pattern)较好处理了传统的在程序里直接使用
	系统调用API所带来的缺陷：

     - 缺乏封装
	  - 信号处理器中有限的泛函性

   为管理应用程序对UNIX信号的反应, SigHandler类允许安装EventHandler对象, 
	当被注册的信号由OS传递到应用程序，EventHandler的handle_signal()方法将
	会被调用.
*/

/*! \class SigHandler SigHandler.h
 * \brief UNIX 信号处理函数的管理/分派(dispatch)类.
 */
class SigHandler
{
public:
	/*! \brief 增加新的信号处理函数和新的信号部署.
	    注意，虽然new_disp_含有处理函数，new_hand_才是真正用
		 于处理信号。也就是说，new_disp_.sa_handler被忽略。

	    @param signum_ 信号
	    @param new_hand_ 用于处理信号EventHandler实例指针
	    @param new_disp_ 用户处理信号部署
	    @param old_hand_ 返回原有的EventHandler指针
	    @param old_disp_ 返回原有的部署

	    @return 成功返回0, 否则返回-1.
	*/
	virtual int install (int            signum_,
			     EventHandler*  new_hand_,
			     SigAction*     new_disp_ = 0,
			     EventHandler** old_hand_ = 0,
			     SigAction*     old_disp_ = 0);

	/*! \brief 移除signum_的EventHandler.同时安装新的部署和返回旧的部署.

	    @param signum_ 信号
	    @param eh_ 移除的EventHandler指针. 如果eh_为0(缺省), 所有关于
		 				这个信号signum_的EventHandler都会被移除.
	    @param new_disp_ 处理信号的新部署
	    @param old_disp_ 返回信号的原有部署.

	    @return 成功返回0, 否则返回-1.
	*/
	virtual int remove (int signum_,
			    EventHandler* eh_       = 0,
			    SigAction*    new_disp_ = 0,
			    SigAction*    old_disp_ = 0);

	/*! \brief 分派
	 	 dispatch()捕捉信号, 通知相关的EventHandler对象并调用其回调函数.

	    @param signum_ 信号
	    @return 成功返回0, 否则返回-1.
	*/
	static void dispatch (int signum_);
    
	/*! \brief 设置新的EventHandler并返回原有的事件处理器.

	    @return 返回指向原有事件处理器(event handler)或者signum_并没有注册过的话，返回0.
	*/
	EventHandler* handler (int signum_, EventHandler* new_);

	/*! \brief 获取信号signum_当前的事件处理器.
	 */
	EventHandler* handler (int signum_);
    
protected:
	/*! \brief 检查signum_是否非法(不在POSIX.1规范定义的范围内)
	    @return 合法返回0,否则返回-1.
	*/
	int in_range(int signum_);
    
private:
	/*! \brief 静态的事件处理器数组
	*/
	static EventHandler* m_signal_handlers [NSIG];
};

/** @} */

END_SUNRISE_NAMESPACE

#endif /* _SigHandler_h */
