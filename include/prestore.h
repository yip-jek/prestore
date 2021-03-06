#ifndef _PRESTORE_H_
#define _PRESTORE_H_

#include <string>
#include "exception.h"
#include "channelpath.h"
#include "task.h"

class Config;
class Input;

class Packet
{
private:	// noncopyable
	Packet(const Packet& );
	const Packet& operator = (const Packet& );

public:
	Packet();
	~Packet();

	static size_t _ZIP_MAX_SIZE;
	static size_t _UNCOMPRESS_MAX_SIZE;

public:
	void Init();
	void BillPeriodZero();
	void ZipBufZero();
	void UncomBufZero();

public:
	std::string	m_srcFilePath;

	int			m_nFileID;
	int			m_nChannelID1;
	int			m_nChannelID2;
	char		m_sBillPeriod[6+1];

	char*		m_pZipBuf;
	int			m_nZipSize;
	char*		m_pUncomBuf;
	int			m_nUncomSize;
};


//////////////////////////////////////////////////////////////////////////////////////
class AutoTask
{
public:
	AutoTask(Task* p_new_task = NULL);
	~AutoTask();

public:
	Task* Get() { return m_pTask; }

	void Reset(Task* p_new_task);

private:
	void Release();

private:
	Task*	m_pTask;
};

//////////////////////////////////////////////////////////////////////////////////////
class Prestore
{
public:
	Prestore(Config& cfg, AutoTask& auto_task);
	virtual ~Prestore();

public:
	void Init() throw(Exception);
	void Run() throw(Exception);

private:
	void Release();
	void SuspendPacket(Packet* p, long error) throw(Exception);
	void DistributePacket(Packet* p) throw(Exception);
	void WriteFile(const std::string& path, const std::string& file_name, char* pBuf, int buf_size) throw(Exception);
	void TryCreateDir(const std::string& dir_path) throw(Exception);
	bool Running();

private:
	Config*				m_pCfg;						// The conguration pointer
	int					m_nWaitSecs;				// Sleep seconds in one loop
	int					m_nPackets;					// Process packets in one loop
	int                 m_nMaxTypeID;
	Input*				m_pInput;
	std::string			m_sSuspendPath;				// Suspend path
	ChannelPath			m_channelPath;
	AutoTask*			m_pAutoTask;
};

#endif	// _PRESTORE_H_

