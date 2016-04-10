//------------------------------------------------------------------------------
//                           PidFileLock.h
//------------------------------------------------------------------------------
#ifndef FILE_LOCK_H
#define FILE_LOCK_H

//System Includes
#include <sys/types.h>

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif
#include "Assert.h"

//STL
#include <string>
using std::string;

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr �������߿�(libsr)
 * \{*/
/** \file PidFileLock.h
 *  \brief �����͹������PID���ļ�
*/

/** \class PidFileLock PidFileLock.h
 * \brief ����PID���ļ�������
 */
/** \}*/
class PidFileLock   : public flock
{
public:
	/** \brief ���캯��
	 */
	PidFileLock ();

	/** \brief ��������
	 * �������ӵ�н������ļ��������������ｫɾ������ļ�
	*/
	~PidFileLock ();

	/** \brief ���ļ�
	    \return ����ɹ�������true.���򷵻�false.
	*/
	bool lock (const string& filename_);

	/** \brief ��������Ǵ�ϵͳ������������<TT>errno</TT>ֵ. 
	    \return �޴��󷵻�0, ���򷵻�<TT>errno</TT>.
	*/
	int get_error () const;
	
	/** \brief ����<TT>errno</TT>����ʶ�Ĵ�����Ϣ
	*/
	const char* get_error_msg () const;

	/** \brief ��lock��״̬��Ϣд��debug�ļ�
	 * <TT>m_fd = -1</TT>��ʶlock�ļ�û�д�.
	*/
	void dump ();

private:
	/** \brief ���ļ�����
	    \return �ɹ�����������0, ���ִ��󷵻�-1,�������ļ��������Ľ���������
		 ʱ��errno�趨ΪEAGAIN����EACCES.
	*/
	int lock_region ();

	/** \brief �����ļ��Ƿ��ܱ���������.
	    \return �ɹ�(�ļ����Ա����������Ѿ�������������)ʱ����0, ʧ��ʱ��������
		 ������ļ����Ǹ����̵�id.
	*/
	pid_t test_region ();

	/** \brief ��¼������Ϣ������errno.
	*/
	void log_error   (const char* msg_);

private:
	string m_filename; /**< ���ļ��� */
	int    m_fd; /**< ���ļ������� */
	int    m_error; /**< ���һ��ϵͳ���ò�����errno */
	string m_error_msg; /**< ������Ϣ */
};

END_SUNRISE_NAMESPACE
#endif /* FILE_LOCK_H */  
