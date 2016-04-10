//------------------------------------------------------------------------------
//                          TimeVal.h
//------------------------------------------------------------------------------
#ifndef TIME_VAL_H
#define TIME_VAL_H

#include <sys/time.h>		// gettimeofday(3)
#include <unistd.h>		// gettimeofday(3)

#include <string> 

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif

/** \example timeval_test.cpp
 * \brief TimeVal����Գ���
 */

using std::string;

BEGIN_SUNRISE_NAMESPACE

/** \defgroup libsr �������߿�(libsr)
 * \{*/
/** \file TimeVal.h
 *  \brief UNIXϵͳtimeval�ṹ������
 */
/** \class TimeVal
 *  \brief UNIXϵͳ��timeval�ṹ������
 */
/** \}*/
class TimeVal : public timeval
{
public:
	enum { 
		gmt,					/**< ��������ʱ�� */
		loc						/**< ����ʱ�� */
	};

	/** \brief ȱʡ���캯��
	 sec������Ϊ0, usecs������Ϊ0.
	 Ҫȡ�õ�ǰʱ�䣬�������� - TimeVal(gettimeofday());
	 */
	TimeVal ();

	/** \brief ��seconds/milliseconds�Թ���TimeValʵ��.
	 *  \param sec_ ������
	 *  \param msec_ ΢����
	 */
	TimeVal (long sec_, long msec_);

	/** \brief ���캯��
	 *  \param d_ ����������Ϊ������,С�����6β��Ϊ΢������ʼ��
	 */
	TimeVal (double d_);

	/** \brief ��<TT> struct timeval</TT>�ṹ�幹��TimeValʵ��.
	 *  \param tv_ ��һ��timeval����ʼ��
	 */
	TimeVal (const timeval& tv_);

	/** \brief �������캯��
	 */
	TimeVal (const TimeVal& tv_); 

	/** \brief ����ת��������
	 */
	operator double () const;

	/** \brief ����timeval�ṹ��sec
	 */
	void sec (long sec_);

	/** \brief ��ȡtimeval�ṹ��sec
	 */
	long sec (void) const;

	/** \brief ����timeval�ṹ���milliseconds.
	 */
	void msec (long msec_);

	/** \brief ��ȡtimeval�ṹ���milliseconds.
	 */
	long msec (void) const;

	/** \brief ת��tv_usec (microseconds) Ϊ milliseconds (1 / 1,000��)
	 */
	long millisec () const;

	/** \brief ����ʱ��
	 */
	void tz (int tz_);

	/** \brief ��ȡʱ��
	 */
	int tz (void) const;

	TimeVal& operator= (const TimeVal& tv_);

	/// Addition.
	TimeVal& operator+= (const TimeVal& rhs_);

	/// Substraction.
	TimeVal& operator-= (const TimeVal& rhs_);

	/// Addition.
	friend TimeVal operator+(const TimeVal& lhs_, const TimeVal& rhs_);

	/// Substraction.
	friend TimeVal operator-(const TimeVal& lhs_, const TimeVal& rhs_);

	/// Comparison.
	bool operator< (const TimeVal& rhs_) const;

	/// Equality.
	bool operator==(const TimeVal& rhs_) const;

	/// Comparison.
	friend bool operator> (const TimeVal& lhs_, const TimeVal& rhs_);

	/// Comparison.
	friend bool operator!=(const TimeVal& lhs_, const TimeVal& rhs_);

	/// Comparison.
	friend bool operator<=(const TimeVal& lhs_, const TimeVal& rhs_);

	/// Comparison.
	friend bool operator>=(const TimeVal& lhs_, const TimeVal& rhs_);

	/** \brief ��ʽ��timeval�ṹ��, ȱʡ��ʽΪMM/DD/YYYY HH:MM:SS.MMM
	    ���Ҫ�����ĸ�ʽ���ɲο�man 3 strftime. ͨ������fmt_����
		 ��ʽ�ַ���
	    \param fmt_ strftime(3)����ĸ�ʽ�ַ���
	    \return ���������ַ���.
	*/
	string fmtString (const char* fmt_ = NULL) const;

	/** \brief ��ʽ��timeval�ṹ��ΪHH:MM:SS
	 */
	string fmt_hh_mm_ss () const;

	/** \brief ��ʽ��timeval�ṹ��ΪHH:MM:SS.MLS
	 */
	string fmt_hh_mm_ss_mls () const;

	/** \brief ��ʽ��timeval�ṹ��ΪMM:SS
	 */
	string fmt_mm_ss () const;

	/** \brief ��ʽ��timeval�ṹ��ΪMM:SS.MLS
	 */
	string fmt_mm_ss_mls () const;

	/** \brief ��ʽ��timeval�ṹ��ΪSS.MLS
	 */
	string fmt_ss_mls () const;

	/** \brief ��timeval�ṹд����־�ļ�
	*/
	void dump_to_log (const string& name_ = "") const;

	/** Static that returns zero timeval: {0,0}
	 */
	static TimeVal zeroTime ();

	/** \brief ȡ�õ�ǰʱ��(����ϵͳ����gettimeofday())
	    \return time of the day as timeval
	*/
	static TimeVal gettimeofday ();

protected:
	/// Internal initialization common to most constructors.
	void init (long, long, int);

//private:
public:
	/// Normalization after arithmetic operation.
	void normalize ();

private:
	/// ʱ��
	int m_tz;

	/// Zero time value
	static TimeVal m_zero;
};
//TimeVal operator+(const TimeVal& lhs_, const TimeVal& rhs_);

END_SUNRISE_NAMESPACE

#endif /* TIME_VAL_H */  
