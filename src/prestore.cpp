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
,m_channelPath(&cfg)
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

	m_sSuspendPath = m_pCfg->GetCfgValue("COMMON", "SUSPEND_PATH");
	STPath::CheckPathFile(m_sSuspendPath, true, true, true);

	m_channelPath.Init();
}

void Prestore::Run() throw(Exception)
{
#ifdef DEBUG
	Log::Instance()->Output("WAIT_SECS=%d", m_nWaitSecs);
	Log::Instance()->Output("PACKETS=%d", m_nPackets);
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

	Log::Instance()->Output(">>>>>>>>>>>>>>>>>>>>>> [CHANNELS] <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
	std::list<std::string> list_str;
	m_channelPath.DebugOutput(list_str);
	for ( std::list<std::string>::iterator it = list_str.begin(); it != list_str.end(); ++it )
	{
		Log::Instance()->Output("%s", it->c_str());
	}
#endif

#if 0
	while ( GSignal::IsRunning() )
#endif
	{
		sleep(m_nWaitSecs);
	}
}

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

