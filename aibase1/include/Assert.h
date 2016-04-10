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

/** \defgroup libsr �������߿�(libsr) 
 *  \{*/
/** \file Assert.h
 *	 \brief ���Ժ궨��
 */
/** \}*/


/** \def Assert_exit(exp_)
 *	 \brief ������ʽexp_ʧ�ܣ���¼������־������SIGTERM�ź��˳�Ӧ��.
 *	 \param exp_ ���ʽ
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
 *	\brief ���Զ��Բ��ڱ��ʽʧ��ʱ��¼������Ϣ֮�󷵻�false.
 *	\param exp_ ���ʽ
 *	\return ������ʽ��Ϊ��0ֵ�򷵻�false. ������ԭ�������̼���ִ��.
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
