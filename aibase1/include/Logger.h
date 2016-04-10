//------------------------------------------------------------------------------
//                            Logger.h
//------------------------------------------------------------------------------
#ifndef LOGGER_H
#define LOGGER_H

//System headers
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

#include <string>
#include <stack>

using std::string;
using std::stack;

#ifdef sun
#ifndef USE_SIMPLE_SINGLETON
#define USE_SIMPLE_SINGLETON 1
#endif
#endif

//sunrise headers
#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#ifdef USE_SIMPLE_SINGLETON
#include "SSingleton.h"
#else
#include "Singleton.h"
#endif
#include "Logger_Impl.h"
#include "MemDump.h"

BEGIN_SUNRISE_NAMESPACE

#define LOCAL_TIME 0 
#define GMT_TIME 1 

/** @addtogroup libsr 
@{
*/


/*! \example logger_test.cpp
 *  Logger��Ĳ��Գ���
 */

/*! \file Logger.h 
 * \brief ��־��Ϣ�ӿ���
 */
/******************************************************************************
  Class Logger
******************************************************************************/ 
/*! \class Logger
 * \brief ��־��
 */
class Logger
{
public:
    Logger () : m_impl (NULL) { /*empty*/ }
    ~Logger () { this->close (); }

public:
	/*! \brief ����־ö������ĳ��־�����־����.
	 * Group��һ��ö��, ������ּ������־��Ϣ. @see Group
	 *  @param g_ ��ʵ��.
	 */
    void enable_group  (Group g_);

	/*! \brief �ر���־ö������ĳ��־�����־����.
	 * Group��һ��ö��, ������ּ������־��Ϣ. @see Group
	 *	@param g_ ��ʵ��
	 */
    void disable_group (Group g_);

	/*! \brief ����־ö�����ж����־�����־����.
	 *  @param groups_ �����־��İ�λ����
	 */
    void enable_groups  (u_long groups_);

	/*! \brief �ر���־ö�����ж����־�����־����.
	 *  @param groups_ �����־��İ�λ����
	 */
    void disable_groups (u_long groups_);

	 /*! \brief �����е���־��
	  */
    void enable_all_groups  (void);

	 /*! \brief �ر����е���־��
	  */
    void disable_all_groups (void);

	 /*! \brief ���ĳ��־���Ƿ񱻴�
	  * @return ����Ǵ򿪣�����true�����򷵻�false��
	  */
    bool group_enabled (Group g_) const;

	 /*! \brief ����־��Ϣ�м���ʱ���
	  */
    void enable_timestamp (void);

	 /*! \brief �ر�ʱ���
	  */
    void disable_timestamp (void);

	 /*! \brief �����־��Ϣ���Ƿ������ʱ���
	  * @return ����ǣ�����true�����򷵻�false��
	  */
    bool timestamp_enabled (void) const;

	 /*! \brief ����ʱ��
	  * 0 - GMT(��������), 1 - LOCAL(����)
	  */
    void set_timezone (int zone);

	 /*! \brief �ǼǺ������Ŀ�ʼ
	  *
	  */
    void sign_on (const string& func_name_);

	 /*! \brief �ǼǺ������Ľ���
	  */
    void sign_off (void);

    /*! \brief д��־��Ϣ����׼���
	  *  @return  �ɹ�����0, ���򷵻�-1����ȫ��errnoֵ.
	  */
    int  open (u_long groups_);

    /* \brief д��־��Ϣ���ļ�.
	 *  @return �ɹ�����0, ���򷵻�-1����ȫ�ֵ�errnoֵ.
	 */
    int  open (const char* logfname_,
	       u_long groups_, u_long maxsize_);

    /* \brief д��־��Ϣ���ļ�.
	 *  @return �ɹ�����0, ���򷵻�-1����ȫ�ֵ�errnoֵ.
	 */
    int  open (const char* base_dir_, int ccm_id_, u_long groups_, u_long maxsize_, char seq_delimer_, int switchday);

#if 0
    /*! \brief д��־��Ϣ����־������.
	  *  @return �ɹ�����0, ���򷵻�-1����ȫ�ֵ�errnoֵ.
	  */
    int  open (const char* logfname_, const INETAddress& logsvr_,
    			   u_long groups_);
#endif

	 /*! \brief �����½��̺�����ͬ����־��.
	  */
    void resync (void);

	 /*! \brief �ر���־��
	  */
    int  close (void);

	 /*! \brief д��־
	  */
    int log_msg  (u_long g_, const char* fmt_, ...);

	int log_func (u_long g_, marker_t type_);


private:
    Logger_Impl*  m_impl;	      /*!<  ��־��¼����ʵ����ʵ���� */
    stack<string> m_context;     /*!< �������������ĵĶ�ջ�� */
};

/*! \def LOGGER
 *  Logger����ʵ��ȡ�ú�
 */

#ifndef USE_SIMPLE_SINGLETON
#ifdef NEED_NAMESPACE
typedef sunrise::SingletonHolder<Logger, CreateUsingNew> SingleLogger;
#define LOGGER sunrise::SingleLogger::Instance()
#else
typedef SingletonHolder<Logger, CreateUsingNew> SingleLogger;
#define LOGGER SingleLogger::Instance()
#endif
#else
typedef SingletonHolder<Logger> SingleLogger;
#define LOGGER SingleLogger::Instance()
#endif

/*******************************************************************************
  Inline functions
*******************************************************************************/

inline void 
Logger::
enable_group (Group g_) 
{ 
    if (m_impl) {
		m_impl->enable_group (g_);
    }
}

inline void 
Logger::
disable_group (Group g_)
{
    if (m_impl) {
		m_impl->disable_group (g_);
    }
}

inline void 
Logger::
enable_groups (u_long g_) 
{ 
    if (m_impl) {
		m_impl->enable_groups (g_);
    }
}

inline void 
Logger::
disable_groups (u_long g_)
{
    if (m_impl) {
		m_impl->disable_groups (g_);
    }
}

inline bool 
Logger::
group_enabled (Group g_) const
{
    return (m_impl) ? m_impl->group_enabled (g_) : false;
}

inline void 
Logger::
enable_all_groups  (void)
{
    if (m_impl) {
		m_impl->enable_all_groups ();
    }
}

inline void 
Logger::
disable_all_groups (void)
{
    if (m_impl) {
		m_impl->disable_all_groups ();
    }
}

inline void 
Logger::
enable_timestamp (void)
{
    if (m_impl) {
		m_impl->enable_timestamp ();
    }
}

inline void 
Logger::
disable_timestamp (void)
{
    if (m_impl) {
		m_impl->disable_timestamp ();
    }
}

inline bool 
Logger::
timestamp_enabled (void) const
{
    return (m_impl) ? m_impl->timestamp_enabled () : false;
}

inline void 
Logger::
set_timezone (int zone_)
{
    if (m_impl) {
		m_impl->set_timezone (zone_);
    }
}

inline void 
Logger::
resync (void) 
{
    if (m_impl) {
		m_impl->resync ();
    }
}


inline void 
Logger::
sign_on (const string& func_name_)
{
    m_context.push (func_name_);
}

inline void 
Logger::
sign_off (void)
{
    if (!m_context.empty ()) {
		m_context.pop ();
    }
}

/*******************************************************************************
  Macro definition shortcuts
*******************************************************************************/
/** @def DL(X)
	дdebug��Ϣ����־��¼���ĺ�.
*/
#if defined (SUNRISE_NLOGGING)
#  define DL(X) do {} while (0)
#else
#  define DL(X) \
     do { \
       LOGGER.log_msg X; \
     } while (0)
#endif

/** @def EL(X)
	дerror��Ϣ����־��¼���ĺ�.
*/
#if defined (SUNRISE_NLOGGING)
#  define EL(X) do {} while (0)
#else
#  define EL(X) \
     do { \
       LOGGER.log_msg X; \
       LOGGER.log_msg(ERROR,"errno: %d \"%s\"\n", errno, strerror(errno)); \
     } while (0)
#endif

/*******************************************************************************
  Namespace Log provides only the most plausable interface.
  Other public functions can be reached via LOGGER
*******************************************************************************/

/*! \namespace Log
 *  \brief Log namespace
 */

namespace Log {					// @start namespace Log

    /**
	 * ��log�ļ�.
	 *
	 * @param logfname_ ��־�ļ���
	 * @param groups_ LogMask.h�ļ��ﶨ�����־�ȱʡ�������е���־��.
	 * @param maxsize_ ��־�ļ���С�����ֵ��ȱʡ��10M���ﵽ������������
	 * 						����־�ļ�������Ϊlogfname_0.
	 * @return �ɹ�����0, ���򷵻�-1.
	 */
    inline int open_log_file (const char* logfname_, 
				  u_long groups_ = ALL, 
				  u_long maxsize_ = 10 * 1024 * 1024) 
    {
		return LOGGER.open (logfname_, groups_, maxsize_);
    }

    /**
	 * ��BOSS��Ŀר��log�ļ�.
	 *
	 * @param ccm_id_ CCM_ID
	 * @param base_dir_ ��־�ļ���ŵĵط���
	 * @param groups_ LogMask.h�ļ��ﶨ�����־�ȱʡ�������е���־��.
	 * @param maxsize_ ��־�ļ���С�����ֵ��ȱʡ��10M���ﵽ������������
	 * 						����־�ļ�����ԭ����logfname_<���>����Ϊ
	 * 						logfname_<���+1>.
	 * @param seq_delimer_ ��־�ļ������ǰ�ķָ�����Ĭ��"_"
	 * @param switchday ��־�ļ����л�������Ĭ��5���л�һ��
	 * @return �ɹ�����0, ���򷵻�-1.
	 */
	inline int open_log_file ( int ccm_id_, 
								const char* base_dir_,
								u_long groups_ = ALL,
								u_long maxsize_ = 10 * 1024 * 1024,
								char seq_delimer_ = '_',
								int switchday = 5 ) 
	{
		return LOGGER.open ( base_dir_, ccm_id_, groups_, maxsize_, seq_delimer_, switchday);
	}

    /** д��־��Ϣ����׼���.
	  */
    inline int open_log_stdout (u_long groups_ = ALL)
    {
		return LOGGER.open (groups_);
    }

#if 0
    /** д��־��Ϣ����־������.
	  */
    inline int open_log_logserver (const char* logfname_, 
                      const INETAddress& logsvr_,
     		   u_long groups_)
     {
     	return LOGGER.open (logfname_, logsvr_, groups_);
     }
#endif

	 /** �����½��̺�����ͬ����־��.
	  */
    inline void resync (void) {	LOGGER.resync (); }
    
	 /** �ر���־��
	  */
    inline int close (void) { return LOGGER.close (); }

	 /** �趨ʱ��
	  */
    inline void set_gmt_timezone (void) { LOGGER.set_timezone (0); }

	/** ����ʱ���
	 */
    inline void enable_timestamp (void) { LOGGER.enable_timestamp (); }

	/** ȥ��ʱ���
	 */
    inline void disable_timestamp (void) { LOGGER.disable_timestamp (); }	

} // @end namespace Log

//------------------------------------------------------------------------------
// ���������.
//------------------------------------------------------------------------------
/** ���ٺ������ö�ջ������������ࡣ
 */
class DiagnosticContext
{
public:
	/** ���캯��
	 */
	DiagnosticContext (const char* fn_, u_long mask_ = TRACE);
	/** ��������
	 */
	~DiagnosticContext ();

private:
	/** ���ƹ��캯��
	 */
	DiagnosticContext (const DiagnosticContext&);
	/** ��ֵ���캯��
	 */
	DiagnosticContext& operator= (const DiagnosticContext&);

private:
	const char*  m_fname; //<! ��־�ļ���
	u_long       m_mask; //<! ��־�����
};

inline
DiagnosticContext::
DiagnosticContext (const char* fn_, u_long mask_)
	: m_fname (fn_), m_mask (mask_)
{
	LOGGER.sign_on (m_fname);
	LOGGER.log_func (m_mask, FUNC_ENTRY);
}

inline 
DiagnosticContext::
~DiagnosticContext ()
{
	LOGGER.log_func (m_mask, FUNC_EXIT);
	LOGGER.sign_off ();
}

/** @def trace(s)

    trace()���ڸ��ٺ�������ջ.

    trace()�ǼǸ�ʽ��:[function_name] --v-- �ļ���ΪTRACE����־��Ϣ��
    �����ﺯ�����˳�������¼��ʽ�磺[function_name] --^--����־��Ϣ
    
    Ӧ�ó���ͨ��������{'-m','--mask'}�ṩ��ͬ��debug������Է���ش�
	 ���߹رռ�¼����TRACE��־��Ϣ.
*/
#if defined (SUNRISE_NTRACE)
//#define trace(s) do {} while(0)
//#define TR(s) do {} while(0)
#define trace(s)
#define TR(s)
#else
#ifdef NEED_NAMESPACE
#define trace(s) sunrise::DiagnosticContext tRaCeR(s);
#define TR(s) sunrise::DiagnosticContext tRaCeR(s);
#else
#define trace(s) DiagnosticContext tRaCeR(s);
#define TR(s) DiagnosticContext tRaCeR(s);
#endif
#endif

/** @def trace_with_mask(s, m)

    trace_with_mask()���ڸ��ٺ�������ջ. ����m�ṩ����־���롣
*/
#ifdef NEED_NAMESPACE
#define trace_with_mask(s, m) sunrise::DiagnosticContext tRaCeR(s, m);
#else
#define trace_with_mask(s, m) DiagnosticContext tRaCeR(s, m);
#endif
/** @} */
END_SUNRISE_NAMESPACE
#endif /* LOGGER_H */
