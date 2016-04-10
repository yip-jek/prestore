
//------------------------------------------------------------------------------
//                              Logger_Impl.h
//------------------------------------------------------------------------------
#ifndef LOGGER_IMPL_H
#define LOGGER_IMPL_H

#define USING_OLD_IOSTREAM
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>



#include <string>

using std::string;
using std::ostream;


#include "Prelude.h"
#include "LogMask.h"

BEGIN_SUNRISE_NAMESPACE
/** @addtogroup libsr 
@{
*/

/*! \file Logger_Impl.h
    \brief ��־��¼���ӿ���
*/


/// Maximum length of the formatted message
//#define LOGGER_MAXLINE 256
#define LOGGER_MAXLINE 1024

/*! \class Logger_Impl Logger_Impl.h
 * \brief ��־��¼��ʵ����
 *
 *��־��¼���Ļ��࣬Ϊ�̳����ṩ�������������ܰ�����\n
 * - ����ʱ�������־ \n
 * - ����ʱ�������־ \n
 * - ����������ʽ \n
 * - �ṩ����־�������־��ˢ����־��������־ͨ�ýӿ�
 */
class Logger_Impl {
public:
	/*��\brief ���캯�� */
    Logger_Impl ();
	/*��\brief �������� */
    virtual ~Logger_Impl () { /* empty */ }
    
	/*! \brief ����˵�������õ�����־������� 
	 * @param[in]  g_�ض���־���� 
	*/
    void enable_group  (Group g_)  { m_groups |= g_;  }
    /*! \brief ����˵����ɾ��������־������� 
	 * @param[in]  g_�ض���־���� 
	 */
	void disable_group (Group g_)  { m_groups &= ~g_; }
   
	/*! \brief ����˵�������ö�����־�������
	 * @param[in]  g_���������־���� 
	 */
    void enable_groups  (u_long g_)  { m_groups |= g_;  }
    /*! \brief ����˵����ɾ��������־������� 
	 * @param[in]  g_���������־����
	 */
	void disable_groups (u_long g_)  { m_groups &= ~g_; }

    /*! \brief ����˵����������־�������ȱʡֵ */
    void enable_all_groups  (void) { 
#ifdef NEED_SPACENAME 
		 m_groups = sunrise::ALL;  
#else
		 m_groups = ALL;  
#endif
	 }
    /*! \brief ����˵����������־���ʧЧ */
    void disable_all_groups (void) { m_groups = 0;    }

	/*! \brief ����˵�������������־���� 
	 * @param[in] g_ ������־����
	 * @return ��������־����
	 */
    bool group_enabled (Group g_) const { return (m_groups & g_); }
    
	/*! \brief ����˵��������ϵͳʱ�������Ч��־ */
    void enable_timestamp  (void)       { m_tmflg = true;  }
     /*! ����˵��������ϵͳʱ�����ʧЧ��־ */
	void disable_timestamp (void)       { m_tmflg = false; }
    /*! \brief ����˵������ȡϵͳʱ�������־ 
	 * @return ϵͳʱ�������־
	 */
	bool timestamp_enabled (void) const { return m_tmflg;  }
     /*! \brief ����˵��������ʱ�� 
	  * @param[in] zone_ ʱ������
	 */
	void set_timezone      (int zone_)  { m_tz = zone_;    }
    /*! \brief ����˵����������������
	* @param[in] step_ ��������
	*/
    void    set_indent_step (u_short step_) { m_indent_step = step_; }
	/*! \brief ����˵������ȡ�������� 
	 * @return ϵͳʱ�������־
	*/
    u_short get_indent_step (void) const    { return m_indent_step;  }

    /*! \brief ����˵����Ϊ�Ա�׼�����Ϊ��־����ṩͳһ�ӿ�*/
    virtual int open (u_long groups_);

    /*! \brief ����˵����Ϊ����־�ļ����ṩͳһ�ӿ�*/
    virtual int open (const char* logfname_, u_long groups_, u_long maxsize_);

	/*! \brief ����˵������־�ļ��������Ŀ¼���Զ�����,Ϊ����־�ļ��ṩͳһ�ӿ�*/
    virtual int open (const char* base_dir_, int ccm_id_, u_long groups_, u_long maxsize_, char seq_delimer_, int switchday);

#if 0
    /// Open LogServer
// 	virtual int open (const char* logfname_,
// 			  const INETAddress& logsvr_,
// 			  u_long groups_);
#endif
    /*! \brief ����˵����Ϊ�ر���־�ļ��ṩͳһ�ӿ� */
    virtual int  close  (void) = 0;
	/*! \brief ����˵����Ϊˢ������������ṩͳһ�ӿ� */
    virtual void resync (void) { /* empty */ }
    
	/*! \brief ����˵����Ϊ�����־��¼�ṩͳһ�ӿ� */
    virtual int log_msg (Group g_, size_t indent_level_, 
						 const string& func_name_, 
						 const char* fmt_, va_list) = 0;

    /*! \brief ����˵����Ϊ���������������������ṩͳһ�ӿ� */
    virtual int log_func (Group g_, size_t indent_level_, 
						  const string& func_name_, 
						  marker_t type_) = 0;
protected:
	
/*!  \brief ����˵������ϵͳʱ�������־Ϊtrue,��������������ϵͳʱ�䣬ʱ�侫ȷ������ 
      * @param[in] sink_ ��־��¼�����
      * @return  ��ϵͳʱ�������־Ϊtrue����23,����Ϊ0
 	  * @pre ��־�ļ��ɹ��򿪺�
*/
    virtual u_short add_timestamp    (ostream& sink_);

/*!  \brief ����˵�����������������������ƣ���Ҫ���ڸ��ٺ����Ľ�����˳��� 
      * @param[in] sink_ ��־��¼�����
	  * @param[in] funcname_ ��������
	  * @param[in] indent_level_ �������
	  * @param[in] type_  �ַ��������־λ��ȡֵ���� \n           
	  * - FUNC_ENTRY ���Ϊ'/'
      * - FUNC_EXIT  ���Ϊ'\'
	  * - FUNC_EXIT  ���Ϊ'[]'
	  * @return  ��������ӷ�������
 	  * @pre ��־�ļ��ɹ��򿪺�
*/
    virtual u_short indent_func_name (ostream& sink_, const string& funcname_,
					size_t indent_level_,
				  marker_t type_);

protected:
    /*! \brief ����˵������־�������λ�� */
    u_short m_indent_step;

   /*! \brief ����˵����������־������˱�־ */
    u_long m_groups;

    /*! \brief ����˵������־�ļ��� */
    string m_logfname;

     /*! \brief ����˵��������־��¼�����ϵͳʱ���־λ */
    bool   m_tmflg;

    /*! \brief ����˵����ʱ�������־λ Timezone: 0-GMT, 1-Local  */
    int    m_tz;
};

inline
Logger_Impl::
Logger_Impl () 
    : m_indent_step (3), m_groups (0), m_tmflg (false), m_tz (0)
{
    // empty
}

inline int 
Logger_Impl::
open (u_long /* groups_ */)
{
    errno = ENOSYS;
    return -1;
}

inline int 
Logger_Impl::
open (const char* /* logfname_ */, u_long /* groups_ */, u_long /* maxsize_ */)
{
    errno = ENOSYS;
    return -1;
}

inline int 
Logger_Impl::
open (const char* /* base_dir_ */, int /* ccm_id_ */, u_long /* groups_ */, 
		u_long /* maxsize_ */, char /* seq_delimer */, int /* switchday */)
{
    errno = ENOSYS;
    return -1;
}

#if 0
// inline int 
// Logger_Impl::open (const char* logfname_, const INETAddress& logsvr_,
// 		   u_long groups_)
// {
// 	errno = ENOSYS;
// 	return -1;
// }
#endif

/*! ��־���
@}
*/

END_SUNRISE_NAMESPACE

#endif /* LOGGER_IMPL_H */
