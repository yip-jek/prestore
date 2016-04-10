//------------------------------------------------------------------------------
//                              LogMask.h
//------------------------------------------------------------------------------
#ifndef LOG_MASK_H
#define LOG_MASK_H

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif

/** @addtogroup libsr 
@{
*/

/*! \file LogMask.h
    \brief 日志记录器的位掩码
*/

/*! \enum Group
    位掩码枚举定义.
*/
BEGIN_SUNRISE_NAMESPACE

enum Group {
	TRACE        = 0x00000001, /*!< 函数调用栈掩码 */
	APP          = 0x00000002, /*!< 应用级掩码 */
	USR1         = 0x00000004, /*!< 用户定义的应用级掩码1 */
	USR2         = 0x00000008, /*!< 用户定义的应用级掩码2 */
	USR3         = 0x00000010, /*!< 用户定义的应用级掩码3 */
	/*-----------------------*/
	ALL_APPS     = 0x0000001F, /*!< 所有应用级掩码 */
	/*-----------------------*/
	ERROR        = 0x00000020, /*!< sunrise和系统级error日志掩码 */
	PIDFLOCK     = 0x00000040, /*!< PidFileLock类信息掩码  */
	CMDLINEOPTS  = 0x00000080, /*!< CmdLineOpts类信息掩码 */
	SEM          = 0x00000100, /*!< Semaphore类信息掩码  */
	SIGHAND      = 0x00000200, /*!< SigHandler(s)类信息掩码 */
	REACT        = 0x00000400, /*!< Reactor/PrioriyQueue类信息掩码  */
	REACTTRACE   = 0x00000800, /*!< 扩展的Reactor/PrioriyQueue类信息掩码  */
	SOCK         = 0x00001000, /*!< Socket类及其友元信息掩码  */
	SOCKTRACE    = 0x00002000, /*!< 扩展的Socket及其友元信息掩码  */
	XDRBUF       = 0x00004000, /*!< xdrIOBuffer类信息掩码 */
	XDRBUFTRACE  = 0x00008000, /*!< 扩展的xdrIOBuffer类掩码 */
	STRMBUF      = 0x00010000, /*!< Streambuf及其友元信息掩码 */
	STRMBUFTRACE = 0x00020000, /*!< 扩展的Streambuf及其友元信息掩码 */
	FORK         = 0x00040000, /*!< Fork类信息掩码 */
	SIGACT       = 0x00080000, /*!< SigACtion类信息掩码 */
	PIPE         = 0x00100000, /*!< Pipe类信息掩码 */
	CHARINBUF    = 0x00200000, /*!< CharInBuffer类信息掩码 */
	ADDRESS      = 0x00400000, /*!< Address及其友元信息掩码 */
	RES3         = 0x00800000, /*!< 保留掩码 */
	RES4         = 0x01000000, /*!< 保留掩码 */
	RES5         = 0x02000000, /*!< 保留掩码 */
	RES6         = 0x04000000, /*!< 保留掩码 */
	RES7         = 0x08000000, /*!< 保留掩码 */
	RES8         = 0x10000000, /*!< 保留掩码 */
	RES9         = 0x20000000, /*!< 保留掩码 */
	RES10        = 0x40000000, /*!< 保留掩码 */
	/*-----------------------*/
	ALL_LIB      = 0x7FFFFFE0, /*!< 所有的库信息 */
	ALL          = 0x7FFFFFFF, /*!< 所有信息: 库 + 应用 */
	NONE         = 0x00000000  /*!< 全部信息都不要 */
};


enum marker_t { 
	FUNC_MSG,
	FUNC_ENTRY, 
   FUNC_EXIT 
};

/** @} */
END_SUNRISE_NAMESPACE

#endif /* LOG_MASK_H */
