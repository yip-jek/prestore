//------------------------------------------------------------------------
//                            Acceptor.h
//------------------------------------------------------------------------
#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Prelude.h"
#include "Logger.h"
#include "EventHandler.h"
#include "Address.h"
#include "Reactor.h"
#include "ServiceHandler.h"

/*! \file Acceptor.h

  Acceptor封装接受TCP/IP连接请求的通用策略.

   This abstract class implementes the generic strategy for passive
   initializing communication services.

   SERVICE_HANDLER is the type of service. It shall be a type derived
   from ServiceHandler interface class.
   
   PEER_ACCEPTOR is the type of concrete Socket class - particular
   transport mechanism used by the Acceptor to passively establish 
   the connection. It should be derived from Socket interface class.

   @see Reactor
   @see ServiceHandler
*/

BEGIN_SUNRISE_NAMESPACE

/*! \class Acceptor
 * \brief TCP/IP连接请求接收器.
 * 封装TCP/IP连接请求, 配合中心分派模式. 在客户的连接请求到达时，向中心分派器
 * 注册这个套接字的读/写事件处理器.
 */
template<class SERVICE_HANDLER, class PEER_ACCEPTOR>
class Acceptor : public virtual EventHandler
{
public:
    /*! \brief 缺省构造函数
		@param r_ 中心分派器实例, 通常是个singleton.
    */
    Acceptor (Reactor* r_);

    /*! \brief 析构函数
    */
    virtual ~Acceptor ();
	
    /*! \brief 初始化通讯流.

		@return 成功返回0, 否则返回-1.
		失败的原因是PEER_ACCEPTOR的open方法或者bind方法的调用失败产生.
    */
    virtual int open (const Address& local_addr_);

    /*! \brief 关闭PEER_ACCEPTOR流.
		@return 成功返回0, 否则返回-1.
    */
    virtual int close (void);

    /*! \brief 当新的连接请求到达，中心分派器所回调的函数
	  * 缺省的策略是接收数据.衍生类可以通过重载修改行为.

		@return 成功返回0,否则返回-1.
		返回-1指使中心分派器从自己的事件处理器表中删除自己.
    */
    int handle_read (int fd);

    /*! \brief 当PEER_ACCEPTOR流出现错误,或者中心分派器被通知停止事件轮询的
	  * 时候回调这个函数. 缺省的行为是删除自己.如果调用对象不是reactor，可以
	  * 通过Reactor::removeHandler(this->id()), 删除自己。衍生类可以通过重载
	  * 修改行为.
	  * @return 这个函数总是返回-1.
    */
    virtual int handle_close (int fd);

protected:
    /*! \brief 为ServiceHandler定义创建策略.
		@return 返回SERVICE_HANDLER指针
    */
    virtual SERVICE_HANDLER* makeServiceHandler (PEER_ACCEPTOR* sock_);

    /*! \brief 接收新连接的缺省策略。
	  * 衍生类可以通过重载修改这个策略.
	
		@param new_socket_ 返回新创建的PEER_STREAM指针，如果失败返回0
		@return 成功返回0, 失败返回-1.
    */
    virtual int acceptServiceHandler (PEER_ACCEPTOR*& new_socket_);

    /*! \brief 定义并行策略.
	  * 缺省是为当前进程(线程)创建SERVICE_HANDLER. 调用其open方法，使
	  * 中心分派器处理I/O事件。衍生类可通过重载修改这个策略.
		
		@param new_socket_ [输入] 要激活的PEER_STREAM指针.
		@return 成功返回0, 失败返回-1.
    */
    virtual int activateServiceHandler (PEER_ACCEPTOR* new_socket_);

private:
    /*! \brief 内部的通讯流.
     */
    PEER_ACCEPTOR m_listenSocket;

    /*! \brief 中心分派器实例
     */
    Reactor* m_reactor;
};

// Convenience definitions

#define SH SERVICE_HANDLER
#define PA PEER_ACCEPTOR

//------------------------------------------------------------------------------
// Template member functions definitions
//------------------------------------------------------------------------------

template<class SH, class PA>
inline
Acceptor<SH, PA>::Acceptor (Reactor* r_) 
    : m_reactor (r_)
{
    trace("Acceptor::Acceptor");
}

template<class SH, class PA>
inline
Acceptor<SH, PA>::~Acceptor () 
{
    trace("Acceptor::~Acceptor");
}

template<class SH, class PA> 
inline int 
Acceptor<SH, PA>::close (void) 
{
    trace("Acceptor::close");
    m_listenSocket.close ();
    return 0;
}

template<class SH, class PA> 
inline int 
Acceptor<SH, PA>::handle_close (int /* fd */) 
{
    trace("Acceptor::handle_close");
	
    // Reactor::getInstance ()->removeHandler (this->id());
	
    // NOT IMPLEMENTED: This spot requires validation
    // whether Acceptor is created on the heap or
    // automatic memory.
	
    delete this;
    return -1;
}

template<class SH, class PA> 
inline SERVICE_HANDLER* 
Acceptor<SH, PA>::makeServiceHandler (PEER_ACCEPTOR* sock_) 
{
    trace("Acceptor<>::makeServiceHandler");
	
    return new SERVICE_HANDLER (sock_);
}

template<class SH, class PA> 
inline int 
Acceptor<SH, PA>::acceptServiceHandler (PEER_ACCEPTOR*& new_socket_) 
{
    trace("Acceptor::acceptServiceHandler");
	
    new_socket_ = m_listenSocket.accept ();
    return new_socket_ ? 0 : -1;
}

template<class SH, class PA> int 
Acceptor<SH, PA>::activateServiceHandler (PA* new_socket_) 
{
    trace("Acceptor::activateServiceHandler");

    if (!new_socket_) {
		return -1;
	}
    SH* sh = makeServiceHandler (new_socket_);
    sh->open ();
    return 0;
}

template<class SH, class PA> int
Acceptor<SH, PA>::open (const Address& local_addr_)
{
    trace("Acceptor::open");
	
    if ( !m_listenSocket.open (local_addr_.getAddress ()->sa_family) ) {
		return -1;
	}

    if ( !m_listenSocket.bind (local_addr_) ) {
		return -1;
	}

    m_reactor->registerIOHandler (
		this, m_listenSocket.getHandler (), READ_EVENT);
	
    DL((TRACE,"Acceptor on FD - %d\n", m_listenSocket.getHandler ()));
    return 0;
}

//------------------------------------------------------------------------------
// Accept all connections waiting in listen queue at once. This avoids going 
// through Reactor's event loop for each new connection.
//------------------------------------------------------------------------------

template <class SH, class PA> int
Acceptor<SH, PA>::handle_read (int fd_)
{
    trace("Acceptor<>::handle_read");

    int fd = m_listenSocket.getHandler ();

    if (fd != fd_) return -1;

    FdSet mask;
    timeval poll = {0, 0};
    PA* new_socket;

    do {
		if ( acceptServiceHandler (new_socket) == -1 ) {
			return -1;
		}
		if ( !activateServiceHandler (new_socket) == -1 ) {
			return -1;
		}
		mask.reset ();
		mask.setFd (fd);
    }
    while ( (::select (fd+1, &mask, NULL, NULL, &poll) == 1) );

    return 0;
}

END_SUNRISE_NAMESPACE

#endif /* ACCEPTOR_H */  
