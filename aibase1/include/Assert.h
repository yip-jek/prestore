#ifndef ASSERT_H
#define ASSERT_H

#include <unistd.h>
#include <errno.h>              /* errno                   */
#include <signal.h>             /* raise                   */

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif
#include "Logger.h"

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr 基础工具库(libsr) 
 *  \{*/
/** \file Assert.h
 *	 \brief 断言宏定义
 */
/** \}*/


/** \def Assert_exit(exp_)
 *	 \brief 如果表达式exp_失败，记录错误日志，发出SIGTERM信号退出应用.
 *	 \param exp_ 表达式
 */
#define Assert_exit( exp_ ) \
    do { \
        if ( !(exp_) ) { \
	       DL((ERROR,"Assert Aborted False Expression!\n")); \
	       DL((ERROR,"Error on line %d in file %s\n", __LINE__, __FILE__)); \
	       ::raise( SIGTERM ); \
        } \
    } while (0)


/** \def Assert_return(exp_)
 *	\brief 测试断言并在表达式失败时记录错误信息之后返回false.
 *	\param exp_ 表达式
 *	\return 如果表达式不为非0值则返回false. 否则按照原来的流程继续执行.
 */
#define Assert_return(exp_) \
    do { \
        if ( !(exp_) ) { \
	        DL((ERROR,"Assert Returned False Expression!\n")); \
            DL((ERROR,"Error on line %d in file %s\n", __LINE__, __FILE__)); \
	        return (false); \
        } \
    } while (0) 

END_SUNRISE_NAMESPACE

#endif /* ASSERT_H */  
