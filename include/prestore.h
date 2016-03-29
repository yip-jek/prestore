#ifndef _PRESTORE_H_
#define _PRESTORE_H_

#include <string>
#include <map>
#include "exception.h"

class Config;

class Prestore
{
public:
	Prestore(Config& cfg);
	virtual ~Prestore();

public:
	void Init() throw(Exception);
	void Run() throw(Exception);

private:
	void InitChannels() throw(Exception);

private:
	Config*						m_pCfg;						// The conguration pointer
	int							m_nWaitSecs;				// Sleep seconds in one loop
	int							m_nPackets;					// Process packets in one loop
	bool						m_bGeneral;					// Is general_file invalid ?
	std::string					m_sGeneralFile;				// General file path
	int							m_nTotalChannels;			// Total channels
	std::string 				m_sDefaultChannel;			// Default channel path
	std::map<int, std::string>	m_mChannels;				// The list of channels
};

#endif	// _PRESTORE_H_

