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
    \brief ��־��¼����λ����
*/

/*! \enum Group
    λ����ö�ٶ���.
*/
BEGIN_SUNRISE_NAMESPACE

enum Group {
	TRACE        = 0x00000001, /*!< ��������ջ���� */
	APP          = 0x00000002, /*!< Ӧ�ü����� */
	USR1         = 0x00000004, /*!< �û������Ӧ�ü�����1 */
	USR2         = 0x00000008, /*!< �û������Ӧ�ü�����2 */
	USR3         = 0x00000010, /*!< �û������Ӧ�ü�����3 */
	/*-----------------------*/
	ALL_APPS     = 0x0000001F, /*!< ����Ӧ�ü����� */
	/*-----------------------*/
	ERROR        = 0x00000020, /*!< sunrise��ϵͳ��error��־���� */
	PIDFLOCK     = 0x00000040, /*!< PidFileLock����Ϣ����  */
	CMDLINEOPTS  = 0x00000080, /*!< CmdLineOpts����Ϣ���� */
	SEM          = 0x00000100, /*!< Semaphore����Ϣ����  */
	SIGHAND      = 0x00000200, /*!< SigHandler(s)����Ϣ���� */
	REACT        = 0x00000400, /*!< Reactor/PrioriyQueue����Ϣ����  */
	REACTTRACE   = 0x00000800, /*!< ��չ��Reactor/PrioriyQueue����Ϣ����  */
	SOCK         = 0x00001000, /*!< Socket�༰����Ԫ��Ϣ����  */
	SOCKTRACE    = 0x00002000, /*!< ��չ��Socket������Ԫ��Ϣ����  */
	XDRBUF       = 0x00004000, /*!< xdrIOBuffer����Ϣ���� */
	XDRBUFTRACE  = 0x00008000, /*!< ��չ��xdrIOBuffer������ */
	STRMBUF      = 0x00010000, /*!< Streambuf������Ԫ��Ϣ���� */
	STRMBUFTRACE = 0x00020000, /*!< ��չ��Streambuf������Ԫ��Ϣ���� */
	FORK         = 0x00040000, /*!< Fork����Ϣ���� */
	SIGACT       = 0x00080000, /*!< SigACtion����Ϣ���� */
	PIPE         = 0x00100000, /*!< Pipe����Ϣ���� */
	CHARINBUF    = 0x00200000, /*!< CharInBuffer����Ϣ���� */
	ADDRESS      = 0x00400000, /*!< Address������Ԫ��Ϣ���� */
	RES3         = 0x00800000, /*!< �������� */
	RES4         = 0x01000000, /*!< �������� */
	RES5         = 0x02000000, /*!< �������� */
	RES6         = 0x04000000, /*!< �������� */
	RES7         = 0x08000000, /*!< �������� */
	RES8         = 0x10000000, /*!< �������� */
	RES9         = 0x20000000, /*!< �������� */
	RES10        = 0x40000000, /*!< �������� */
	/*-----------------------*/
	ALL_LIB      = 0x7FFFFFE0, /*!< ���еĿ���Ϣ */
	ALL          = 0x7FFFFFFF, /*!< ������Ϣ: �� + Ӧ�� */
	NONE         = 0x00000000  /*!< ȫ����Ϣ����Ҫ */
};


enum marker_t { 
	FUNC_MSG,
	FUNC_ENTRY, 
   FUNC_EXIT 
};

/** @} */
END_SUNRISE_NAMESPACE

#endif /* LOG_MASK_H */
