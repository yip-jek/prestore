//------------------------------------------------------------------------------
//                             ServiceHandler.h
//------------------------------------------------------------------------------
#ifndef SERVICE_HANDLER_H
#define SERVICE_HANDLER_H

#include "Prelude.h"
#include "Assert.h"
#include "EventHandler.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file ServiceHandler.h
	\brief 提供服务处理接口的抽象类
*/

/*! \class ServiceHandler
 * \brief 服务处理类
 */
template <class PEER_STREAM>
class ServiceHandler : public EventHandler
{
public:
	ServiceHandler () 
		: m_peerStream (new PEER_STREAM)
		{
			trace("ServiceHandler::ServiceHandler");
		}
		
	ServiceHandler (PEER_STREAM* ps_)
		: m_peerStream (ps_) 
		{
			trace("ServiceHandler::ServiceHandler");
		}

	virtual ~ServiceHandler () {
		trace("ServiceHandler::~ServiceHandler");

		if ( m_peerStream ) {
			delete m_peerStream;
			m_peerStream = (PEER_STREAM*) NULL;
		}
	}
	
	virtual int open (void) = 0;

	virtual void close (void) 
		{
			trace("ServiceHandler::close");
			if ( m_peerStream ) m_peerStream->close ();
		}

	operator PEER_STREAM& () 
		{ 
			trace("ServiceHandler::opt PEER_STREAM& ()");
			return *m_peerStream;
		}

	PEER_STREAM& get_stream () { return *m_peerStream; }

protected:
	PEER_STREAM* m_peerStream;
};

END_SUNRISE_NAMESPACE

#endif /* SERVICE_HANDLER_H */
