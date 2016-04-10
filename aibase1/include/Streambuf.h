//------------------------------------------------------------------------------
//                         Streambuf.h
//------------------------------------------------------------------------------
#ifndef STREAM_BUF_H
#define STREAM_BUF_H

#include <stdio.h>		// EOF
#include "Prelude.h"
#include "Assert.h"		// trace() & Assert family

BEGIN_SUNRISE_NAMESPACE

/*! \file Streambuf.h 
 * ����Standard C++ iostream streambuf���������࣬ʵ���׽ӿ���ײ�Ļ�������.
 */

/** 
 * io_ptrs structure.
 */
class io_ptrs
{
public:
	char* m_read_base;
	char* m_read_ptr;
	char* m_read_end;

	char* m_write_base;
	char* m_write_ptr;
	char* m_write_end;

	char* m_buf_base;
	char* m_buf_end;

	enum { USER_BUF = 1, UNBUFFERED = 2, EOF_SEEN = 4, ERR_SEEN = 8 };

	int   m_flags;
	char  m_shortbuf[1];

	io_ptrs () : m_read_base (0), m_read_ptr (0), m_read_end (0),
		     m_write_base (0), m_write_ptr (0), m_write_end (0),
		     m_buf_base (0), m_buf_end (0), m_flags (0)
		{ 
			m_shortbuf [0] = 0; 
		}

	void dump () const;
};

/*! \class Streambuf 
 *
 * \brief ����������
 *
 * Streambuf�����Standard C++ iostream streambuf.ʵ���׽ӿ���
 * �ײ�Ļ������.
 *
 * Streambuf���ڲ�������������: get����,put����ͱ�������.
 *
 * get���򱣴��������õ����뻺������.
 * put���򱣴滹û��ˢ�µ�����Ļ�������.
 * ����������get���򲿷��ص�. get�����put���������չ��
 * ���������ʣ�ಿ��. �����������Ĳ��������У�get�����
 * put������չ������С, �������ܻ����С��Χ��.
 */
class Streambuf : public io_ptrs
{
public:
	static const int MAXTCPFRAMESZ; /*!< TCP֡�����ߴ�(TCPѡ��) */

	/*! \brief ��������
	 */
	virtual ~Streambuf ();

	/*! \brief ���û�����
	 *  @return ���ػ���������ָ��.
	 */
	Streambuf* pubsetbuf (char* s_, int n_);

	/*! \brief ˢ�����ݵ����.
	 * @see sync
	 * @return ����ˢ�����ݵ��ֽ���.
	 */
	int        pubsync ();

	/*! \brief ��Socet���л���I/O������£��������������������socket api
	 * ������ֱ�ӷ��ػ������п��õ��ֽ���, ȷ����ȡ���ֽڲ��������֤
	 * ������ʵײ��׽���.
	 */
	int        in_avail ();

	/*! \brief �ƶ�get����ָ�뵽��һ��λ�ã�������һ�ζ�������λ��.
	    ����ٵ����������֮ǰ����֮��get����ָ���Ѿ�����EOF, �򷵻�EOF.
	*/
	int        snextc ();

	/*! \brief �ƶ�get����ָ�뵽��һ���ַ���λ�ã�������֮ǰ���ַ�
	 * �����ǰ��get����ָ���Ѿ���EOF, ����EOF.
	 */
	int        sbumpc ();

	/*! \brief ���ص�ǰget����ָ�����һ���ַ�. �������get����
	 * ָ���Ѿ�����EOF������EOF.(get����ָ�벻�ᱻ�ƶ�)
	 */
	int        sgetc ();

	/*! \brief ȡ����len_���ַ�,������b_����. get����ָ���ƶ�len_��
	 * �ֽ�.�����ȡ���ֽ�����len_����ȡ�����п�ȡ�ģ�����ʵ��ȡ��
	 * ���ַ���.
	 * @return ����ʵ��ȡ�����ַ���.
	*/
	int        sgetn (char* b_, int len_);

	/*! \brief ����c��put����ָ�����һ��λ��,put����ָ���ƶ�һ��
	 * �ֽ�.��չput����߽�.  ����c�����ʧ�ܣ�����EOF. ��ι���
	 * ���������������.
	 */
	int        sputc (char c_);

	/*! \brief ����len_���ַ���put����ָ��֮��. �ƶ�put����ָ��len_
	 * ���ֽ�.����ɴ洢�Ŀռ�����len_����洢���ó��ȵ����ݡ�
	 *
	 * @return ����ʵ�ʴ洢���ֽ���.
	 */
	int        sputn (char* b_, int len_);


	/*! \brief ���i_���㣬���е�IO������Ҫ�л���.����IO������û�л���.
	*/
	void       unbuffered (int i_);
	
	/*! \brief  ����Ƿ��޻���.
	 * @return ������޻��壬����true, ���򷵻�false.
	 */
	int        unbuffered ();

protected:
	/*! \brief ȱʡ���캯��.
	 * ����protected��ȷ����������ܹ���.
	 */
	Streambuf ();

	Streambuf (const Streambuf&);
	Streambuf& operator= (const Streambuf&);

	/*! \brief ����get����ָ��ĵ�ֵ.
	*/
	char*      base () const;

	/*! \brief ����get����ָ���ֵ.
	 * ��һ�εĴ�ȡ��������ͨ��egptr()-1-gptr()����֪��ȡ�������ݡ�
	 * ���egptr()<=gptr()����ʾû�����ݿ�ȡ.
	 */
	char*      gptr ()  const;

	/*! \brief ����get����ָ��ĵ�ֵ
	*/
	char*      egptr () const;

	/*! \brief ����get����ָ��.
	 */
	void       setg (char* gbeg_, char* gnext_, char* gend_);

	/*! \brief ����put����ָ��ĵ�ֵ
	 * put����ָ��ֵ��ͨ��pptr()-1�����õ���pptr()-1-pbase()����֪��
	 * put����洢�˶������ݡ�
	*/
	char*      pbase () const;

	/*! \brief ����put����ָ���ֵ.
	 */
	char*      pptr ()  const;

	/*! \brief ����put����ָ���ֵ
	 */
	char*      epptr () const;

	/*! \brief ����put����ָ��
	*/
	void       setp (char* pbeg_, char* pend_);

	/*! \brief put����ָ���ƶ�n_λ.
	 */
	void       pbump (int n_);

	/*! \brief ������������. 
	 * ����ָ���ֵ(base())����Ϊb_, ��ֵ(ebase())����Ϊeb_, 
	 * ���del_���㣬�����ۺ�ʱ��ֵ�ı䶼��ѻ���ɾ��.����,
	 * ���岻���Զ���ɾ��.
	 *
	 *   @param b_ �����һ���ֽڵĵ�ַ
	 *   @param eb_ �������һ���ֽڵĵ�ַ
	 *   @param del_ 0 - �ⲿ���ڴ������������󲻻��Լ������ڴ�.
	 *   				1 - �ڲ��Զ����ڴ�����ڽ�������������ʱ��ɾ������.
	*/
	void       setb (char* b_, char* eb_, int del_);

	void       init ();

protected:
	/*! \brief ���û���ָ��ͻ���������.
	 *
	    @param p_ ������ָ��
	    @param len_ ����������
	*/
	virtual Streambuf* setbuf (char* p_, int len_);

	/*! \brief ������ͬ��
	 * �������put���������ˢ�µ�Ŀ�ĵ�(����socket���壬���Ƿ��͵��Է�
	 * �׽ӿ�), ͬʱ�����뻺���ȡ���ݡ�
	 *
	 * @return ������ִ��󷵻�EOF, �ɹ��򷵻�0. ����������࣬
	 * ȱʡ����Ϊ��: ���û������Ϊ�������������뻺������������,
	 * ����0, ���򷵻�EOF.
	 */
	virtual int        sync ();
	
	/*  \brief ���ػ����п��õ�����������ֵ. 
		 
		 @return ������ش���0��ֵ����ô����underflow()����������
		 ����EOF, �������-1, ����underflow()����uflow()��ʧ��.
	*/
 	virtual int        showmanyc ();

	/*! \brief ���������з���len_���ֽڵ����ݵ�b_.
	    @return ������ֽ���.
	*/
	virtual int        xsgetn (char* b_, int len_);

	/*! \brief ��get����Ϊ��ʱ�ṩ����������.
	 */
	virtual int        underflow ();

	/*! \brief ���������ж�������, �ƶ���ָ��λ�����£�

	    1) �������������һ����λ��ֵ���򷵻�(unsigned char)*gnext++ֵ��־�ɹ�.

	    2) ����, ����ܴ���������ֱ�Ӷ���, �򷵻�(unsigned char) xֵ��־�ɹ�. 

	*/
	virtual int        uflow ();

	/*! \brief  ���������дlen_���ֽڵ�����(ͨ��ѭ������sputc(c)).
	 *
	 * @param b_ �������ָ��
	 * @param len_ ���ݳ���
	 * @return ����ʵ��д����ֽ���.
	 */
	virtual int        xsputn (const char* b_, int len_);

	/*! \brief ��put����������Ҫ�洢���������ʱ��������(������ˢ�µ����),
	 * ��������ȱʡ��Ϊû�ж��塣������������趨���Լ���overflow������
	 * ͨ���Ķ���������put����(pbase()��pptr()֮��)������,  ͬʱ����setp()
	 * ��������һ���µ�put����Ȼ�����c����EOF�Ļ��洢c(ʹ��sputc()����).
	 *
	 * @return ��������ʱ����EOF�����򷵻�����ֵ.
	 */
	virtual int        overflow (int c = EOF);

	/*! \brief ������unbuffered()��base()����Ϊ0ʱ������һ���ʵ��ߴ�
	 * �ı�������
	 *
	 * @return ����ɹ����������������ش���0��ֵ�����򷵻�EOF.
	 * ȱʡ�Ĳ�������new����������.
	 */
	virtual int        doallocate ();
};


END_SUNRISE_NAMESPACE

#endif /* STREAM_BUF_H */  
