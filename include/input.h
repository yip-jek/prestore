#ifndef _INPUT_H_
#define _INPUT_H_

#include <string>
#include <set>
#include <dirent.h>
#include "exception.h"

class Input
{
public:
	Input(const std::string& paths);
	virtual ~Input();

public:
	virtual void Init() throw(Exception) = 0;
	virtual bool GetPacket(std::string& pack) = 0;
	virtual void DelSrcPacket() = 0;

protected:
	virtual void Close() = 0;

protected:
	std::string m_paths;
};

//////////////////////////////////////////////////////////////////
#ifdef AIX
// Input for MQ
class InputMQ : public Input
{
public:
	InputMQ(const std::string& paths);
	virtual ~InputMQ();

public:
	virtual void Init() throw(Exception);
	virtual bool GetPacket(std::string& pack);
	virtual void DelSrcPacket();

protected:
	virtual void Close();

private:
	std::string						m_sMQMgr;
	std::set<std::string>			m_sMQQueue;
	std::set<std::string>::iterator	m_sIter;
};
#endif

//////////////////////////////////////////////////////////////////
// Input for path
class InputPath : public Input
{
public:
	InputPath(const std::string& paths);
	virtual ~InputPath();

public:
	virtual void Init() throw(Exception);
	virtual bool GetPacket(std::string& pack);
	virtual void DelSrcPacket();

protected:
	virtual void Close();

private:
	std::set<std::string>			m_sInputDir;
	std::set<std::string>::iterator	m_sIter;
	std::string						m_sFullName;
};

#endif	// _INPUT_H_

