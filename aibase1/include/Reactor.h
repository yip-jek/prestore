//------------------------------------------------------------------------------
//                             Reactor.h
//------------------------------------------------------------------------------
#ifndef REACTOR_H
#define REACTOR_H

#include <sys/time.h>			// select(2)
#include <sys/resource.h>		// getrlimit(2)

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "EventHandler.h"

#ifdef sun
#ifndef USE_SIMPLE_SINGLETON
#define USE_SIMPLE_SINGLETON 1
#endif
#endif

#ifdef USE_SIMPLE_SINGLETON
#include "SSingleton.h"
#else
#include "Singleton.h"
#endif

#include "MaskSet.h"
#include "TimerQueue.h"
#include "TimerCountdown.h"

BEGIN_SUNRISE_NAMESPACE

/*! \example reactor_test.cpp
 * ���ķ��������Գ���
 */

/*! \file Reactor.h 
	Reactorģʽ(��Ӧ��ģʽ, ���ķ�����ģʽ)��ʵ��.

	���ķ��������ļ������ֶ�д���ݣ������ļ������ֵ�I/O�쳣, ����ʱ������¼�.
һ���û������ķ�����ע����ĳ�����ֵ��¼�������(EventHandler)�����ķ������ͻ���
������I/O��д�¼�����ʱ�ص���ע����¼�������. ʱ������¼��������ͬ������
Reactor::waitForEvents(TimeVal* tv_)��һ��ʱ�䵽�����ķ��������ص�ʱ���¼���
����.
*/
/*! \class Reactor Reactor.h
 * \brief ���ķ�����
 */
class Reactor 
{
public:
	/*! \brief ���캯��
	 */
	Reactor ();

	/*! \brief ��������
	 */
	~Reactor();

	/*! \brief �����ķ�����ע��ʱ�䴦����. 
	 *  ע���һ��ʱ�䵽�����ķ��������ص���Ӧ�Ĵ������Ļص�����.

	    @param eh_ [RW] Pointer to the EventHandler
	    @param tv_ [RO] Timeout value
	    @return Timer id that can be used to cancel timer.
	*/
	TimerId registerTimerHandler (EventHandler* eh_, const TimeVal& tv_);

	/*! \brief �����ķ�����ע��I/O�¼�������.
	    ע���һ��I/O�¼����������ķ��������ص���Ӧ�������Ļص�����.

	    @param eh_ Pointer to the EventHandler
	    @param fd_ File descriptor
	    @param et_ Event Type
	    @return true if success, false if error
	 */
	bool registerIOHandler (EventHandler* eh_, int fd_, 
							EventType et_ = RWE_EVENTS);

	/*! \brief �Ƴ��¼�������
	 *  ���et_��TIMEOUT_EVENT,���е�ʱ�䴦�������ᱻ�Ƴ�.

		 @param eh_ Pointer to the EventHandler
	    @param et_ Event Type to remove. Default will remove
	               Event Handler for all events.
	    @return true if success, false if wasn't registered for any events.
	 */
	bool removeHandler (EventHandler* eh_, EventType et_ = ALL_EVENTS);

	/*! \breif �Ƴ�id_�궨��ʱ�䴦����
	    @param id_ Timer Id returned by registerTimer.
	    @return true if timer found and removed; false otherwise
	*/
	bool removeTimerHandler (TimerId id_);

	/*! \brief �Ƴ�fd_�궨��I/O�¼�������
	    @param fd_ File descriptor
	    @return true on success, false if fd_ is out of range
	*/
	bool removeIOHandler (int fd_);

	/*! \brief �¼��ȴ�ѭ�� 
	 */
	void waitForEvents (void);

	/*! \brief ��ָ����ʱ����ڵȴ��¼�����.
	 *  ���tv_ΪNULL, ����Ϊ��waitForEvents(void)һ�������tv_Ϊ{0,0}, 
	 *  ��ʹ���ķ������Է�������ʽ��ѯ�¼�. ������¼����ڷ������¼���
	 *  ���ķ������ص���Ӧ�Ĵ�������������tv_�����Ϊʣ�µ�ʱ�䡣
	 *
	    @param tv_ [RW] is time to wait for. 
	*/
	void waitForEvents (TimeVal* tv_);

	/*! \brief �ر����ķ���������.
	 * ���������ķ�����ע����¼������������Ƴ������ķ���������رպ���
	 * ����.
	*/
	void stopReactor (void);

    /*! \brief �������ķ�����. 
	  * ���ķ���������������¼�����ѭ�����Թ����е��źš�һ�����źŴ���
	  * �����еĴ����е�������������źŴ����غ����ķ������Զ�������.
	  * @see GenServer::handle_signal()
     */
    void deactivate (void);

private:
	Reactor (const Reactor&);	         /// no cloning 
	Reactor& operator= (const Reactor&); /// no cloning

private:
	/*! \brief ����select(2)������ѯ����
	 */
	bool handleError (void);

	/*! \brief �¼�����ʱ֪ͨע���ڱ����ķ��������¼���������
	    @param minimum_ number of file descriptors ready.
	 */
	bool dispatch (int minimum_);

	/*! \brief ���������ּ����Ѿ�׼���ö�(д)���ļ�������.
	 */
	int isAnyReady (void);

	/*! \brief ���Ƿ����ļ�������.
	    @return true if any fd(s) were found and removed; 
	    false otherwise
	*/
	bool checkFDs (void);

	/*! \brief �ص���������������������ظ�ֵ������ķ������Ƴ�����¼�������
	 */
	void dispatchHandler ( FdSet& mask_, EventHandler** fdSet_,
			       EH_IO_Callback callback_);
	
	/*! ��������ĳ�ʱʱ��.
	    @param maxwait_ (in) how long we are expected to wait for event(s).
	    @param howlong_ (out) how long we are going to wait.
	*/
	void calculateTimeout (TimeVal*& howlong_, TimeVal* maxwait_);

	
private:
	/*! ÿ���������ܴ�����ļ���Ŀ(����OS�������в��,��ͨ��ulimit/limit����ȡ��.)
	 */
	int m_noFiles;

	/*! \brief �����ļ�������+1
	 */
	int m_maxfd;

	/*! \brief ��ʶ���ķ������ǻ�Ծ���Ǳ�����ı�־.
	 */
	bool m_active;

	/*! \brief �ȴ�READ_EVENT���¼�������
	 */
	EventHandler** m_readSet;

	/*! \brief �ȴ�WRITE_EVENT���¼�������
	 */
	EventHandler** m_writeSet;

	/*! \brief �ȴ�EXCEPT_EVENT���¼�������
	 */
	EventHandler** m_exceptSet;

	MaskSet m_waitSet;

	MaskSet m_readySet;

	/*! ��ʱ������
	 */
	TimerQueue m_tqueue;
};
/*******************************************************************************
 Inline functions
*******************************************************************************/

inline void Reactor::deactivate (void) {  m_active = false; }

END_SUNRISE_NAMESPACE

#endif /* REACTOR_H */  
