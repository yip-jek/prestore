

#ifndef __MUTEX_H__
#define __MUTEX_H__

// system includes
#include <pthread.h>

namespace SRThread {

/** \addtogroup libsr 基础工具库(libsr) 
@{ 
*/

/** @file mutex.h
 * \brief 互斥量包裹类.
**/


/**
 * \brief POSIX Thread互斥量包裹类．互斥量确保同一时间内只有一个线程可以访问资源．
 *
**/
class Mutex {
    friend class Lock;
public:
    //####################################################################
    /** 
     * \brief 构造函数
     *
    **/
    //####################################################################
    Mutex (void);

    //####################################################################
    /** 
     * \brief 析构函数
     *
    **/
    //####################################################################
    ~Mutex (void);

    pthread_mutex_t& mutex(void);

    /**
     * \brief 内嵌类Mutex::Lock，确保给定的互斥量被锁定（在构造的时候）并在适当的
	 * 时候释放(析构)
     */
    class Lock {
	friend class Condition;

    public:
	//####################################################################
	/** 
	 * \brief 锁定给定的Mutex对象．
	 *
	 * @param m Mutex实例．
	**/
	//####################################################################
	explicit Lock (Mutex &m);

	//####################################################################
	/** 
	 * \brief 析构函数，如果所管理的mutex对象被锁，释放之．
	**/
	//####################################################################
	~Lock (void);

	//####################################################################
	/** 
	 * \brief 上锁，如果当前线程已经拥有这个锁，将导致抛出异常．
	 *
	 * @author Peter Jones
	**/
	//####################################################################
	void unlock (void);

	//####################################################################
	/** 
	 * \brief 上锁，如果当前线程已经释放了这个锁，将导致抛出异常．
	 *
	**/
	//####################################################################
	void lock (void);


	private:
	Lock (const Lock&);
	Lock& operator= (const Lock&);

    private:
	pthread_mutex_t &mutex_;
	volatile bool locked_;

    };

private:
    Mutex (const Mutex&);
    Mutex& operator= (const Mutex&);

private:
    pthread_mutex_t mutex_;

}; 

} 
/** @} */

#endif
