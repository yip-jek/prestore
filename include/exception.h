#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <string>
#include <stdarg.h>

class Exception
{
public:
	Exception(): m_errorcode(0)
	{}
	Exception(int error, const std::string& descript): m_errorcode(error), m_description(descript)
	{}
	Exception(int error, const char* fmt, ...): m_errorcode(error)
	{
		char buf[2048] = "";

		va_list ap;
		va_start(ap, fmt);
		vsnprintf(buf, sizeof(buf), fmt, ap);
		va_end(ap);

		m_description = buf;
	}
	virtual ~Exception() {}

public:
	void ErrorCode(int error)
	{ m_errorcode = error; }

	int ErrorCode() const
	{ return m_errorcode; }

	void Descript(const std::string& descript)
	{ m_description = descript; }

	void ErrorInfo(int error, const char* fmt, ...)
	{
		m_errorcode = error;

		char buf[2048] = "";

		va_list ap;
		va_start(ap, fmt);
		vsnprintf(buf, sizeof(buf), fmt, ap);
		va_end(ap);

		m_description = buf;
	}

	std::string What() const
	{ return m_description; }

private:
	int         m_errorcode;
	std::string m_description;
};

#endif

