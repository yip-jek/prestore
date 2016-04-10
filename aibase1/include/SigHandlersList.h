//---------------------------------------------------------------------------
//                            SigHandlersList.h
//------------------------------------------------------------------------------

#ifndef _SigHandlersList_h
#define _SigHandlersList_h

#include <signal.h>
#include <errno.h>
#include <sys/time.h>	
#include <sys/types.h>

#include "Prelude.h"
#include "SigHandler.h"

#include <set>
using std::set;

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr  信号处理模块(libsr)  
@{ 
*/


/*! \file SigHandlersList.h 
  \brief SigHandlersList是一个为监听信号而维护信号和事件处理器集(EventHandlers)影像的单件类.
*/

/*! \class CFUNC_Handler  SigHandlersList.h
    CFUNC_Handler是一个信号处理函数(signal handler function)的包裹类.
	 向外界表现出它是一个EventHandler.
*/
class CFUNC_Handler : public EventHandler
{
public:
	CFUNC_Handler (C_SIG_HANDLER csigh_);

	int           handle_signal (int signum_);
	C_SIG_HANDLER handler       () { return m_c_sig_hand; }
	
private:
	C_SIG_HANDLER m_c_sig_hand;
};

/** SigHandlersList class.


	SigHandlersList class is used by SigHandlers class to keep track of
	EventHandlers installed to be called on signal's delivery. It is sort of
	global process map of signal numbers into corresponding sets of 
	EventHandlers that are listening for signal delivery.
*/
class SigHandlersList 
{
public:
	typedef EventHandler* key_type;
	typedef EventHandler* data_type;

	struct CompSHL {
		bool operator () (const key_type c1_, const key_type c2_) const
		{
			return (unsigned long)(c1_) < (unsigned long)(c2_);
		}
	};

	typedef set< key_type, CompSHL > set_t;
	typedef set< key_type, CompSHL >::iterator iterator;

	/** Retrieve a pointer to the list of event handlers 
	    listening to signum_ signal delivery.
	*/
	static SigHandlersList* instance (int signum_);

	/// Destructor
	~SigHandlersList ();

	/// Is list empty
	bool empty () const;

	/// Size of the list
	size_t size () const;

	/** Add an event handler data_ to the list.
	    @return TRUE on success, FALSE on error.
	*/
	bool insert (data_type data_);

	/** Find and remove event handler  key_  from the list.
	 */
	void erase (const key_type key_);

	/** Remove an event handler pointed by iterator  it_ 
	    from the list.
	*/
	void erase (iterator it_);
	
	/** Empty event handlers' list.
	 */
	void erase ();
	
	/** Return an iterator pointing to the beginning of the list.
	 */
	iterator begin ();
	
	/** Return an iterator pointing to the end of the list.
	 */
	iterator end ();

	/** Find event handler by its pointer key_.
	    @return Iterator to the element.
	*/
	iterator find (const key_type key_);

	/** Save 3rd party C function handler to remember.
	    @param cfp_ New 3rd party C function handler. If it is NULL,
	    then seen_cfh flag is set to FALSE.
	    @return old 3rd party C function handler.
	*/
	CFUNC_Handler* cfunc_handler (CFUNC_Handler* cfp_);

	/** Retrieve pointer to 3rd party C function handler.
	    @return 3rd party C function handler.
	*/
	CFUNC_Handler* cfunc_handler () const;

	/** Indicate whether 3rd party C function handler was installed.
	    @param ft_ TRUE if was, FALSE if not.
	*/
	void seen_cfunc_handler (bool ft_);

	/** @return TRUE if we've seen 3rd party C function handler;
	    FALSE otherwise.
	*/
	bool seen_cfunc_handler () const;

protected:
	SigHandlersList ();		// Singleton
	SigHandlersList (const SigHandlersList& map_); // prohibit copying
	SigHandlersList& operator= (const SigHandlersList& map_);
	
public:
	/** Static map of signal numbers to SigHandlerLists. 
	 */
	static SigHandlersList* m_instance[NSIG];

private:
	/// Set of all event handlers registered for this signal.
	set_t*         m_set;

	/** If true this flag indicates that 3rd party event handler
	    has already been installed prior taking control by SigHandlers
	    manager.
	*/
	int            m_seen_cfh;  

	/** Pointer to the 3rd party signal handler in the set
	*/
	CFUNC_Handler*  m_cfhp;
};

//-------------------------------------------------------------------------
//----------------------- SigHandlersList Inlines -------------------------
//-------------------------------------------------------------------------

inline
SigHandlersList::
SigHandlersList ()
	: m_seen_cfh (false), m_cfhp (NULL)
{
	trace_with_mask("SigHandlersList::SigHandlersList", SIGHAND);

	m_set = new set_t;
}

inline
SigHandlersList::
~SigHandlersList ()
{
	trace_with_mask("SigHandlersList::~SigHandlersList", SIGHAND);

	erase ();
	delete m_set;
	m_set = NULL;
}

inline  SigHandlersList*
SigHandlersList::
instance (int signum_)
{
	trace_with_mask("SigHandlersList::instance", SIGHAND);

	DL((APP, "m_instance[%d] = 0x%x\n", signum_,
	    SigHandlersList::m_instance[signum_]));

	if (SigHandlersList::m_instance[signum_] == 0) {
		DL((APP, "new SigHandlersList allocated\n"));
		SigHandlersList::m_instance[signum_] = new SigHandlersList();
	}
	return SigHandlersList::m_instance[signum_];
}

inline bool
SigHandlersList::
empty () const
{
	trace_with_mask("SigHandlersList::empty", SIGHAND);

	// true if map is empty, false otherwise

	return m_set->empty ();
}

inline size_t
SigHandlersList::
size () const
{
	trace_with_mask("SigHandlersList::size", SIGHAND);

	// return number of elements in the map

	return m_set->size ();
}

inline bool
SigHandlersList::
insert (data_type eh_)
{
	trace_with_mask("SigHandlersList::insert", SIGHAND);

	/*---
	  Insert 'eh_' into the set. set::insert() returns a 'pair' object.

	  If the set doesn't contain an element that matches 'eh_', insert a 
	  copy of 'eh_' and returns a 'pair' whose first element is an
	  iterator positioned at the new element and second element is
	  'true'.

	  If the set already contains an element that matches 'eh_', returns
	  a pair whose first element is an iterator positioned at the
	  existing element and second element is false!
	  ---*/

	set_t::const_iterator it = m_set->find (eh_);

	/*--- Not in the set ---*/
	if (it == m_set->end ()) { 
		return (m_set->insert (eh_)).second;
	}
	/*--- Already in the set ---*/
	return true;
}

inline void
SigHandlersList::
erase (const key_type key_)
{
	// return number of erased elements
	trace_with_mask("SigHandlersList::erase(key_)", SIGHAND);
	
	m_set->erase (key_);
}

inline void
SigHandlersList::
erase ()
{
	// empty the map
	trace_with_mask("SigHandlersList::erase(void)", SIGHAND);

	m_set->erase (m_set->begin(), m_set->end());
}

inline void
SigHandlersList::
erase(iterator it_)
{
	// erase element pointed by iterator
	trace_with_mask("SigHandlersList::erase(it_)", SIGHAND);

	m_set->erase(it_);
}

inline SigHandlersList::iterator
SigHandlersList::
begin ()
{
	trace_with_mask("SigHandlersList::begin()", SIGHAND);

	return m_set->begin ();
}

inline SigHandlersList::iterator
SigHandlersList::
end ()
{
	trace_with_mask("SigHandlersList::end", SIGHAND);

	return m_set->end ();
}

inline SigHandlersList::iterator
SigHandlersList::
find (const key_type key_)
{
	trace_with_mask("SigHandlersList::find", SIGHAND);

	return m_set->find (key_);
}


inline CFUNC_Handler*
SigHandlersList::
cfunc_handler (CFUNC_Handler* cfhp_)
{
	trace_with_mask("SigHandlersList::cfunc_handler", SIGHAND);
	
	CFUNC_Handler* old_cfhp = m_cfhp;
	m_cfhp = cfhp_;
	m_seen_cfh = cfhp_ == NULL ? false : true;
	return old_cfhp;
}

inline CFUNC_Handler*
SigHandlersList::
cfunc_handler () const
{
	trace_with_mask("SigHandlersList::cfunc_handler", SIGHAND);
	
	return m_cfhp;
}

inline void
SigHandlersList::
seen_cfunc_handler (bool ft_) 
{
	trace_with_mask("SigHandlersList::seen_cfunc_handler", SIGHAND);

	m_seen_cfh = ft_;
}

inline bool
SigHandlersList::
seen_cfunc_handler () const
{
	trace_with_mask("SigHandlersList::seen_cfunc_handler", SIGHAND);

	return m_seen_cfh;
}

//-------------------------------------------------------------------------
//------------------------ CFUNC_Handler Inlines --------------------------
//-------------------------------------------------------------------------

inline
CFUNC_Handler::
CFUNC_Handler (C_SIG_HANDLER csigh_)
	: m_c_sig_hand (csigh_)
{
	trace_with_mask("CFUNC_Handler::CFUNC_Handler", SIGHAND);
}

inline int
CFUNC_Handler::
handle_signal (int signum_)
{
	trace_with_mask("CFUNC_Handler::handle_signal", SIGHAND);

	if (m_c_sig_hand) {
		(*m_c_sig_hand)(signum_);
	}
	return 1;
}

/** @} */

END_SUNRISE_NAMESPACE

#endif /* _SigHandlersList_h */

