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
 fd_set结构体包裹类
*/

/*! \class FdSet FdSet.h
 * \brief fd_set结构体包裹类
 */
class FdSet : public fd_set
{
public:
	/*! \brief 构造函数. 
	 */
	FdSet ();

	/*! \brief 析构函数.
    */
	~FdSet () {}

	/*! \brief 设置参数fd_的位为ON状态
	    @param  fd_ Bit to set.
	    @return false if argument is out of bounds, true otherwise.
	 */
	bool setFd (const unsigned int fd_);

	/*! \brief 清除参数fd_的位为OFF状态
	    @param fd_ Bit to clear
	    @return false if argument is out of bounds; true otherwise.
	 */
	bool clear (const unsigned int fd_);

	/*! \brief 检测参数fd_的标志位状态是否为ON.
	    @param fd_ Bit to test
	    @return true if fd_ bit is set; false otherwise
	 */
	bool isSet (const unsigned int fd_);

	/*! \brief 重置fd集中所有的位为OFF.
	 */
	void reset (void);

	/*! \brief 检测fd集中被设为ON的数目
	    @return Number of bits set
	 */
	int numSet ();

	/*! \brief 把位集信息写入日志.
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
