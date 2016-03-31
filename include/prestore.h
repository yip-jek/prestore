#ifndef _PRESTORE_H_
#define _PRESTORE_H_

#include <string>
#include <map>
#include <set>
#include "exception.h"
#include "channelpath.h"

class Config;

class Prestore
{
public:
	Prestore(Config& cfg);
	virtual ~Prestore();

	enum INPUT_TYPE
	{
		ITYPE_UNKNOWN	= 0,
		ITYPE_MQ		= 1,
		ITYPE_FILE		= 2
	};

public:
	void Init() throw(Exception);
	void Run() throw(Exception);

private:
	std::string InitCfgPath(const std::string& segment, const std::string& name, 
		bool check_read, bool check_write, bool is_dir) throw(Exception);

#ifdef AIX
	void InitInputMQ(const std::string& paths) throw(Exception);
#endif

	void InitInputPaths(const std::string& paths) throw(Exception);
	void InitChannels() throw(Exception);

private:
	Config*						m_pCfg;						// The conguration pointer
	int							m_nWaitSecs;				// Sleep seconds in one loop
	int							m_nPackets;					// Process packets in one loop
	INPUT_TYPE					m_inputType;				// Input type

#ifdef AIX
	std::string					m_sMQMgr;					// MQ Manager
#endif

	std::set<std::string>		m_sInputPaths;				// Input path(s)
	bool						m_bGeneral;					// Is general_file invalid ?
	std::string					m_sGeneralFile;				// General file path
	int							m_nTotalChannels;			// Total channels
	std::string 				m_sDefaultChannel;			// Default channel path
	std::string					m_sSuspendPath;				// Suspend path
	//std::map<int, std::string>	m_mChannels;				// The list of channels
	ChannelPath					m_channelPath;
};

#endif	// _PRESTORE_H_

