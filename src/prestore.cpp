#include "prestore.h"
#include <list>
#include <unistd.h>
#include "gsignal.h"
#include "config.h"
#include "log.h"
#include "def.h"
#include "input.h"

Prestore::Prestore(Config& cfg)
:m_pCfg(&cfg)
,m_nWaitSecs(0)
,m_nPackets(0)
,m_pInput(NULL)
,m_channelPath(&cfg)
{
}

Prestore::~Prestore()
{
	ReleaseInput();
}

void Prestore::Init() throw(Exception)
{
	if ( NULL == m_pCfg )
	{
		throw Exception(PS_CONFIG_INVALID, "The configuration pointer is NULL! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
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
		throw Exception(PS_CFG_ITEM_INVALID, "The [SYS->WAIT_SEC] configuration is invalid! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	m_nPackets = (int)m_pCfg->GetCfgLongVal("SYS", "PACKETS");
	if ( m_nPackets <= 0 )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The [SYS->PACKETS] configuration is invalid! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	std::string input_paths = m_pCfg->GetCfgValue("INPUT", "INPUT_PATH");

	ReleaseInput();

	int type = (int)m_pCfg->GetCfgLongVal("INPUT", "INPUT_TYPE");
	if ( 1 == type )
	{
#ifdef AIX
		m_pInput = new InputMQ(input_paths, m_nPackets);
#else
		throw Exception(PS_UNSUPPORT_INPUTTYPE, "Unsupport \"MQ\" input type in non-AIX environment! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
#endif
	}
	else if ( 2 == type )
	{
		m_pInput = new InputPath(input_paths, m_nPackets);
	}
	else
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The [INPUT->INPUT_TYPE] configuration is invalid! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}
	m_pInput->Init();

	m_sSuspendPath = m_pCfg->GetCfgValue("COMMON", "SUSPEND_PATH");
	STPath::CheckPathFile(m_sSuspendPath, true, true, true);

	m_channelPath.Init();

	Log::Instance()->Output("Init OK!");
}

void Prestore::Run() throw(Exception)
{
	if ( NULL == m_pInput )
	{
		throw Exception(PS_INPUT_INVALID, "The input pointer is NULL! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	int counter = 0;
	std::string file_name;

	while ( GSignal::IsRunning() )
	{
		if ( m_pInput->GetPacket(file_name) )
		{
			Log::Instance()->Output("Get file: %s", file_name.c_str());

			// Todo ...

			m_pInput->DelSrcPacket();

			if ( ++counter < m_nPackets )
			{
				continue;
			}
		}

		counter = 0;

		sleep(m_nWaitSecs);
	}
}

void Prestore::ReleaseInput()
{
	if ( m_pInput != NULL )
	{
		delete m_pInput;
		m_pInput = NULL;
	}
}

