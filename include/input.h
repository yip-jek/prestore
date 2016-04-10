#ifndef _INPUT_H_
#define _INPUT_H_

#include <string>
#include <map>
#include <deque>
#include <dirent.h>
#include "exception.h"

class Input
{
public:
	Input(const std::string& paths, int packet);
	virtual ~Input() {}

public:
	virtual void Init() throw(Exception) = 0;
	virtual bool GetPacket(std::string& pack) throw(Exception) = 0;
	virtual void DelSrcPacket() = 0;

protected:
	virtual void Close() = 0;

protected:
	std::string m_paths;
	int			m_packets;
};

//////////////////////////////////////////////////////////////////
#ifdef AIX
class TMq;

// Input for MQ
class InputMQ : public Input
{
public:
	InputMQ(const std::string& mq, int packet);
	virtual ~InputMQ();

public:
	virtual void Init() throw(Exception);
	virtual bool GetPacket(std::string& pack) throw(Exception);
	virtual void DelSrcPacket();

protected:
	virtual void Close();

private:
	std::string								m_sMQMgr;
	std::map<std::string, TMq*>				m_mMQQueue;
	std::map<std::string, TMq*>::iterator	m_mIter;
};
#endif

//////////////////////////////////////////////////////////////////
// Input for path
class Dir
{
private:	// noncopyable
	Dir(const Dir& );
	const Dir& operator = (const Dir& );

public:
	explicit Dir(const std::string& path);
	virtual ~Dir();

public:
	void Open() throw(Exception);
	void Close();
	bool IsEmpty() const;
	bool GetFileName(std::string& file_name);
	bool GetFullName(std::string& full_name);

private:
	std::string	m_sPath;
	DIR*		m_pDir;
	bool		m_bEmpty;
};

class InputPath : public Input
{
public:
	InputPath(const std::string& paths, int packet);
	virtual ~InputPath();

public:
	virtual void Init() throw(Exception);
	virtual bool GetPacket(std::string& pack) throw(Exception);
	virtual void DelSrcPacket();

protected:
	virtual void Close();

private:

private:
	std::map<std::string, Dir*>	m_mInputDir;
	std::deque<std::string>		m_qFullName;
};

#endif	// _INPUT_H_

