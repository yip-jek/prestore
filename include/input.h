#ifndef _INPUT_H_
#define _INPUT_H_

#include <string>
#include <map>
#include <set>
#include <deque>
#include <dirent.h>
#include "exception.h"

class CZip;
class Packet;

class Input
{
public:
	Input(const std::string& paths, int packet);
	virtual ~Input();

public:
	virtual void Init() throw(Exception) = 0;
	virtual bool GetPacket(Packet* p) throw(Exception) = 0;
	virtual void DelSrcPacket() = 0;

protected:
	virtual void Close() = 0;

protected:
	std::string m_paths;
	int			m_packets;
	CZip*		m_pZip;
};

//////////////////////////////////////////////////////////////////
#ifdef AIX
class TMq;

class AutoCloseMQ
{
public:
	AutoCloseMQ(TMq* pMq);
	~AutoCloseMQ();

private:
	TMq*	m_pMq;
};

// Input for MQ
class InputMQ : public Input
{
public:
	InputMQ(const std::string& mq, int packet);
	virtual ~InputMQ();

	static const int MQ_WAIT_SEC = 3;

public:
	virtual void Init() throw(Exception);
	virtual bool GetPacket(Packet* p) throw(Exception);
	virtual void DelSrcPacket();

protected:
	virtual void Close();

private:
	void NextIter(std::set<std::string>::iterator& it);
	bool GetMQMsg(const std::string& q_name, Packet* p);

private:
	std::string						m_sMQMgr;
	std::set<std::string>			m_sMQQueue;
	std::set<std::string>::iterator	m_sIter;
	TMq*							m_pMQ;
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

class AutoFile
{
public:
	AutoFile();
	~AutoFile();

public:
	bool Open(const std::string& file);
	void Close();
	bool GetSize(const std::string& file, int& size);
	bool Read(char* pBuf, int file_size);

private:
	FILE*	m_fp;
};

class InputPath : public Input
{
public:
	InputPath(const std::string& paths, int packet);
	virtual ~InputPath();

public:
	virtual void Init() throw(Exception);
	virtual bool GetPacket(Packet* p) throw(Exception);
	virtual void DelSrcPacket();

protected:
	virtual void Close();

private:
	bool GetFile(std::string& file_name);

private:
	std::map<std::string, Dir*>	m_mInputDir;
	std::deque<std::string>		m_qFullName;
};

#endif	// _INPUT_H_

