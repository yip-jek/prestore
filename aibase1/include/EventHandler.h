#ifndef _EventHandler_h
#define _EventHandler_h

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif

#include "Assert.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file EventHandler.h 
    \brief ����I/O�¼�,ʱ���¼��ȵȵĳ���ӿ�
*/

/*! \var unsigned long TimerId.
	����handle_timeout()����. 
*/
typedef unsigned long TimerId;

/*! \enum EventType
 �¼����Ͷ���
*/
enum EventType
{	
	READ_EVENT    = 0x01,	/*!< һ��IO��ͨ�����ڷ�������״̬�¶�������1bytes
									  ʱ֪ͨ���ķ������� */
	WRITE_EVENT   = 0x02,	/*!< һ��IOдͨ�����ڷ�������״̬��д������1bytesʱ֪ͨ
									  ���ķ�����. */
	EXCEPT_EVENT  = 0x04,	/*!< һ����TCP���⵽�쳣���ʱ֪ͨ���ķ����� */
	TIMEOUT_EVENT = 0x10,	/*!< ��ʱ�¼�����ʱ֪ͨ���ķ�����. */
	SIGNAL_EVENT  = 0x20,	/*!< һ��OS�����ź�(signal)��Ӧ�ó���ʱ֪ͨ���ķ�����. */
	RWE_EVENTS    = 0x07,	/*!< READ_EVENT | WRITE_EVENT | EXCEPT_EVENT   */
	ALL_EVENTS    = 0x37	/*!< �������������¼� */
};

bool isReadEvent (EventType e_) ;
bool isWriteEvent (EventType e_) ;
bool isExceptEvent (EventType e_) ;
bool isTimeoutEvent (EventType e_) ;
bool isSignalEvent (EventType e_) ;
bool isRWEEvents (EventType e_) ;
bool isAllEvents (EventType e_) ;

/*! \class EventHandler EventHandler.h "include/EventHandler.h"
 * EventHandler��һ�������.
 * �����¼������� - ���¼������Ǳ����ķ������ص� - �Ľӿ�. ��Щ�¼�����
 * �źţ�ʱ�䣬I/O�ܵ����ļ������֣�Sockets�ȵȡ�
 * \see EventType
*/
class EventHandler
{
public:
	/*! \brief ȱʡ���캯��
	 */
	EventHandler();

	/*! \brief ���¼��Ļص�����. ������EOF�����践��-1
	    @return �ɹ�����0,���򷵻�-1.
	*/
	virtual int handle_read (int fd);

	/*! \brief ���¼��Ļص�����.
	    @return �ɹ�����0,���򷵻�-1.
	*/
	virtual int handle_write (int fd);

	/*! \brief �쳣�¼��Ļص�����
	    @return �ɹ�����0,���򷵻�-1.
	*/
	virtual int handle_except (int fd);

	/*! \brief ��ʱ�¼��Ļص�����.
		����ֵ�����ԣ�����´λ�Ҫ����ʱ�¼��������ڷ���ǰ���°�װ�¼�������.
	    @return �ɹ�����0,���򷵻�-1.
	*/
	virtual int handle_timeout (TimerId tid);

	/*! \brief �źŴ���Ļص�����
	    @return �ɹ�����0,���򷵻�-1.
	*/
	virtual int handle_signal(int signum_); 

	/*! \brief �Է��׽��ֵ���EOFʱ�Ļص�����.
	    û����Ӧ���¼����ͣ�����ص���������Ҫ�����handle_read()�Ѿ�������
		 EOF�����ʱ��
	    @return �ɹ�����0,���򷵻�-1.
	*/
	virtual int handle_close (int fd);

	/*! \brief ���������ڲ�״̬�Ĺ��Ӻ���
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
