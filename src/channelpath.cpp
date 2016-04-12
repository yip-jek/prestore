#include "channelpath.h"
#include <algorithm>
#include "def.h"
#include "config.h"
#include "helper.h"

STPath::STPath()
:m_itCurrent(m_sPath.end())
{
}

STPath::STPath(const STPath& stp)
:m_itCurrent(m_sPath.end())
{
	InitCopy(stp.m_sPath);
}

STPath::~STPath()
{
}

const STPath& STPath::operator = (const STPath& stp)
{
	if ( this != &stp )
	{
		InitCopy(stp.m_sPath);

		this->m_itCurrent = this->m_sPath.end();
	}

	return *this;
}

void STPath::CheckPathFile(std::string& path_file, bool is_dir, bool check_read, bool check_write) throw(Exception)
{
	if ( path_file.empty() )
	{
		throw Exception(CP_PATH_FILE_INVALID, "The path_file is blank! [FILE:%s, LINE:%d]", __FILE__, __LINE__); 
	}

	if ( is_dir )
	{
		if ( !Helper::IsDirectory(path_file) )
		{
			throw Exception(CP_PATH_FILE_INVALID, "The path \"%s\" is not valid! [FILE:%s, LINE:%d]", path_file.c_str(), __FILE__, __LINE__);
		}
	}
	else
	{
		if ( !Helper::IsRegularFile(path_file) )
		{
			throw Exception(CP_PATH_FILE_INVALID, "The file \"%s\" is not valid! [FILE:%s, LINE:%d]", path_file.c_str(), __FILE__, __LINE__);
		}
	}

	if ( check_read && !Helper::CheckReadPermission(path_file) )
	{
		throw Exception(PS_PERMISSION_DENIED, "The path_file \"%s\" read permission denied! [FILE:%s, LINE:%d]", path_file.c_str(), __FILE__, __LINE__);
	}

	if ( check_write && !Helper::CheckWritePermission(path_file) )
	{
		throw Exception(PS_PERMISSION_DENIED, "The path_file \"%s\" write permission denied! [FILE:%s, LINE:%d]", path_file.c_str(), __FILE__, __LINE__);
	}

	if ( is_dir )
	{
		Helper::AddDirSlash(path_file);
	}
}

void STPath::Init(std::string path, std::string sub_path) throw(Exception)
{
	m_sPath.clear();
	m_itCurrent = m_sPath.end();

	CheckPathFile(path, true, true, true);

	if ( sub_path.empty() )
	{
		m_sPath.insert(path);
	}
	else
	{
		std::list<std::string> list_subpath;
		Helper::SplitStr(sub_path, ",", list_subpath, true);

		for ( std::list<std::string>::iterator it = list_subpath.begin(); it != list_subpath.end(); ++it )
		{
			sub_path = path + (*it);

			CheckPathFile(sub_path, true, true, true);

			if ( m_sPath.find(sub_path) != m_sPath.end() )
			{
				throw Exception(CP_PATH_FILE_INVALID, "The sub_path \"%s\" duplication! [FILE:%s, LINE:%d]", sub_path.c_str(), __FILE__, __LINE__);
			}

			m_sPath.insert(sub_path);
		}
	}
}

void STPath::InitCopy(const std::set<std::string>& set_path)
{
	m_sPath.clear();

	for ( std::set<std::string>::const_iterator c_it = set_path.begin(); c_it != set_path.end(); ++c_it )
	{
		m_sPath.insert(*c_it);
	}
}

std::string STPath::GetPath()
{
	if ( m_sPath.empty() )
	{
		return std::string();
	}

	if ( m_sPath.end() == m_itCurrent )
	{
		m_itCurrent = m_sPath.begin();
	}

	return *(m_itCurrent++);
}


////////////////////////////////////////////////////////////////////////////////////////////////
ChannelPath::ChannelPath(Config* p_cfg)
:m_pCfg(p_cfg)
{
}

ChannelPath::~ChannelPath()
{
}

void ChannelPath::SetDefaultChannel() throw(Exception)
{
	try
	{
		m_sDefaultChannel = m_pCfg->GetCfgValue("COMMON", "DEFAULT_CHANNEL");
		STPath::CheckPathFile(m_sDefaultChannel, true, true, true);
	}
	catch ( Exception& ex )
	{
		ex.Descript("[DEFAULT_CHANNEL] "+ex.What());
		throw ex;
	}
}

void ChannelPath::SetGeneralChannel() throw(Exception)
{
	try
	{
		bool general = m_pCfg->GetCfgBoolVal("COMMON", "GENERAL");

		if ( general )
		{
			std::string general_file = m_pCfg->GetCfgValue("COMMON", "GENERAL_FILE");
			STPath::CheckPathFile(general_file, false, true, false);

			m_pCfg->DeleteItems();
			m_pCfg->SetCfgFile(general_file);
			m_pCfg->RegisterItem("COMMON", "CHANNELS");
			m_pCfg->ReadConfig();

			SetMapChannels(m_mGeneralChannel);
		}
		else
		{
			m_mGeneralChannel.clear();
		}
	}
	catch ( Exception& ex )
	{
		ex.Descript("[GENERAL] "+ex.What());
		throw ex;
	}
}

void ChannelPath::Init() throw(Exception)
{
	if ( NULL == m_pCfg )
	{
		throw Exception(PS_CONFIG_INVALID, "[CHANNEL_PATH] The configuration pointer is empty! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	m_pCfg->RegisterItem("COMMON", "DEFAULT_CHANNEL");
	m_pCfg->RegisterItem("COMMON", "CHANNELS");
	m_pCfg->RegisterItem("COMMON", "GENERAL");
	m_pCfg->RegisterItem("COMMON", "GENERAL_FILE");

	m_pCfg->ReadConfig();

	SetDefaultChannel();
	SetMapChannels(m_mChnnPath);
	SetGeneralChannel();
}

std::string ChannelPath::GetChannelPath(int channel_id)
{
	std::map<int, STPath>::iterator it_find;

	// First find in m_mChnnPath
	if ( (it_find = m_mChnnPath.find(channel_id)) != m_mChnnPath.end() )
	{
		return it_find->second.GetPath();
	}

	// Second find in [GENERAL_CHANNEL]
	if ( (it_find = m_mGeneralChannel.find(channel_id)) != m_mGeneralChannel.end() )
	{
		return it_find->second.GetPath();
	}

	// Last get [DEFAULT_CHANNEL]
	return m_sDefaultChannel;
}

void ChannelPath::SetMapChannels(std::map<int, STPath>& m_chann) throw(Exception)
{
	const int CHANNELS = (int)m_pCfg->GetCfgLongVal("COMMON", "CHANNELS");
	if ( CHANNELS < 0 )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The [COMMON->CHANNELS] configuration is invalid! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	m_chann.clear();

	// When "CHANNELS = 0", do nothing!
	if ( 0 == CHANNELS )
	{
		return;
	}

	// CHANNELS > 0
	std::string channel_segment;
	for ( int i = 0; i < CHANNELS; ++i )
	{
		channel_segment = "CHANNEL_" + Helper::Num2Str(i+1);
		m_pCfg->RegisterItem(channel_segment, "ID");
		m_pCfg->RegisterItem(channel_segment, "PATH");
		m_pCfg->RegisterItem(channel_segment, "SUB_PATH");
	}

	m_pCfg->ReadConfig();

	std::string channel_ids;
	std::string path;
	std::string sub_path;
	std::list<int> list_ids;
	STPath stp;
	for ( int i = 0; i < CHANNELS; ++i )
	{
		channel_segment = "CHANNEL_" + Helper::Num2Str(i+1);

		channel_ids = m_pCfg->GetCfgValue(channel_segment, "ID");
		SplitChannelIDs(channel_ids, list_ids);

		path = m_pCfg->GetCfgValue(channel_segment, "PATH");

		// Ignore Exception!
		try
		{
			sub_path = m_pCfg->GetCfgValue(channel_segment, "SUB_PATH");
		}
		catch ( Exception& ex )
		{
			sub_path.clear();
		}

		stp.Init(path, sub_path);

		for ( std::list<int>::iterator it = list_ids.begin(); it != list_ids.end(); ++it )
		{
			if ( *it < 0 )
			{
				throw Exception(CP_CHANNEL_ID_INVALID, "The channel_id [%d] is not valid! [FILE:%s, LINE:%d]", *it, __FILE__, __LINE__);
			}

			if ( m_chann.find(*it) != m_chann.end() )
			{
				throw Exception(CP_CHANNEL_ID_INVALID, "The channel_id [%d] duplication! [FILE:%s, LINE:%d]", *it, __FILE__, __LINE__);
			}

			m_chann[*it] = stp;
		}
	}
}

void ChannelPath::SplitChannelIDs(const std::string& chnn_ids, std::list<int>& list_ids) throw(Exception)
{
	list_ids.clear();

	if ( chnn_ids.empty() )
	{
		throw Exception(CP_CHANNEL_ID_INVALID, "The [CHANNEL->ID] is blank! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	std::list<std::string> sp_list;
	Helper::SplitStr(chnn_ids, ",", sp_list, true);

	std::list<std::string> sp2list;
	std::list<std::string>::iterator sp2_it;
	for ( std::list<std::string>::iterator it = sp_list.begin(); it != sp_list.end(); ++it )
	{
		if ( it->empty() )
		{
			throw Exception(CP_CHANNEL_ID_INVALID, "There is a blank in the [CHANNEL->ID] configuration: %s [FILE:%s, LINE:%d]", chnn_ids.c_str(), __FILE__, __LINE__);
		}

		sp2list.clear();
		Helper::SplitStr(*it, "-", sp2list, true);

		size_t size = sp2list.size();
		sp2_it = sp2list.begin();
		if ( 1 == size )
		{
			if ( !Helper::IsAllNumber(*sp2_it) )
			{
				throw Exception(CP_CHANNEL_ID_INVALID, "There is invalid character(s) in the [CHANNEL->ID] configuration: %s [FILE:%s, LINE:%d]", chnn_ids.c_str(), __FILE__, __LINE__);
			}

			list_ids.push_back(Helper::Str2Int(*sp2_it));
		}
		else if ( 2 == size )
		{
			if ( !Helper::IsAllNumber(*sp2_it) )
			{
				throw Exception(CP_CHANNEL_ID_INVALID, "There is invalid character(s) in the [CHANNEL->ID] configuration: %s [FILE:%s, LINE:%d]", chnn_ids.c_str(), __FILE__, __LINE__);
			}

			int l_id = Helper::Str2Int(*sp2_it);

			if ( !Helper::IsAllNumber(*(++sp2_it)) )
			{
				throw Exception(CP_CHANNEL_ID_INVALID, "There is invalid character(s) in the [CHANNEL->ID] configuration: %s [FILE:%s, LINE:%d]", chnn_ids.c_str(), __FILE__, __LINE__);
			}

			int r_id = Helper::Str2Int(*sp2_it);

			if ( l_id >= r_id )
			{
				throw Exception(CP_CHANNEL_ID_INVALID, "The [CHANNEL->ID] configuration \"%s\" is invalid! [FILE:%s, LINE:%d]", chnn_ids.c_str(), __FILE__, __LINE__);
			}

			for ( int i = l_id; i <= r_id; ++i )
			{
				list_ids.push_back(i);
			}
		}
		else
		{
			throw Exception(CP_CHANNEL_ID_INVALID, "The [CHANNEL->ID] configuration \"%s\" is invalid! [FILE:%s, LINE:%d]", chnn_ids.c_str(), __FILE__, __LINE__);
		}
	}
}

