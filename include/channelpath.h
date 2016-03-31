#ifndef _CHANNEL_PATH_H_
#define _CHANNEL_PATH_H_

#include <map>
#include <set>
#include <string>
#include "exception.h"

class Config;

class STPath
{
public:
	STPath();
	~STPath();

public:
	static void CheckPath(std::string& path, bool check_read, bool check_write) throw(Exception);

public:
	void Init(std::string path, std::string sub_path) throw(Exception);
	std::string GetPath();

private:
	std::set<std::string>			m_sPath;
	std::set<std::string>::iterator m_itCurrent;
};

class ChannelPath
{
public:
	ChannelPath(Config* p_cfg);
	~ChannelPath();

public:
	void Init() throw(Exception);
	std::string GetChannelPath(int channel_id);

private:
	void SetDefaultChannel(std::string default_chnn) throw(Exception);
	void SetGeneralChannel(const std::string& general_chnn) throw(Exception);
	void SetMapChannels(std::map<int, STPath>& m_chann);

private:
	Config*					m_pCfg;
	std::string				m_sDefaultChannel;
	std::map<int, STPath>	m_mChnnPath;
	std::map<int, STPath>	m_mGeneralChannel;
};

#endif	// _CHANNEL_PATH_H_

