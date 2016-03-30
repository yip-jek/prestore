#include "prestore.h"
#include <list>
#include <unistd.h>
#include "gsignal.h"
#include "config.h"
#include "log.h"
#include "helper.h"
#include "def.h"

Prestore::Prestore(Config& cfg)
:m_pCfg(&cfg)
,m_nWaitSecs(0)
,m_nPackets(0)
,m_inputType(ITYPE_UNKNOWN)
,m_bGeneral(false)
,m_nTotalChannels(0)
{
}

Prestore::~Prestore()
{
}

void Prestore::Init() throw(Exception)
{
	if ( NULL == m_pCfg )
	{
		throw Exception(PS_CONFIG_INVALID, "The configuration pointer is empty!");
	}

	m_pCfg->RegisterItem("SYS", "WAIT_SEC");
	m_pCfg->RegisterItem("SYS", "PACKETS");
	m_pCfg->RegisterItem("INPUT", "INPUT_TYPE");
	m_pCfg->RegisterItem("INPUT", "INPUT_PATH");
	m_pCfg->RegisterItem("COMMON", "GENERAL");
	m_pCfg->RegisterItem("COMMON", "GENERAL_FILE");
	m_pCfg->RegisterItem("COMMON", "CHANNELS");
	m_pCfg->RegisterItem("COMMON", "DEFAULT_CHANNEL");
	m_pCfg->RegisterItem("COMMON", "SUSPEND_PATH");

	m_pCfg->ReadConfig();

	m_nWaitSecs = (int)m_pCfg->GetCfgLongVal("SYS", "WAIT_SEC");
	if ( m_nWaitSecs < 0 )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The [SYS->WAIT_SEC] configuration is invalid!");
	}

	m_nPackets = (int)m_pCfg->GetCfgLongVal("SYS", "PACKETS");
	if ( m_nPackets <= 0 )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The [SYS->PACKETS] configuration is invalid!");
	}

	std::string input_paths = m_pCfg->GetCfgValue("INPUT", "INPUT_PATH");

	int type = (int)m_pCfg->GetCfgLongVal("INPUT", "INPUT_TYPE");
	if ( 1 == type )
	{
#ifdef AIX
		m_inputType = ITYPE_MQ;
		InitInputMQ(input_paths);
#else
		throw Exception(PS_UNSUPPORT_INPUTTYPE, "Unsupport \"MQ\" input type in non-AIX environment!");
#endif
	}
	else if ( 2 == type )
	{
		m_inputType = ITYPE_FILE;
		InitInputPaths(input_paths);
	}
	else
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The [INPUT->INPUT_TYPE] configuration is invalid!");
	}

	m_bGeneral = m_pCfg->GetCfgBoolVal("COMMON", "GENERAL");
	if ( m_bGeneral )		// GENERAL = TRUE
	{
		m_sGeneralFile = InitCfgPath("COMMON", "GENERAL_FILE", true, false, false);
	}

	m_nTotalChannels = (int)m_pCfg->GetCfgLongVal("COMMON", "CHANNELS");
	if ( m_nTotalChannels < 0 )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The [COMMON->CHANNELS] configuration is invalid!");
	}

	m_sDefaultChannel = InitCfgPath("COMMON", "DEFAULT_CHANNEL", false, true, true);

	m_sSuspendPath = InitCfgPath("COMMON", "SUSPEND_PATH", false, true, true);

	InitChannels();
}

void Prestore::Run() throw(Exception)
{
#ifdef DEBUG
	Log::Instance()->Output("WAIT_SECS=%d", m_nWaitSecs);
	Log::Instance()->Output("PACKETS=%d", m_nPackets);
	Log::Instance()->Output("GENERAL=%d", m_bGeneral);
	Log::Instance()->Output("GENERAL_FILE=%s", m_sGeneralFile.c_str());
	Log::Instance()->Output("TOTAL_CHANNELS=%d", m_nTotalChannels);
	Log::Instance()->Output("DEFAULT_CHANNEL=%s", m_sDefaultChannel.c_str());
	Log::Instance()->Output("SUSPEND_PATH=%s", m_sSuspendPath.c_str());

	switch ( m_inputType )
	{
	case ITYPE_MQ:
		Log::Instance()->Output("INPUT_TYPE=MQ");
		break;
	case ITYPE_FILE:
		Log::Instance()->Output("INPUT_TYPE=FILE");
		break;
	default:
		Log::Instance()->Output("INPUT_TYPE=UNKNOWN");
		break;
	}
#ifdef AIX
	Log::Instance()->Output("MQ_Manager=%s", m_sMQMgr.c_str());
#endif

	int counter = 0;
	for ( std::set<std::string>::iterator it = m_sInputPaths.begin(); it != m_sInputPaths.end(); ++it )
	{
		Log::Instance()->Output("PATH %d: [%s]", ++counter, it->c_str());
	}
#endif

#if 0
	while ( GSignal::IsRunning() )
#endif
	{
		sleep(m_nWaitSecs);
	}
}

std::string Prestore::InitCfgPath(const std::string& segment, const std::string& name, 
	bool check_read, bool check_write, bool is_dir) throw(Exception)
{
	std::string cfg_val = m_pCfg->GetCfgValue(segment, name);

	if ( cfg_val.empty() || (is_dir && !Helper::IsDirectory(cfg_val))
		|| (!is_dir && !Helper::IsRegularFile(cfg_val)) )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The ["+segment+"->"+name+"] configuration is invalid!");
	}

	if ( check_read && !Helper::CheckReadPermission(cfg_val) )
	{
		throw Exception(PS_PERMISSION_DENIED, "["+name+"] "+cfg_val+" read permission denied!");
	}

	if ( check_write && !Helper::CheckWritePermission(cfg_val) )
	{
		throw Exception(PS_PERMISSION_DENIED, "["+name+"] "+cfg_val+" write permission denied!");
	}

	if ( is_dir )
	{
		Helper::AddDirSlash(cfg_val);
	}

	return cfg_val;
}

#ifdef AIX
void Prestore::InitInputMQ(const std::string& paths) throw(Exception)
{
	// Format: [ QM_Mgr : QM_Queue1, QM_Queue2, QM_Queue3, ...]
	std::list<std::string> list_str;
	Helper::SplitStr(paths, ":", list_str, true);

	if ( list_str.size() != 2 )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The [INPUT->INPUT_PATH] (Type:MQ) configuration is invalid!");
	}

	std::list<std::string>::iterator it = list_str.begin();
	m_sMQMgr = *it;
	if ( m_sMQMgr.empty() )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:MQ) configuration [MQ_Manager] is blank!");
	}
	Helper::Upper(m_sMQMgr);

	std::string mq_queues = *(++it);
	Helper::Upper(mq_queues);

	list_str.clear();
	Helper::SplitStr(mq_queues, ",", list_str, true);

	if ( list_str.empty() )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:MQ) configuration [MQ_Queue(s)...] is blank!");
	}

	m_sInputPaths.clear();
	for ( it = list_str.begin(); it != list_str.end(); ++it )
	{
		if ( it->empty() )
		{
			throw Exception(PS_CFG_ITEM_INVALID, "There is a blank in input (Type:MQ) configuration [MQ_Queue(s)] !");
		}

		if ( m_sInputPaths.find(*it) != m_sInputPaths.end() )
		{
			throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:MQ) configuration [MQ_Queue:"+*it+"] duplication!");
		}

		m_sInputPaths.insert(*it);
	}
}
#endif

void Prestore::InitInputPaths(const std::string& paths) throw(Exception)
{
	// Format: [ Path1, Path2, Path3, ...]
	std::list<std::string> list_str;
	Helper::SplitStr(paths, ",", list_str, true);

	if ( list_str.empty() )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:FILE) configuration is blank!");
	}

	m_sInputPaths.clear();
	for ( std::list<std::string>::iterator it = list_str.begin(); it != list_str.end(); ++it )
	{
		if ( it->empty() )
		{
			throw Exception(PS_CFG_ITEM_INVALID, "There is a blank in input (Type:FILE) configuration!");
		}

		if ( !Helper::IsDirectory(*it) )
		{
			throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:FILE) configuration ["+*it+"] is not a valid path!");
		}

		if ( !Helper::CheckReadPermission(*it) )
		{
			throw Exception(PS_PERMISSION_DENIED, "The input (Type:FILE) configuration ["+*it+"] read permission denied!");
		}

		if ( !Helper::CheckWritePermission(*it) )
		{
			throw Exception(PS_PERMISSION_DENIED, "The input (Type:FILE) configuration ["+*it+"] write permission denied!");
		}

		Helper::AddDirSlash(*it);

		if ( m_sInputPaths.find(*it) != m_sInputPaths.end() )
		{
			throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:FILE) configuration ["+*it+"] duplication!");
		}

		m_sInputPaths.insert(*it);
	}
}

void Prestore::InitChannels() throw(Exception)
{
	m_pCfg->DeleteItems();

	std::string channel_segment;
	for ( int i = 0; i < m_nTotalChannels; ++i )
	{
		channel_segment = "CHANNEL_" + Helper::Num2Str(i+1);
		m_pCfg->RegisterItem(channel_segment, "ID");
		m_pCfg->RegisterItem(channel_segment, "PATH");
		m_pCfg->RegisterItem(channel_segment, "SUB_PATH");
	}

	m_pCfg->ReadConfig();

	m_mChannels.clear();

	std::string tmp;
	for ( int i = 0; i < m_nTotalChannels; ++i )
	{
		channel_segment = "CHANNEL_" + Helper::Num2Str(i+1);
		tmp = m_pCfg->GetCfgValue(channel_segment, "ID");
		Log::Instance()->Output("[%s] ID=[%s]\n", channel_segment.c_str(), tmp.c_str());
		tmp = m_pCfg->GetCfgValue(channel_segment, "PATH");
		Log::Instance()->Output("[%s] PATH=[%s]\n", channel_segment.c_str(), tmp.c_str());

		//if ( m_mChannels.find(channel_id) != m_mChannels.end() )
		//{
		//}
	}

	if ( m_bGeneral )
	{
//		Config g_cfg(m_sGeneralFile);
//		g_cfg->RegisterItem("COMMON"
	}
}

