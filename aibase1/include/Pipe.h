//------------------------------------------------------------------------------
//                           Pipe.h
//------------------------------------------------------------------------------
#ifndef PIPE_H
#define PIPE_H

#include <stdio.h>
#include <string>

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "Logger.h"

BEGIN_SUNRISE_NAMESPACE

/** \example pipe_test.cpp
 * �ܵ�����Գ��򣬴����ӽ��̵���smoker, ��ͨ����smoker֮��Ĺܵ�(Pipe)ͨѶ.
 */

/** \example smoker.cpp
 * �������ӽ���ִ��.
 */

/** \defgroup libsr �������߿�(libsr)
 * \{*/
/** \file Pipe.h
 *  \brief UNIXϵͳ����popen()/pclose()�İ�����(wrapper class).
 */

/** \class Pipe Pipe.h
 * \brief UNIXϵͳ����popen()/pclose()�İ�����(wrapper class).
 */
/** \}*/
class Pipe 
{
public:
	/** \brief ȱʡ���캯��
	 */
	Pipe ();

	/** \brief ��������
	 * ����close()�����رմ򿪵Ĺܵ�.
	 */
	~Pipe ();

	/** \brief ����һ����shell��ִ�������ַ���cmd_.
	 * Ϊ�ӽ��̴����ܵ�����Ӧ����type_ָ����"r"(read)��"w"(write)�ѹܵ�
	 * ճ�����ӽ��̵ı�׼����ͱ�׼������.��׼����/����ĺ���(fprintf(), fscanf()
	 * , fgets��)���԰���Ϣ�ڸ��ӽ���֮�䴫�ݡ�
	 *
	 * @see Fork
	 * @param cmd_  ִ�е������ַ���
	 * @param type_ "w" for write pipe and "r" for read pipe
	 * @return ��׼I/O��ָ�룬������󣬷���NULL������errno���������Ĵ���.
	*/
	FILE* open (const string& cmd_, const string& type_);

	/** \brief �رչܵ�.�ռ��ӽ���״̬ȷ���ӽ����ѽ���
	 *
	 * @return �ɹ�����0; ���򷵻�-1.
	*/
	int close ();

	/** \brief ��SIGTERM�ź�kill�ӽ���, ��û��ǵ���close()�ռ��ӽ���״̬
	 *
	 * @see close()
	 * @return �ɹ�����0, kill(2)ʧ�ܷ���-1.
	*/
	int kill ();

	/** \brief ȡ���ӽ���id. */
	pid_t pid () const;

	/** \brief ȡ�ùܵ��ı�׼I/O�ļ�ָ�� */
	FILE* fp () const;

	/** \brief ȡ�ùܵ����ļ������� */
	int fd () const;

private:						
	Pipe (const Pipe&);
	Pipe& operator= (const Pipe&);

private:
	FILE*  m_fp; /**< I/O��ָ�� */
	pid_t  m_child_pid; /**< �ӽ���id */
};

inline pid_t 
Pipe::pid () const { return m_child_pid; }

inline int
Pipe::fd () const { return fileno (m_fp); }

inline FILE*
Pipe::fp () const { return m_fp; }

END_SUNRISE_NAMESPACE

#endif // PIPE_H

