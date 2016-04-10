
/** @file
 * 线程需要用的工具函数
**/

#ifndef __PTHREAD_UTILITY_H__
#define __PTHREAD_UTILITY_H__

namespace SRThread {

    //####################################################################
    /** 
	 * 使线程暂停一段确定的时间.
	 *
     * @param sec 暂停秒数.
     * @param usec 暂停微秒数.
    **/
    //####################################################################
    void micro_sleep (unsigned long sec, unsigned long usec=0);
    
}
#endif
