#ifndef _PRESTORE_H_
#define _PRESTORE_H_

#include <string>
#include <map>
#include "exception.h"
#include "channelpath.h"

class Config;
class Input;

class Prestore
{
public:
	Prestore(Config& cfg);
	virtual ~Prestore();

public:
	void Init() throw(Exception);
	void Run() throw(Exception);

private:
	void InitInputPaths(const std::string& paths) throw(Exception);

#ifdef AIX
	void InitInputMQ(const std::string& paths) throw(Exception);
#endif

private:
	Config*						m_pCfg;						// The conguration pointer
	int							m_nWaitSecs;				// Sleep seconds in one loop
	int							m_nPackets;					// Process packets in one loop
	Input*						m_pInput;

#ifdef AIX
	std::string					m_sMQMgr;					// MQ Manager
#endif

	std::set<std::string>		m_sInputPaths;				// Input
	std::set<DIR*>				m_sInputDIR;				// Input DIR(s)
	std::string					m_sSuspendPath;				// Suspend path
	ChannelPath					m_channelPath;
};

#endif	// _PRESTORE_H_

