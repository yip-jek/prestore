#include "prestore.h"
#include <list>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "gsignal.h"
#include "config.h"
#include "log.h"
#include "def.h"
#include "input.h"
#include "packethead.h"

size_t Packet::_ZIP_MAX_SIZE        = 4194304;
size_t Packet::_UNCOMPRESS_MAX_SIZE = 16777216;

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

	m_pZipBuf = new char[_ZIP_MAX_SIZE];
	ZipBufZero();

	m_pUncomBuf = new char[_UNCOMPRESS_MAX_SIZE];
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
	m_srcFilePath.clear();

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
	memset(m_pZipBuf, 0, _ZIP_MAX_SIZE);
}

void Packet::UncomBufZero()
{
	memset(m_pUncomBuf, 0, _UNCOMPRESS_MAX_SIZE);
}


//////////////////////////////////////////////////////////////////////////////////////
AutoTask::AutoTask(Task* p_new_task /*= NULL*/)
:m_pTask(p_new_task)
{
}

AutoTask::~AutoTask()
{
	Release();
}

void AutoTask::Reset(Task* p_new_task)
{
	Release();

	m_pTask = p_new_task;
}

void AutoTask::Release()
{
	if ( m_pTask != NULL )
	{
		delete m_pTask;
		m_pTask = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////////////////
Prestore::Prestore(Config& cfg, AutoTask& auto_task)
:m_pCfg(&cfg)
,m_nWaitSecs(0)
,m_nPackets(0)
,m_nMaxTypeID(0)
,m_pInput(NULL)
,m_channelPath(&cfg)
,m_pAutoTask(&auto_task)
{
}

Prestore::~Prestore()
{
	Release();
}

void Prestore::Init() throw(Exception)
{
	if ( NULL == m_pCfg )
	{
		throw Exception(PS_CONFIG_INVALID, "The configuration pointer is NULL! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	m_pCfg->RegisterItem("SYS", "WAIT_SEC");
	m_pCfg->RegisterItem("SYS", "PACKETS");
	m_pCfg->RegisterItem("INPUT", "MAX_TYPE_ID");
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

	m_nMaxTypeID = (int)m_pCfg->GetCfgLongVal("INPUT", "MAX_TYPE_ID");
	if ( m_nMaxTypeID <= 0 )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The [INPUT->MAX_TYPE_ID] configuration is invalid! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	std::string input_paths = m_pCfg->GetCfgValue("INPUT", "INPUT_PATH");

	Release();
	
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
	PacketHead* pHead;

	while ( Running() )
	{
		pack.Init();

		Input::PACKET_STATE p_state = m_pInput->GetPacket(&pack);
		if ( p_state != Input::PKS_NO_PACKET )
		{
			if ( p_state != Input::PKS_LINK_INVALID )
			{
				if ( (size_t)pack.m_nUncomSize < sizeof(PacketHead) )	// Packet size less than packet_head size
				{
					Log::Instance()->Output("[ERROR] Packet size (%d) < PacketHead size (%d)", pack.m_nUncomSize, sizeof(PacketHead));

					pack.m_nFileID = -time(NULL);
					strcpy(pack.m_sBillPeriod, "0");

					SuspendPacket(&pack, SUS_LESS_PACKET_HEAD);
				}
				else
				{
					pHead = (PacketHead*)pack.m_pUncomBuf;

					if ( pack.m_srcFilePath.empty() )
					{
						Log::Instance()->Output("[GET MQ] type_id:%d, type_ex:%s file_id:%d, channel_id1:%d, channel_id2:%d", 
								pHead->type_id, pHead->type_extend, pHead->file_id, pHead->channel_id1, pHead->channel_id2);
					}
					else
					{
						Log::Instance()->Output("[GET FILE] type_id:%d, type_ex:%s file_id:%d, channel_id1:%d, channel_id2:%d", 
								pHead->type_id, pHead->type_extend, pHead->file_id, pHead->channel_id1, pHead->channel_id2);
					}

					pack.m_nFileID     = pHead->file_id;
					pack.m_nChannelID1 = pHead->channel_id1;
					pack.m_nChannelID2 = pHead->channel_id2;
					strcpy(pack.m_sBillPeriod, pHead->bill_period);

					if ( pHead->type_id > 0 && pHead->type_id < m_nMaxTypeID )
					{
						DistributePacket(&pack);
					}
					else
					{
						Log::Instance()->Output("[ERROR] Packet type_id (%d) is out of range!", pHead->type_id);
						SuspendPacket(&pack, SUS_INVALID_TYPE_ID);
					}
				}
			}

			m_pInput->DelSrcPacket(p_state);

			if ( ++counter < m_nPackets )
			{
				continue;
			}
		}

		counter = 0;

		sleep(m_nWaitSecs);
	}
}

void Prestore::Release()
{
	if ( m_pInput != NULL )
	{
		delete m_pInput;
		m_pInput = NULL;
	}
}

void Prestore::SuspendPacket(Packet* p, long error) throw(Exception)
{
	char file_name[128] = "";
	snprintf(file_name, sizeof(file_name), "%d.%d.%d.%ld.%s.Z", p->m_nFileID, 
		p->m_nChannelID1, p->m_nChannelID2, error, p->m_sBillPeriod);

	Log::Instance()->Output("Suspend file: %swork/%s", m_sSuspendPath.c_str(), file_name);

	WriteFile(m_sSuspendPath, file_name, p->m_pZipBuf, p->m_nZipSize);
}

void Prestore::DistributePacket(Packet* p) throw(Exception)
{
	char file_name[128] = "";
	snprintf(file_name, sizeof(file_name), "%d.%d.%d.%s.Z", p->m_nFileID, 
		p->m_nChannelID1, p->m_nChannelID2, p->m_sBillPeriod);

	std::string channel_path = m_channelPath.GetChannelPath(p->m_nChannelID1);

	if ( p->m_srcFilePath.empty() )
	{
		Log::Instance()->Output("Distribute file: [FROM] MQ [TO] %swork/%s", channel_path.c_str(), file_name);
	}
	else
	{
		Log::Instance()->Output("Distribute file: [FROM] %s [TO] %swork/%s", p->m_srcFilePath.c_str(), channel_path.c_str(), file_name);
	}

	WriteFile(channel_path, file_name, p->m_pZipBuf, p->m_nZipSize);
}

void Prestore::WriteFile(const std::string& path, const std::string& file_name, char* pBuf, int buf_size) throw(Exception)
{
	std::string work_file = path + "work/";
	std::string link_file = path + "commit/";

	TryCreateDir(work_file);
	TryCreateDir(link_file);

	work_file += file_name;

	FILE* fp = fopen(work_file.c_str(), "w");
	if ( NULL == fp )
	{
		throw Exception(PS_OPEN_FILE_FAIL, "Open file \"%s\" fail! %s [FILE:%s, LINE:%d]", work_file.c_str(), strerror(errno), __FILE__, __LINE__);
	}

	if ( fwrite(pBuf, buf_size, 1, fp) != 1 )
	{
		fclose(fp);
		unlink(work_file.c_str());
		throw Exception(PS_WRITE_FILE_FAIL, "Write file \"%s\" fail! %s [FILE:%s, LINE:%d]", work_file.c_str(), strerror(errno), __FILE__, __LINE__);
	}

	fclose(fp);

	link_file += file_name;

	unlink(link_file.c_str());
	if ( symlink(work_file.c_str(), link_file.c_str()) != 0 )
	{
		unlink(work_file.c_str());
		throw Exception(PS_LINK_FILE_FAIL, "Link file \"%s\" fail! %s [FILE:%s, LINE:%d]", link_file.c_str(), strerror(errno), __FILE__, __LINE__);
	}
}

void Prestore::TryCreateDir(const std::string& dir_path) throw(Exception)
{
	// Not existed ?
	if ( access(dir_path.c_str(), F_OK) != 0 )
	{
		if ( mkdir(dir_path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) != 0 )
		{
			throw Exception(PS_CREATE_DIR_FAIL, "Try to create dir \"%s\" fail! %s [FILE:%s, LINE:%d]", dir_path.c_str(), strerror(errno), __FILE__, __LINE__);
		}
	}
}

bool Prestore::Running()
{
	Task* pTask = m_pAutoTask->Get();
	if ( pTask != NULL )	// Monitor
	{
		char param[2048] = "";

		int result = pTask->t_get_task(param, sizeof(param));
		if ( 2 == result )
		{
			Log::Instance()->Output("Get exit task, ready to quit ...");

			if ( pTask->t_set_task_result(true) != 0 )
			{
				Log::Instance()->Output("<WARNING> t_set_task_result error: %s", pTask->t_error());
			}

			if ( pTask->t_set_exit(true) != 0 )
			{
				Log::Instance()->Output("<WARNING> t_set_exit error: %s", pTask->t_error());
			}

			return false;
		}
		else if ( result < 0 )
		{
			Log::Instance()->Output("<WARNING> t_get_task error: %s", pTask->t_error());
		}
		else if ( result > 0 && pTask->t_set_task_result(true) != 0 )
		{
			Log::Instance()->Output("<WARNING> t_set_task_result error: %s", pTask->t_error());
		}

		return true;
	}
	else
	{
		return GSignal::IsRunning();
	}
}

