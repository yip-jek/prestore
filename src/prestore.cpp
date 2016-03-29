#include "prestore.h"
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
	m_pCfg->RegisterItem("COMMON", "GENERAL");
	m_pCfg->RegisterItem("COMMON", "GENERAL_FILE");
	m_pCfg->RegisterItem("COMMON", "CHANNELS");
	m_pCfg->RegisterItem("COMMON", "DEFAULT_CHANNEL");

	m_pCfg->ReadConfig();

	m_nWaitSecs = (int)m_pCfg->GetCfgLongVal("SYS", "WAIT_SEC");
	if ( m_nWaitSecs < 0 )
	{
		throw Exception(PS_WAIT_SECS_INVALID, "The [SYS->WAIT_SEC] configuration is invalid!");
	}

	m_nPackets = (int)m_pCfg->GetCfgLongVal("SYS", "PACKETS");
	if ( m_nPackets <= 0 )
	{
		throw Exception(PS_PACKETS_INVALID, "The [SYS->PACKETS] configuration is invalid!");
	}

	m_bGeneral = m_pCfg->GetCfgBoolVal("COMMON", "GENERAL");
	if ( m_bGeneral )		// GENERAL = TRUE
	{
		m_sGeneralFile = m_pCfg->GetCfgValue("COMMON", "GENERAL_FILE");

		if ( m_sGeneralFile.empty() || !Helper::IsRegularFile(m_sGeneralFile) )
		{
			throw Exception(PS_GENERAL_FILE_INVALID, "The [COMMON->GENERAL_FILE] configuration is invalid!");
		}
		if ( !Helper::CheckReadPermission(m_sGeneralFile) )
		{
			throw Exception(PS_PERMISSION_DENIED, "[GENERAL_FILE] "+m_sGeneralFile+" read permission denied!");
		}
	}

	m_nTotalChannels = (int)m_pCfg->GetCfgLongVal("COMMON", "CHANNELS");
	if ( m_nTotalChannels < 0 )
	{
		throw Exception(PS_TOTAL_CHANNELS_INVALID, "The [COMMON->CHANNELS] configuration is invalid!");
	}

	m_sDefaultChannel = m_pCfg->GetCfgValue("COMMON", "DEFAULT_CHANNEL");
	if ( m_sDefaultChannel.empty() || !Helper::IsDirectory(m_sDefaultChannel) )
	{
		throw Exception(PS_DEFAULT_CHANNEL_INVALID, "The [COMMON->DEFAULT_CHANNEL] configuration is invalid!");
	}
	if ( !Helper::CheckWritePermission(m_sDefaultChannel) )
	{
		throw Exception(PS_PERMISSION_DENIED, "[DEFAULT_CHANNEL] "+m_sDefaultChannel+" write permission denied!");
	}
	Helper::AddDirSlash(m_sDefaultChannel);

	InitChannels();
}

void Prestore::Run() throw(Exception)
{
	Log::Instance()->Output("WAIT_SECS=%d", m_nWaitSecs);
	Log::Instance()->Output("PACKETS=%d", m_nPackets);
	Log::Instance()->Output("GENERAL=%d", m_bGeneral);
	Log::Instance()->Output("GENERAL_FILE=%s", m_sGeneralFile.c_str());
	Log::Instance()->Output("TOTAL_CHANNELS=%d", m_nTotalChannels);
	Log::Instance()->Output("DEFAULT_CHANNEL=%s", m_sDefaultChannel.c_str());

#if 0
	while ( GSignal::IsRunning() )
#endif
	{
		sleep(1);
	}
}

void Prestore::InitChannels() throw(Exception)
{
	m_mChannels.clear();
	for ( int i = 0; i < m_nTotalChannels; ++i )
	{
	}

	if ( m_bGeneral )
	{
		m_pCfg->
		m_pCfg->SetCfgFile(m_sGeneralFile);
		m_pCfg->RegisterItem(
	}
}

