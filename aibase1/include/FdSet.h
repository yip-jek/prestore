#ifndef FDSET_H
#define FDSET_H

#include <string.h>
#include <sys/time.h>		/* select(3) */
#if defined(Linux)		/* select(3) */
#    include <sys/types.h>
#    include <unistd.h>
#endif

#include "Prelude.h"
#include "Logger.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file FdSet.h
 fd_set�ṹ�������
*/

/*! \class FdSet FdSet.h
 * \brief fd_set�ṹ�������
 */
class FdSet : public fd_set
{
public:
	/*! \brief ���캯��. 
	 */
	FdSet ();

	/*! \brief ��������.
    */
	~FdSet () {}

	/*! \brief ���ò���fd_��λΪON״̬
	    @param  fd_ Bit to set.
	    @return false if argument is out of bounds, true otherwise.
	 */
	bool setFd (const unsigned int fd_);

	/*! \brief �������fd_��λΪOFF״̬
	    @param fd_ Bit to clear
	    @return false if argument is out of bounds; true otherwise.
	 */
	bool clear (const unsigned int fd_);

	/*! \brief ������fd_�ı�־λ״̬�Ƿ�ΪON.
	    @param fd_ Bit to test
	    @return true if fd_ bit is set; false otherwise
	 */
	bool isSet (const unsigned int fd_);

	/*! \brief ����fd�������е�λΪOFF.
	 */
	void reset (void);

	/*! \brief ���fd���б���ΪON����Ŀ
	    @return Number of bits set
	 */
	int numSet ();

	/*! \brief ��λ����Ϣд����־.
	 */
	void dump (void);
};

inline
FdSet::
FdSet () 
{
	reset (); 
}

inline bool
FdSet::
setFd (const unsigned int fd_) 
{ 
	if ( fd_ <= FD_SETSIZE ) {
		FD_SET (fd_, this); 
		return true;
	}
	return false;
}

inline bool 
FdSet::
clear (const unsigned int fd_) 
{
	if ( fd_ <= FD_SETSIZE ) {
		FD_CLR (fd_, this); 
		return true;
	}
	return false;
}

inline bool 
FdSet::
isSet (const unsigned int fd_) 
{ 
	return FD_ISSET (fd_, this); 
}

inline void 
FdSet::
reset (void) 
{ 
	::memset(this, 0, sizeof (*this)); 
}

inline int 
FdSet::
numSet () 
{
	register int i, n;

	for (i=0, n=0; i < FD_SETSIZE; i++) {
		if ( isSet (i) ) {
			n++;
		}
	}
	return n;
}

inline void 
FdSet::
dump (void) 
{
	for (int i=0; i< FD_SETSIZE; i++) {

		if ( isSet (i) ) {
			DL((TRACE,"fd # %d\n",i));
		}
	}
}

END_SUNRISE_NAMESPACE

#endif /* FDSET_H */  
