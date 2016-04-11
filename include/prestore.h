#ifndef _PRESTORE_H_
#define _PRESTORE_H_

#include <string>
#include "exception.h"
#include "channelpath.h"

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

	static const size_t ZIP_MAX_SIZE = 4194304;
	static const size_t UNCOMPRESS_MAX_SIZE = 16777216;

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
class Prestore
{
public:
	Prestore(Config& cfg);
	virtual ~Prestore();

public:
	void Init() throw(Exception);
	void Run() throw(Exception);

private:
	void ReleaseInput();
	void SuspendPacket(Packet* p, long error) throw(Exception);
	void DistributePacket(Packet* p) throw(Exception);
	void WriteFile(const std::string& file_name, char* pBuf, int buf_size) throw(Exception);

private:
	Config*				m_pCfg;						// The conguration pointer
	int					m_nWaitSecs;				// Sleep seconds in one loop
	int					m_nPackets;					// Process packets in one loop
	int                 m_nMaxTypeID;
	Input*				m_pInput;
	std::string			m_sSuspendPath;				// Suspend path
	ChannelPath			m_channelPath;
};

#endif	// _PRESTORE_H_

