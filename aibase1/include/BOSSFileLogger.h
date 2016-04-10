
#ifndef BOSSFILE_LOGGER_H
#define BOSSFILE_LOGGER_H

#define USING_OLD_IOSTREAM

#include <fstream>
#include <string>

#include "Prelude.h"
#include "Logger_Impl.h"


BEGIN_SUNRISE_NAMESPACE

#define LOG_SEQ_DELIMER '_'

/** @defgroup libsr ��־���ģ��(libsr) 
@{
*/

/*! \file BOSSFileLogger.h
    \brief BOSSϵͳ���ļ���־��¼��
*/
/*! \class BOSSFileLogger
 *\brief BOSS�ļ���־��¼��ʵ����
 *
 * ����־��ʱ��ָ��CCM_ID��������¼���Զ�����
 * BOSSϵͳ�Ĺ��������־�ļ����������������ļ�����ĳ��ָ���ĳߴ�ʱ�Զ�
 * �����µ���־��Ų�����־�ļ�����
 */
class BOSSFileLogger : public Logger_Impl
{
public:

	/*! \brief ����˵�������캯�� */
	BOSSFileLogger ();

    /*!  \brief ����˵��:����־�ļ�. \n
	 * 
	 *  ͨ���ṩ���Ŀ¼���Խ���ID�����ں�˳��Ų����ļ�����ȷ�������Ŀ¼��������ͬ����־�ļ���,�����ļ�
	 *  @param[in]  base_dir_ ��־���Ŀ¼
	 *  @param[in]  ccm_id_   ���̱��
	 *  @param[in]  groups_   ��־������
	 *  @param[in]  maxsize_   ��־�ļ�����С
	 *  @param[in]  seq_delimer_   ��־�ļ����ָ���
	 * @return 0��ʾ�ɹ���-1��ʾʧ��
	*/
	virtual int open (const char*	base_dir_,
			  int		 ccm_id_,
			  u_long      groups_,
			  u_long      maxsize_ = 10 * 1024 * 1024,  // 10 Mb = 1024x1024x10
			  char 		  seq_delimer_ = LOG_SEQ_DELIMER,
			  int 		switchday = 5); 
   
	/*!  \brief ����˵��:�ر���־�ļ�. \n
	*
	*    ˢ����������ݻ��壬�ر������־�ļ����ͷ�ռ����Դ
	*  @return 0��ʾ�ɹ�
    */
	virtual int  close  (void);

    /*!  \brief ����˵��:ˢ����������ݻ��嵽��־�ļ� */
	virtual void resync (void);

    /*!  \brief ����˵��:��������������־��Ϣ,
	 *
	 *   ������־����λ����־�ļ�������ͱ�־,��ϵͳʱ�������־ΪTRUE,�����ϵͳʱ�䣬����������Ϊ�����������,
	 *   �������־��Ϣ����ϵͳʱ�������л���������־�ļ������������ֵ��������־�ļ����л�, 
	 *   �ر�ԭ��־�ļ���������־�ļ�
	 *   @param[in]  groups_      ��־����λ
	 *   @param[in]  indent_level �������
	 *   @param[in]  func_name_   ��������
	 *   @param[in]  fmt_         �����ʽ
	 *   @param[in]  va_list      ���ֵ�б�
	 * @return 0��ʾ�ɹ�
	 * @pre ��־�ļ��ɹ��򿪺�
	*/
	virtual int log_msg (Group g_, 
						 size_t indent_level_,
						 const string& func_name_,
						 const char* fmt_, va_list);

   /*!  \brief ����˵���������������������� 
     *
     *   ������־��������־�ļ�������ͱ�־,��ϵͳʱ�������־ΪTRUE,�����ϵͳʱ�䣬����������Ϊ�����������,
	 *   ��ϵͳʱ�������л���������־�ļ������������ֵ��������־�ļ����л�,�ر�ԭ��־�ļ���������־�ļ�
	 *   @param[in]  groups_      ��־����λ
	 *   @param[in]  indent_level �������
	 *   @param[in]  func_name_   ��������
	 * @param[in] type_  �ַ��������־λ��ȡֵ���� \n           
	 * - FUNC_ENTRY ���Ϊ'/'
     * - FUNC_EXIT  ���Ϊ'\'
	 * - FUNC_EXIT  ���Ϊ'[]'
	 * @return 0��ʾ�ɹ�
 	 * @pre ��־�ļ��ɹ��򿪺�
   */
	virtual int log_func (Group g_, 
						  size_t indent_level_,
						  const string& func_name_,
						  marker_t type_);

   /*!  \brief ����˵����������Ա�����е�ֵ������δ����     */
	void dump (void);

private:
	enum state_t { opened, closed };
   /*!  \brief ����˵�������������� */
	BOSSFileLogger (const BOSSFileLogger&);            // copy constructor
   /*!  \brief ����˵������ֵ������ */	
	BOSSFileLogger& operator= (const BOSSFileLogger&); // copy assignment
   /*!  \brief ����˵��������ϵͳʱ��ͳ�Ա����������ǰ������ȷ���Ƿ������־�ļ������л���
   * @return true��ʾҪ������־�ļ����л���flase�����κδ���
   */
	bool switch_dt(void);
  /*!  \brief ����˵��������ϵͳʱ�����־�ļ���С�л���־�ļ����� 
   * @return 0��ʾ�ɹ���-1��ʾʧ��
   */
	int handle_rollover ( void );

private:
	/*! \brief ����� */
	std::ofstream  m_sink;
	/*! \brief ���̺�*/
	int	  m_ccm_id;
	/*! \brief �ָ���,ȱʡΪ"_" */
	char      m_seqdelimer;
	/*! \brief ��־�ļ������ */
	int 	  m_year;
	/*! \brief ��־�ļ����·� */
	int	  m_month;
	/*! \brief ��־�ļ����շ� */
	int 	  m_day;
	/*! \brief Ŀ־�ļ����Ŀ¼ */
	char*	  m_basedir;
	/*! \brief Ӧ�ó����� */
	char*   m_app_name;
	/*! \brief ��־�ļ��������,ȱʡΪ10M */
	u_long    m_maxsize;
	/*! \brief ��־�ļ��򿪹ر�״̬ */
	state_t   m_state;
	/*! \brief ��־�ļ���ǰ��С */
	u_long    m_bytecount;
	/*! \brief ��־�л�����,Ĭ��5��һ��*/
	int m_switchday;
};

inline BOSSFileLogger::
BOSSFileLogger () 
    : m_maxsize   (10 * 1024 * 1024), 
      m_state     (closed),
      m_bytecount (0),
	  m_switchday (5)
{
	/*--- empty ---*/
}

inline void 
BOSSFileLogger::
resync (void)
{
	m_sink << std::flush;
}

END_SUNRISE_NAMESPACE

/** @} */

#endif /* FILE_LOGGER_H */
