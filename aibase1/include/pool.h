

/** @file
 * �̳߳���.
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
 * �̳߳ط���
 */

namespace SRThread {

/**
 * �̳߳�ģ�������ʹ���߿���һ���߳�,ʹ��dispatch()�������ɹ������ȴ����߳�.
 * �ڹ��캯���Ĳ�����ָ�����ڵ��߳���, ���캯������ȷ�����ݵ��߳�, ��֮�󲻻�
 * ���µ��̱߳�����.
 *
 *
 * @param T_thread_type �û��������, ��Ҫ��ȱʡ���캯��������һ�������Ĺ��캯��,
 *                      �������ṩoperator()(T_dispatch_type)����.
 *
 * @param T_dispatch_type �û��������,���ڴ���ڶ�������,��Ϊ�������ɸ��߳�, ����
 *                        ����Է���һ��socket�����ָ��ȴ��������߳�.
**/
template <typename T_thread_type, typename T_dispatch_type> 
class Pool {
public:
    //####################################################################
    /** 
     * ���캯��.
	 * ����ȷ����Ŀ���߳�, ���ڵ���Щ�̵߳ȴ�������.
     *
     * @param pool_size �������̵߳���Ŀ
    **/
    //####################################################################
    explicit Pool (int pool_size=10);

    //####################################################################
    /** 
     * ���캯��.
	 * ʹ��T_ctor_typeΪ��������ȷ����Ŀ���߳�, ���ڵ���Щ�̵߳ȴ�������.
	 * 
     *
     * @param c Ϊ�����߳�,�ڵ��ù��캯��ʱ�Ĳ���.
     * @param pool_size �������̵߳���Ŀ.
    **/
    //####################################################################
    template <typename T_ctor_type> 
	explicit Pool (T_ctor_type &c, int pool_size=10);

    //####################################################################
    /** 
     * ��������
	 *�����г��ڵ��߳̽��ᱻ���٣�
     *
     * @author Peter Jones
    **/
    //####################################################################
    ~Pool (void);

    //####################################################################
    /** 
     * ���ɹ��������ڵȴ�״̬���߳�, �����������������
     *
     * @param t ���ɸ��߳�operator()(T_Dispatch_type& t)�Ĳ�����
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
