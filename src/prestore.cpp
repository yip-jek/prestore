#include "prestore.h"
#include <list>
#include <unistd.h>
#include <string.h>
#include "gsignal.h"
#include "config.h"
#include "log.h"
#include "def.h"
#include "input.h"

Packet::Packet()
:m_nFileID(0)
,m_nChannelID1(-1)
,m_nChannelID2(-1)
,m_pZipBuf(NULL)
,m_nZipSize(0)
,m_pUncomBuf(NULL)
,m_nUncomSize(0)
{
	BillPeriodZero();

	m_pZipBuf = new char[ZIP_MAX_SIZE];
	ZipBufZero();

	m_pUncomBuf = new char[UNCOMPRESS_MAX_SIZE];
	UncomBufZero();
}

Packet::~Packet()
{
	if ( m_pZipBuf != NULL )
	{
		delete[] m_pZipBuf;
		m_pZipBuf = NULL;
	}

	if ( m_pUncomBuf != NULL )
	{
		delete[] m_pUncomBuf;
		m_pUncomBuf = NULL;
	}
}

void Packet::Init()
{
	m_nFileID     = 0;
	m_nChannelID1 = -1;
	m_nChannelID2 = -1;
	m_nZipSize    = 0;
	m_nUncomSize  = 0;

	BillPeriodZero();
	ZipBufZero();
	UncomBufZero();
}

void Packet::BillPeriodZero()
{
	memset(m_sBillPeriod, 0, sizeof(m_sBillPeriod));
}

void Packet::ZipBufZero()
{
	memset(m_pZipBuf, 0, ZIP_MAX_SIZE);
}

void Packet::UncomBufZero()
{
	memset(m_pUncomBuf, 0, UNCOMPRESS_MAX_SIZE);
}


//////////////////////////////////////////////////////////////////////////////////////
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
	Packet pack;

	while ( GSignal::IsRunning() )
	{
		pack.Init();

		if ( m_pInput->GetPacket(&pack) )
		{
			//Log::Instance()->Output("Get file: %s", file_name.c_str());

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

