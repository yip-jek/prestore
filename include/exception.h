#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <string>

class Exception
{
public:
	Exception(): m_errorcode(0)
	{}
	Exception(int error, const std::string& descript): m_errorcode(error), m_description(descript)
	{}
	virtual ~Exception() {}

public:
	void ErrorCode(int error)
	{ m_errorcode = error; }

	int ErrorCode() const
	{ return m_errorcode; }

	void Descript(const std::string& descript)
	{ m_description = descript; }

	std::string What() const
	{ return m_description; }

private:
	int         m_errorcode;
	std::string m_description;
};

#endif

