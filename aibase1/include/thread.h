

#ifndef __SR_THREAD_H__
#define __SR_THREAD_H__

#include <impl/callback.h>
#include <pthread.h>
#include <stdexcept>
#include <cstring>
#include <string>

/** \addtogroup libsr 基础工具库(libsr)
@{ 
*/

/** @file thread.h
 * \brief SRThread::Thread模板类
**/


/*! \example thread.cpp
 * \brief 线程使用范例
 */

namespace SRThread {

/**
 * T\brief hread模板类使用户定义的类能够运行在一个线程里面. 模板类的构造函数创建一个
 * 新的线程,并用用户定义的类的operator()()方法作为新线程的entrypoint.
 *
 * 在缺省的情况下, 新的线程是分离的,也就是说创建线程并不等待这个新线程的返回.
 * , 可以选择创建合并的线程, 这时候可以通过调用join()方法使主线程等待新线程的
 *　返回.
 *
 * 当operator()()方法返回时, 线程结束.
 *
 * @param T 用户定义的类, 这个类必须定义缺省构造函数和operator()()方法.
**/
template <typename T> 
class Thread {
private:
    Thread (const Thread&);
    Thread& operator= (const Thread&);

    void init (void);

public:
    //####################################################################
    /** 
     * \brief Thread模板类构造函数调用T::T()创建一个新的T实例, 开启一个新的线程,并调用
	 * T的operator()()方法, 当这个方法返回, 线程则告结束,并被销毁.
     *
     * @param detached 如果为true,则创建一个分离式的线程. 你不能用join()方法.
    **/
    //####################################################################
    explicit Thread (bool detached=true);

    //####################################################################
    /** 
     * \brief Thread模板类构造函数调用T::T(const T&)创建一个新的T实例, 开启一个新的线程,并调用
	 * T的operator()()方法, 当这个方法返回, 线程则告结束,并被销毁.
     *
     * @param detached 如果为true,则创建一个分离式的线程. 你不能用join()方法.
    **/
    //####################################################################
    explicit Thread (T &t, bool detached=true);

    //####################################################################
    /** 
     * \brief Thread模板类构造函数调用T::T(P&)创建一个新的T实例, 开启一个新的线程,并调用
	 * T的operator()()方法, 当这个方法返回, 线程则告结束,并被销毁.
     *
	 * @param p 用户定义的类构造函数使用的参数．
     * @param detached 如果为true,则创建一个分离式的线程. 你不能用join()方法.
    **/
    //####################################################################
    template <typename P> 
	explicit Thread (P &p, bool detached=true);

    //####################################################################
    /** 
     * \brief Thread模板类构造函数调用T::T(P&)创建一个新的T实例, 开启一个新的线程,并调用
	 * T的operator()()方法, 当这个方法返回, 线程则告结束,并被销毁.
     *
	 * @param p1 用户定义的类构造函数使用的参数1．
	 * @param p2 用户定义的类构造函数使用的参数2．
     * @param detached 如果为true,则创建一个分离式的线程. 你不能用join()方法.
    **/
    //####################################################################
    template <typename P1, typename P2> 
	explicit Thread (P1 &p1, P2 &p2, bool detached=true);

    //####################################################################
    /** 
     * \brief 析构函数
	 * 在线程还在运行的时候，这个函数不会被调用，所以不要在线程仍在运行的时候
	 * 让Thread实例走出它的生存区.
     *
    **/
    //####################################################################
    ~Thread (void);

    //####################################################################
    /** 
     * \brief 调用这个方法将使主线程等待新线程退出.
	 *
     * @return 如果等待线程结束成功返回true, 否则返回false.
    **/
    //####################################################################
    bool join (void);

private:
    T t_;

    struct Callback : CallbackWrapper {
	Callback (T &t) : t_(t) { }
	void operator() (void) { t_(); }
	T &t_;
    } callback_;

    bool detached_;
    bool joined_;
    pthread_t thread_;

}; 
/** @} */

#include <impl/thread.impl>

} 
#endif
