
#ifndef CMD_LINE_OPTS_H
#define CMD_LINE_OPTS_H

#ifndef __PRELUDE_H__
#include "Prelude.h"
#endif
#include "Assert.h"

#include <string> 
#include <vector>
using std::string;
using std::vector;

/** \example cmdlineopt_test.cpp
 * \brief �����в������Գ���
 */

/** \defgroup libsr  ��������ģ��(libsr) 
 */
/** \{*/

/** \file CmdLineOpts.h
 *       \brief ������ѡ�����
 *       ����������ѡ��
 */


/** \class Option CmdLineOpts.h "include\CmdLineOpts.h"
 * \brief ������ѡ������Э����(Helper).
 * �����ֻ��������ѡ����ʹ�ã����ܱ�ʵ������
 */
/** \}*/

BEGIN_SUNRISE_NAMESPACE

class CmdLineOpts; 


class Option {
public:
	friend class CmdLineOpts; 

	/** \enum type_t
	 *  \briefѡ������ö��
	 */
	enum type_t { 
		string_t=0,	/**< STL string */ 
		int_t,		/**< int  */
		uint_t,		/**< unsigned int  */
		long_t,		/**< long  */
		ulong_t,	/**< unsinged long  */
		double_t,	/**< double  */
		float_t,	/**< float  */
		flag_t,		/**< No argument; bool value is flipped.  */
		func_t,		/**< function  */
		func_one_t,	/**< function with one argument  */
		none_t					
	};

private:
	/** \brief ˽��ȱʡ���캯��
	 */
	Option ();

	/** \brief ȱʡ���캯��
	 */
	Option (char shopt_, const string& lopt_, type_t type_, void* val_);

	/** \brief dumpʵ��״̬����־�ļ�
	 */
	void dump () const;

private:
	/** \brief ����ĸѡ����.
	 */
	char    m_short_name;

	/** \brief ��ѡ����
	 */
	string  m_long_name;

	/** \brief ѡ������
	 */
	type_t  m_type;

	/** \brief ѡ��ֵ
	 */
	void*   m_val;
};

inline 
Option::Option () :
	m_short_name (' '), m_long_name (""), m_type (none_t), m_val (NULL) 
{
	/* empty */
}

inline
Option::Option (char shopt_, const string& lopt_, type_t type_, void* val_) : 
	m_short_name (shopt_), 	m_long_name (lopt_),
	m_type (type_), m_val (val_) 
{
	trace_with_mask("Option::Option", CMDLINEOPTS);
}

/** \class CmdLineOpts CmdLineOpts.h
 * \brief ����������ѡ�
 * ����һ�����࣬Ҫʹ����������дһ���̳�������������. �ɿ����ӳ���
*/
class CmdLineOpts
{
public:
	typedef void (* OPTS_FUNC) (void);
	typedef void (* OPTS_FUNC_ONE) (const string&);

	typedef vector<Option> OptionSet;

	/** \brief ȱʡ���캯��
	 */
	CmdLineOpts ();

	/** \brief ��������
	 */
	virtual ~CmdLineOpts () { 
		trace_with_mask ("CmdLineOpts::~CmdLineOpts", CMDLINEOPTS); 
	}

	/** \brief ����һ������ѡ��.
	   ����һ������ѡ��.
	    @param c  ѡ��Ķ���
	    @param s  ѡ��ĳ���
	    @param f  ����ѡ�������ָ��
	    @return �ɹ�����true, ���򷵻�false.
	*/
	bool add_flag_opt (const char c, const string& s, bool* f);

	/** \brief ����һ��STL stringѡ��
	  ����һ��STL stringѡ��
	    @param c  ѡ��Ķ���
	    @param s  ѡ��ĳ���
	    @param str  stringѡ�����ָ��
	    @return �ɹ�����true, ���򷵻�false.
	*/
	bool add_opt (const char c, const string& s, string* str);

	/** \brief ����һ������ѡ��
	   ����һ������ѡ��
	    @param c  ѡ��Ķ���
	    @param s  ѡ��ĳ���
	    @param i  ����ѡ�����ָ��
	    @return �ɹ�����true, ���򷵻�false.
	*/
	bool add_opt (const char c, const string& s, int* i);

	/** \brief ����һ��unsigned integerѡ��
   	 ����һ��unsigned integerѡ��
	    @param c  ѡ��Ķ���
	    @param s  ѡ��ĳ���
	    @param ui  u_intѡ�����ָ��
	    @return �ɹ�����true, ���򷵻�false.
	*/
	bool add_opt (const char c, const string& s, unsigned int* ui);

	/** \brief ����һ��������ѡ��
	    ����һ��������ѡ��
	    @param c  ѡ��Ķ���
	    @param s  ѡ��ĳ���
	    @param l  ������ѡ�����ָ��
	    @return �ɹ�����true, ���򷵻�false.
	*/
	bool add_opt (const char c, const string& s, long* l);

	/** \brief ����һ��unsigned longѡ��.
	   ����һ��unsigned longѡ��.
	    @param c  ѡ��Ķ���
	    @param s  ѡ��ĳ���
	    @param ul  unsigned longѡ�����ָ��.
	    @return �ɹ�����true, ���򷵻�false.
	*/
	bool add_opt (const char c, const string& s, unsigned long* ul);

	/*1 \brief ����һ��doubleѡ��.
	    ����һ��doubleѡ��.
	    @param c  ѡ��Ķ���
	    @param s  ѡ��ĳ���
	    @param d  doubleѡ�����ָ��.
	    @return �ɹ�����true, ���򷵻�false.
	*/
	bool add_opt (const char c, const string& s, double* d);

	/** \brief ����һ��floatѡ��.
	    ����һ��floatѡ��.
	    @param c  ѡ��Ķ���
	    @param s  ѡ��ĳ���
	    @param f  floatѡ�����ָ��
	    @return �ɹ�����true, ���򷵻�false.
	*/
	bool add_opt (const char c, const string& s, float* f);

	/** \brief ����һ������ѡ��.
	    ����һ������ѡ��.
		 �ڴ���������ѡ��ʱ����f_��ָ��ĺ�����������.

	    @param c_  ѡ��Ķ���
	    @param s_  ѡ��ĳ���
	    @param f_  ����ָ��

	    @return �ɹ�����true, ���򷵻�false.
	*/
	bool add_opt (const char c_, const string& s_, OPTS_FUNC f_);

	/** \brief ����һ��STL stringѡ��.
		 �ڴ���������ѡ��ʱ����f_��ָ��ĺ�����������, ѡ��ֵ��Ϊ�����Ĳ���.

	    @param c_  ѡ��Ķ���
	    @param s_  ѡ��ĳ���
	    @param f_  ����ָ��
	    @return �ɹ�����true, ���򷵻�false.
	*/
	bool add_opt (const char c_, const string& s_, OPTS_FUNC_ONE f_);

	/** \brief �Ƴ�ѡ��
	    �Ƴ�ѡ��
	    @param c_  ѡ��Ķ���
	    @param s_  ѡ��ĳ���
	    @return �ɹ�����true, ���򷵻�false.
	*/
	bool rm_opt (const char c_, const string& s_);

	/** \brief ���������в���
	    ���������в���
	    @return �ɹ�����true, ���򷵻�false.
	*/
	bool parse_args (const char* argv[]);

	/** \brief ȡ������Ϣ(�������parse_argsʧ��)
	   ȡ������Ϣ(�������parse_argsʧ��)
	*/
	const char* get_opt_error () const;

	/** \brief �ַ���ת����argv����
	   ���������ַ���ת��Ϊ��̬���������argv�����ʽ�ı���argv_��.
	 	 ������NULL��β. �ڴ����Ҫ��ʹ������ͷ�.
	    @param src_  �������ַ���
	    @param argc_ ���������ַ������ҵ���ѡ�����
	    @param argv_ �Ӷ��з�����ڴ�, argv����.
	*/
	static void str_to_argv (const string& src_, int&  argc_, char**& argv_);

	/** \brief �ͷŵ���<TT>str_to_argv()</TT>�����ص��ڴ�.	 
	 */
	static void free_argv (char**& argv_);

	/** \brief ��ѡ����Ϣд����־�ļ���.
	 */
	void dump () const;

protected:
	/** \brief ���ѡ��Ϸ���.
	 */
	bool is_valid (const char sopt_, const string& lopt_);
	
	/** \brief ���ô�����Ϣ
	 */
	void set_error_none ();

	/** Perform value assignment to the node. Data conversion  happens here. */
	bool assign (Option* node_, const char* op_[]);

	/** \brief ��ѡ��ж�λѡ��
	 */
	Option* find_option (const char* str_);

	/** \brief ��ѡ��ж�λѡ��
	 */
	Option* find_option (const char letter_);

	/** Process positional argument arg_. This method must be
	    overloaded by the derived class to take advantage of it.
	    @param arg_ positional argument value
	*/
	virtual void pos_arg (const char* arg_);

private:
	OptionSet m_opts_set; /**< ѡ� */

	string    m_error; /**< �������Ĵ�����Ϣ */
};


inline void 
CmdLineOpts::pos_arg (const char* /*arg_*/) { /* no-opt*/ }
	
inline
CmdLineOpts::CmdLineOpts () : m_opts_set (), m_error ("")
{
	trace_with_mask("CmdLineOpts::CmdLineOpts", CMDLINEOPTS);
	set_error_none ();
}

inline void
CmdLineOpts::set_error_none ()
{
	trace_with_mask("CmdLineOpts::set_error_none", CMDLINEOPTS);
	m_error = "";
}

inline const char*
CmdLineOpts::get_opt_error () const
{
	return (m_error.c_str ());
}


END_SUNRISE_NAMESPACE

#endif /* CMD_LINE_OPTS_H */  
