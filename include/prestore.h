#ifndef _PRESTORE_H_
#define _PRESTORE_H_

#include <string>
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
	void ReleaseInput();

private:
	Config*				m_pCfg;						// The conguration pointer
	int					m_nWaitSecs;				// Sleep seconds in one loop
	int					m_nPackets;					// Process packets in one loop
	Input*				m_pInput;
	std::string			m_sSuspendPath;				// Suspend path
	ChannelPath			m_channelPath;
};

#endif	// _PRESTORE_H_

