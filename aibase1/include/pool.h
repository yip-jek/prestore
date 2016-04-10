

/** @file
 * 线程池类.
**/

#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <thread.h>
#include <mutex.h>
#include <condition.h>
#include <utility.h>

#include <vector>
#include <deque>
#include <queue>
#include <list>
#include <ctime>

/*! \example int_pool.cpp
 * 线程池范例
 */

namespace SRThread {

/**
 * 线程池模板类可让使用者开启一组线程,使用dispatch()方法分派工作给等待的线程.
 * 在构造函数的参数里指定池内的线程数, 构造函数创建确定数据的线程, 这之后不会
 * 有新的线程被创建.
 *
 *
 * @param T_thread_type 用户定义的类, 需要有缺省构造函数或者有一个参数的构造函数,
 *                      还必须提供operator()(T_dispatch_type)方法.
 *
 * @param T_dispatch_type 用户定义的类,用于存放在队列里面,作为参数分派给线程, 比如
 *                        你可以分派一个socket描述字给等待工作的线程.
**/
template <typename T_thread_type, typename T_dispatch_type> 
class Pool {
public:
    //####################################################################
    /** 
     * 构造函数.
	 * 创建确定数目的线程, 池内的这些线程等待被分派.
     *
     * @param pool_size 创建的线程的数目
    **/
    //####################################################################
    explicit Pool (int pool_size=10);

    //####################################################################
    /** 
     * 构造函数.
	 * 使用T_ctor_type为参数创建确定数目的线程, 池内的这些线程等待被分派.
	 * 
     *
     * @param c 为创建线程,在调用构造函数时的参数.
     * @param pool_size 创建的线程的数目.
    **/
    //####################################################################
    template <typename T_ctor_type> 
	explicit Pool (T_ctor_type &c, int pool_size=10);

    //####################################################################
    /** 
     * 析构函数
	 *　所有池内的线程将会被销毁．
     *
     * @author Peter Jones
    **/
    //####################################################################
    ~Pool (void);

    //####################################################################
    /** 
     * 分派工作给处于等待状态的线程, 这个方法不会阻塞．
     *
     * @param t 分派给线程operator()(T_Dispatch_type& t)的参数．
    **/
    //####################################################################
    void dispatch (const T_dispatch_type &t);

public:

    typedef typename std::queue<
	T_dispatch_type, 
	typename std::deque<T_dispatch_type> 
	> queue_type;

    struct queinfo_type {
	volatile bool quit_;
	volatile int pool_size_;

	Mutex mutex_;
	Condition cond_;
	queue_type que_;

	Mutex ids_mutex_;
	std::list<pthread_t> ids_;
    };

private:
    struct ThreadWrapper {
	ThreadWrapper (queinfo_type &qi);
	template <typename P> explicit ThreadWrapper (queinfo_type &qi, P &p);
	void operator() (void);

	queinfo_type &qi_;
	T_thread_type t_;
    };

    typedef Thread<ThreadWrapper> thread_type;

    queinfo_type qi_;
    typename std::vector<thread_type*> threads_;

    Pool (const Pool&);
    Pool& operator= (const Pool&);
}; 

#include <impl/pool.impl>

}
#endif
