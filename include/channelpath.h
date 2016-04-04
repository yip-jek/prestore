#ifndef _CHANNEL_PATH_H_
#define _CHANNEL_PATH_H_

#include <list>
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
	static void CheckPathFile(std::string& path_file, bool is_dir, bool check_read, bool check_write) throw(Exception);

public:
	void Init(std::string path, std::string sub_path) throw(Exception);
	std::string GetPath();

#ifdef DEBUG
public:
#else
private:
#endif
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

#ifdef DEBUG
	void DebugOutput(std::list<std::string>& list_str);
#endif

private:
	void SetDefaultChannel() throw(Exception);
	void SetGeneralChannel() throw(Exception);
	void SetMapChannels(std::map<int, STPath>& m_chann) throw(Exception);
	void SplitChannelIDs(const std::string& chnn_ids, std::list<int>& list_ids) throw(Exception);

private:
	Config*					m_pCfg;
	std::string				m_sDefaultChannel;
	std::map<int, STPath>	m_mChnnPath;
	std::map<int, STPath>	m_mGeneralChannel;
};

#endif	// _CHANNEL_PATH_H_

