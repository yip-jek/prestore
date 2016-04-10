#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

#define USING_OLD_IOSTREAM

#include <fstream>
#include <string>

#include "Prelude.h"
#include "Logger_Impl.h"

BEGIN_SUNRISE_NAMESPACE

/** @addtogroup libsr 
@{
*/

/*! \file FileLogger.h
    \brief �ļ���־��¼��
*/
/*! \class FileLogger
 * \brief �ļ���־��¼��
 *
 * �ļ���־��¼��ʵ����, ��־��Ϣ��¼��ָ�����ļ�.
 */
class FileLogger : public Logger_Impl
{
public:
	/*! \brief ����˵����ȱʡ���캯�� */
	FileLogger ();

	/*! \brief ����˵��:����־�ļ�. \n
	 * 
	 *  @param[in]  logfname_ ��־�ļ���
	 *  @param[in]  groups_   ��־������
	 *  @param[in]  maxsize__   ��־�ļ�����С
	 * @return 0��ʾ�ɹ���-1��ʾʧ��
	*/
	virtual int open (const char* logfname_, 
			  u_long      groups_,
			  u_long      maxsize_ = 10485760); // 10 Mb = 1024x1024x10

	/*! \brief ����˵��:�ر���־�ļ�. \n
	*
	*    ˢ����������ݻ��壬�ر������־�ļ�
	*  @return 0��ʾ�ɹ�
    */
	virtual int  close  (void);

	/*! \brief ����˵��:ˢ����������ݻ��嵽��־�ļ� */
	virtual void resync (void);

    /*!  \brief ����˵��:��������������־��Ϣ,
	 *
	 *   ������־����������־�ļ�������ͱ�־,��ϵͳʱ�������־ΪTRUE,�����ϵͳʱ�䣬����������Ϊ�����������,
	 *   �������־��Ϣ������־�ļ������������ֵ��������־�ļ�����������־�ļ������ش�ԭ��־�ļ�. 
	 *   @param[in]  groups_      ��־����
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
     *   ������־����������־�ļ�������ͱ�־,��ϵͳʱ�������־ΪTRUE,�����ϵͳʱ�䣬����������Ϊ�����������,
	 *   ����־�ļ������������ֵ��������־�ļ�����������־�ļ������ش�ԭ��־�ļ�
	 *   @param[in]  groups_      ��־����
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
	
   /*! \brief �ļ���״ֵ̬
     * - opened �ļ�������
	 * - closed �ļ��ѹر�
	*/
	enum state_t { opened, closed };
    /*!  \brief ����˵�������������� */
	FileLogger (const FileLogger&);            // copy constructor
	/*!  \brief ����˵������ֵ������ */	
	FileLogger& operator= (const FileLogger&); // copy assignment

	int handle_rollover (void);

private:
	/*! \brief ����� */
	std::ofstream  m_sink;
	/*! \brief ��־�ļ��������,ȱʡΪ10M */
	u_long    m_maxsize;
	/*! \brief ��־�ļ��򿪹ر�״̬ */
	state_t   m_state;
	/*! \brief ��־�ļ���ǰ��С */
	u_long    m_bytecount;
};

inline FileLogger::
FileLogger () 
    : m_maxsize   (1048576), 
      m_state     (closed),
      m_bytecount (0)
{
	/*--- empty ---*/
}

inline void 
FileLogger::
resync (void)
{
	m_sink << std::flush;
}

/** @} */

END_SUNRISE_NAMESPACE

#endif /* FILE_LOGGER_H */
