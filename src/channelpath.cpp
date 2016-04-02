#include "channelpath.h"
#include "def.h"
#include "config.h"
#include "helper.h"
#include <iostream>

STPath::STPath()
:m_itCurrent(m_sPath.end())
{
}

STPath::~STPath()
{
}

void STPath::CheckPathFile(std::string& path_file, bool is_dir, bool check_read, bool check_write) throw(Exception)
{
	if ( path_file.empty() )
	{
		throw Exception(CP_PATH_FILE_INVALID, "The path_file is blank!"); 
	}

	if ( is_dir )
	{
		if ( !Helper::IsDirectory(path_file) )
		{
			throw Exception(CP_PATH_FILE_INVALID, "The path \""+path_file+"\" is not valid!"); 
		}
	}
	else
	{
		if ( !Helper::IsRegularFile(path_file) )
		{
			throw Exception(CP_PATH_FILE_INVALID, "The file \""+path_file+"\" is not valid!"); 
		}
	}

	if ( check_read && !Helper::CheckReadPermission(path_file) )
	{
		throw Exception(PS_PERMISSION_DENIED, "The path_file \""+path_file+"\" read permission denied!");
	}

	if ( check_write && !Helper::CheckWritePermission(path_file) )
	{
		throw Exception(PS_PERMISSION_DENIED, "The path_file \""+path_file+"\" write permission denied!");
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
				throw Exception(CP_PATH_FILE_INVALID, "The sub_path \""+sub_path+"\" duplication!");
			}

			m_sPath.insert(sub_path);
		}
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
		throw Exception(PS_CONFIG_INVALID, "[CHANNEL_PATH] The configuration pointer is empty!");
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

#ifdef DEBUG
std::string ChannelPath::DebugOutput()
{
	std::string output;

	std::cout << "m_mChnnPath.size() = " << m_mChnnPath.size() << std::endl;

	if ( m_mChnnPath.empty() )
	{
		output += "[CHANNELS = 0] CHANNEL is empty!\n";
	}
	else
	{
		for ( std::map<int, STPath>::iterator it = m_mChnnPath.begin(); it != m_mChnnPath.end(); ++it )
		{
			int id = it->first;
			STPath& stp = it->second;

			for ( std::set<std::string>::iterator s_it = stp.m_sPath.begin(); s_it != stp.m_sPath.end(); ++s_it )
			{
				output += "Channel " + Helper::Num2Str(id) + ": [" + *s_it + "]\n";
			}
		}
	}

	std::cout << "m_mGeneralChannel.size() = " << m_mGeneralChannel.size() << std::endl;

	if ( m_mGeneralChannel.empty() )
	{
		output += "[GENERAL CHANNELS = 0] GENERAL CHANNEL is empty!\n";
	}
	else
	{
		for ( std::map<int, STPath>::iterator it = m_mGeneralChannel.begin(); it != m_mGeneralChannel.end(); ++it )
		{
			int id = it->first;
			STPath& stp = it->second;

			for ( std::set<std::string>::iterator s_it = stp.m_sPath.begin(); s_it != stp.m_sPath.end(); ++s_it )
			{
				output += "General_channel " + Helper::Num2Str(id) + ": [" + *s_it + "]\n";
			}
		}
	}

	std::cout << "Default_channel: " << m_sDefaultChannel << std::endl;

	output += "Default_channel: [" + m_sDefaultChannel + "]\n";

	return output;
}
#endif

void ChannelPath::SetMapChannels(std::map<int, STPath>& m_chann) throw(Exception)
{
	const int CHANNELS = (int)m_pCfg->GetCfgLongVal("COMMON", "CHANNELS");
	if ( CHANNELS < 0 )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The [COMMON->CHANNELS] configuration is invalid!");
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
				throw Exception(CP_CHANNEL_ID_INVALID, "The channel_id ["+Helper::Num2Str(*it)+"] is not valid!");
			}

			if ( m_chann.find(*it) != m_chann.end() )
			{
				throw Exception(CP_CHANNEL_ID_INVALID, "The channel_id ["+Helper::Num2Str(*it)+"] duplication!");
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
		throw Exception(CP_CHANNEL_ID_INVALID, "The [CHANNEL->ID] is blank!");
	}

	std::list<std::string> sp_list;
	Helper::SplitStr(chnn_ids, ",", sp_list, true);

	std::list<std::string> sp2list;
	std::list<std::string>::iterator sp2_it;
	for ( std::list<std::string>::iterator it = sp_list.begin(); it != sp_list.end(); ++it )
	{
		if ( it->empty() )
		{
			throw Exception(CP_CHANNEL_ID_INVALID, "There is a blank in the [CHANNEL->ID] configuration: "+chnn_ids);
		}

		sp2list.clear();
		Helper::SplitStr(*it, "-", sp2list, true);

		size_t size = sp2list.size();
		sp2_it = sp2list.begin();
		if ( 1 == size )
		{
			if ( !Helper::IsAllNumber(*sp2_it) )
			{
				throw Exception(CP_CHANNEL_ID_INVALID, "There is invalid character(s) in the [CHANNEL->ID] configuration: "+chnn_ids);
			}

			list_ids.push_back(Helper::Str2Int(*sp2_it));
		}
		else if ( 2 == size )
		{
			if ( !Helper::IsAllNumber(*sp2_it) )
			{
				throw Exception(CP_CHANNEL_ID_INVALID, "There is invalid character(s) in the [CHANNEL->ID] configuration: "+chnn_ids);
			}

			int l_id = Helper::Str2Int(*sp2_it);

			if ( !Helper::IsAllNumber(*(++sp2_it)) )
			{
				throw Exception(CP_CHANNEL_ID_INVALID, "There is invalid character(s) in the [CHANNEL->ID] configuration: "+chnn_ids);
			}

			int r_id = Helper::Str2Int(*sp2_it);

			if ( l_id >= r_id )
			{
				throw Exception(CP_CHANNEL_ID_INVALID, "The [CHANNEL->ID] configuration \""+chnn_ids+"\" is invalid!");
			}

			for ( int i = l_id; i <= r_id; ++i )
			{
				list_ids.push_back(i);
			}
		}
		else
		{
			throw Exception(CP_CHANNEL_ID_INVALID, "The [CHANNEL->ID] configuration \""+chnn_ids+"\" is invalid!");
		}
	}
}

