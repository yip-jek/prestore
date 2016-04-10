#ifndef _EventHandler_h
#define _EventHandler_h

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif

#include "Assert.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file EventHandler.h 
    \brief 处理I/O事件,时间事件等等的抽象接口
*/

/*! \var unsigned long TimerId.
	用于handle_timeout()调用. 
*/
typedef unsigned long TimerId;

/*! \enum EventType
 事件类型定义
*/
enum EventType
{	
	READ_EVENT    = 0x01,	/*!< 一旦IO读通道能在非阻塞的状态下读入至少1bytes
									  时通知中心分派器。 */
	WRITE_EVENT   = 0x02,	/*!< 一旦IO写通道能在非阻塞的状态下写入至少1bytes时通知
									  中心分派器. */
	EXCEPT_EVENT  = 0x04,	/*!< 一旦在TCP层检测到异常情况时通知中心分派器 */
	TIMEOUT_EVENT = 0x10,	/*!< 超时事件发生时通知中心分派器. */
	SIGNAL_EVENT  = 0x20,	/*!< 一旦OS传递信号(signal)给应用程序时通知中心分派器. */
	RWE_EVENTS    = 0x07,	/*!< READ_EVENT | WRITE_EVENT | EXCEPT_EVENT   */
	ALL_EVENTS    = 0x37	/*!< 包含以上所有事件 */
};

bool isReadEvent (EventType e_) ;
bool isWriteEvent (EventType e_) ;
bool isExceptEvent (EventType e_) ;
bool isTimeoutEvent (EventType e_) ;
bool isSignalEvent (EventType e_) ;
bool isRWEEvents (EventType e_) ;
bool isAllEvents (EventType e_) ;

/*! \class EventHandler EventHandler.h "include/EventHandler.h"
 * EventHandler是一个虚基类.
 * 它是事件处理器 - 当事件发生是被中心分派器回调 - 的接口. 这些事件包括
 * 信号，时间，I/O管道等文件描述字，Sockets等等。
 * \see EventType
*/
class EventHandler
{
public:
	/*! \brief 缺省构造函数
	 */
	EventHandler();

	/*! \brief 读事件的回调函数. 如遇到EOF，必需返回-1
	    @return 成功返回0,否则返回-1.
	*/
	virtual int handle_read (int fd);

	/*! \brief 读事件的回调函数.
	    @return 成功返回0,否则返回-1.
	*/
	virtual int handle_write (int fd);

	/*! \brief 异常事件的回调函数
	    @return 成功返回0,否则返回-1.
	*/
	virtual int handle_except (int fd);

	/*! \brief 超时事件的回调函数.
		返回值被忽略，如果下次还要处理超时事件，必需在返回前重新安装事件处理器.
	    @return 成功返回0,否则返回-1.
	*/
	virtual int handle_timeout (TimerId tid);

	/*! \brief 信号处理的回调函数
	    @return 成功返回0,否则返回-1.
	*/
	virtual int handle_signal(int signum_); 

	/*! \brief 对方套接字到达EOF时的回调函数.
	    没有相应的事件类型，这个回调函数不需要，如果handle_read()已经处理了
		 EOF的情况时。
	    @return 成功返回0,否则返回-1.
	*/
	virtual int handle_close (int fd);

	/*! \brief 子类重置内部状态的钩子函数
	*/
	virtual void resetState (void);
};


/*! @var int (EventHandler::*EH_IO_Callback) (int)
A type for the pointer to I/O-related callback member function 
of class EventHandler.
These are:
   - handle_read ()
   - handle_write ()
   - handle_except ()

@see EventHandler
*/
typedef int (EventHandler::*EH_IO_Callback) (int);

END_SUNRISE_NAMESPACE

#endif // _EventHandler_h
