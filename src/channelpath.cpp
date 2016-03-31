#include "channelpath.h"
#include "def.h"
#include "config.h"

STPath::STPath()
:m_itCurrent(m_sPath.end())
{
}

STPath::~STPath()
{
}

void STPath::CheckPath(std::string& path, bool check_read, bool check_write) throw(Exception)
{
	if ( path.empty() )
	{
		throw Exception(CP_PATH_INVALID, "The path is blank!"); 
	}

	if ( !Helper::IsDirectory(path) )
	{
		throw Exception(CP_PATH_INVALID, "The path \""+path+"\" is not valid!"); 
	}

	if ( check_read && !Helper::CheckReadPermission(path) )
	{
		throw Exception(PS_PERMISSION_DENIED, "The path \""+path+"\" read permission denied!");
	}

	if ( check_write && !Helper::CheckWritePermission(path) )
	{
		throw Exception(PS_PERMISSION_DENIED, "The path \""+path+"\" write permission denied!");
	}

	Helper::AddDirSlash(path);
}

void STPath::Init(std::string path, std::string sub_path) throw(Exception)
{
	m_sPath.clear();
	m_itCurrent = m_sPath.end();

	CheckPath(path, true, true);

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

			CheckPath(sub_path, true, true);

			if ( m_sPath.find(sub_path) != m_sPath.end() )
			{
				throw Exception(CP_PATH_INVALID, "The sub_path \""+sub_path+"\" duplication!");
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

void ChannelPath::SetDefaultChannel(std::string default_chnn) throw(Exception)
{
	try
	{
		STPath::CheckPath(default_chnn);
	}
	catch ( Exception& ex )
	{
		ex.Descript("[DEFAULT_CHANNEL] "+ex.What());
		throw ex;
	}

	m_sDefaultChannel = default_chnn;
}

void ChannelPath::SetGeneralChannel(const std::string& general_chnn) throw(Exception)
{
	Config cfg;
	cfg.SetCfgFile(general_chnn);

	try
	{
		SetMapChannels(m_mGeneralChannel);
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

	SetMapChannels(m_mChnnPath);
	SetGeneralChannel()
}

std::string ChannelPath::GetChannelPath(int channel_id)
{
	std::map<int, STPath>::iterator it_find;

	// First find in m_mChnnPath
	if ( (it_find = m_mChnnPath.find(channel_id)) != m_mChnnPath.end() )
	{
		return it_find->GetPath();
	}

	// Second find in [GENERAL_CHANNEL]
	if ( (it_find = m_mGeneralChannel.find(channel_id)) != m_mGeneralChannel.end() )
	{
		return it_find->GetPath();
	}

	// Last get [DEFAULT_CHANNEL]
	return m_sDefaultChannel;
}

void SetMapChannels(Config* p_cfg, std::map<int, STPath>& m_chann);
{
	p_cfg->DeleteItems();

	p_cfg->RegisterItem("COMMON", "CHANNELS");

	p_cfg->ReadConfig();

	int channels = (int)p_cfg->GetCfgLongVal("COMMON", "CHANNELS");
	if ( channels < 0 )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "[COMMON->CHANNELS] configuration is invalid!");
	}

	m_chann.clear();
	// When "channels = 0", do nothing!
	if ( channels > 0 )
	{
	}
}

