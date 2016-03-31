#ifndef _CHANNEL_PATH_H_
#define _CHANNEL_PATH_H_

#include <map>
#include <set>
#include <string>
#include "exception.h"

class STPath
{
public:
	STPath();
	~STPath();

public:
	void Init(const std::string& path, const std::string& sub_path) throw(Exception);
	std::string GetPath();

private:
	std::set<std::string>			m_sPath;
	std::set<std::string>::iterator m_itCurrent;
};

class ChannelPath
{
public:
	ChannelPath();
	~ChannelPath();

public:
	void AddChannelPath() throw(Exception);
	std::string GetChannelPath(int channel_id);

private:
	std::string				m_sDefaultChannel;
	std::map<int, STPath>	m_mChnnPath;
	std::map<int, STPath>	m_mGeneralChannel;
};

#endif	// _CHANNEL_PATH_H_

