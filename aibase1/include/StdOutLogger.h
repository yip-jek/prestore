
//------------------------------------------------------------------------------
//                            StdOutLogger.h
//------------------------------------------------------------------------------
#ifndef STDOUT_LOGGER_H
#define STDOUT_LOGGER_H

#define USING_OLD_IOSTREAM
#include <fstream>
#include <string>

#include "Prelude.h"
#include "Logger_Impl.h"

BEGIN_SUNRISE_NAMESPACE

/** @addtogroup libsr 
@{
*/

/*! \file StdOutLogger.h 
 *
 * \brief ��׼�����־��¼��
 */

/*! \class StdOutLogger StdOutLogger.h
 * \brief ��׼�����־��¼��
 *
 *����Logger_Impl�Ľӿں������ڱ�׼������������־��Ϣ�ͺ�������ˢ�����buffer.
 */
class StdOutLogger : public Logger_Impl
{
public:
	StdOutLogger () { /*empty*/ }
    
	/*!  \brief ����˵��:������־�������
	 *   @param[in]  groups_ ��־����
	 * @return 0��ʾ�ɹ�
	*/
	virtual int  open   (u_long  groups_);
	
	/*!  \brief ����˵��:ֻʵ�ֹر�������ӿڣ����������κδ��� 
	 * @return 0��ʾ�ɹ�
	*/
	virtual int  close  (void);

    /*!  \brief ����˵��:�ڱ�׼������������־��Ϣ,��
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

   /*!  \brief ����˵�����ڱ�׼����������������,��ϵͳʱ�������־ΪTRUE�����ϵͳʱ�䡣 
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

 /*!  \brief ����˵����ˢ�±�׼���������������� */
	virtual void resync ();

private:
  /*!  \brief ����˵�������������� */
	StdOutLogger (const StdOutLogger&);            /// no cloning
  /*!  \brief ����˵������ֵ������ */
	StdOutLogger& operator= (const StdOutLogger&); /// no cloning
};

/*******************************************************************************
 Inline member functions
*******************************************************************************/
inline int
StdOutLogger::
open (u_long groups_)
{
    m_groups   = groups_;
    return 0;
}

inline int
StdOutLogger::
close (void)
{
    return 0;
}

inline void
StdOutLogger::
resync ()
{
	COUT << FLUSH;
}

/** @} */

END_SUNRISE_NAMESPACE

#endif /* STDERR_LOGGER_H */
