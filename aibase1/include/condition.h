
#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <mutex.h>
#include <pthread.h>

/** \defgroup libsr 基础工具库(libsr) 
@{ 
*/


/** @file condition.h
 * \brief 条件量包裹类.
**/


/*! \example condition.cpp
 * 条件量范例
 */

namespace SRThread {

/**  
 * \brief POSIX Thread的条件量. 条件量使多个线程等待某个条件变为true．另一个线程通过
 * 信号唤醒等待线程中的某个线程，或者通过广播唤醒所有的等待线程．
 *
 * 在调用wait()方法前，线程必须先要获取共享互斥量的锁．wait()将对这个互斥量
 * 解锁
 *
 * boardcase()和signal()方法的调用也必须先获得这个互斥量的锁．
**/
class Condition {
private:
    Condition (const Condition&);
    Condition& operator= (const Condition&);

public:
    //####################################################################
    /** 
     * \brief 构造函数
    **/
    //####################################################################
    Condition (void);

    //####################################################################
    /** 
     * \brief 析构函数
    **/
    //####################################################################
    ~Condition (void);

    //####################################################################
    /** 
     * \brief 通过信号通知条件，结果是唤醒了某个等在在这个条件量上的线程．如果没有
	 * 任何的线程等待这个条件量，则什么都没有做．
     *
     * @param l 互斥锁实例
    **/
    //####################################################################
    void signal (Mutex::Lock &l);

    //####################################################################
    /** 
     * \brief 广播一个条件，结果是唤醒所有等待在这个条件量上的所有线程．如果没有
	 * 任何的线程等待这个条件量，则什么都没有做．
     *
     * @param l A mutex lock for the mutex that is shared with all the threads.
    **/
    //####################################################################
    void broadcast (Mutex::Lock &l);

    //####################################################################
    /** 
     * \brief 调用线程将被阻塞直到另一个线程调用了signal()或者broadcast(). 当调用这个
	 * 方法返回时最好要再检查条件量是否真为true. 如果不为true, 则应该再次调用
	 * wait().
     *
     * @param l 互斥锁实例
    **/
    //####################################################################
    void wait (Mutex::Lock &l);

private:
    /** \brief 条件变量 */
    pthread_cond_t cond_;
};

}
/** @} */
#endif
