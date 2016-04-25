#include <iostream>
#include <unistd.h>
#include "config.h"
#include "gsignal.h"
#include "helper.h"
#include "log.h"
#include "prestore.h"
#include "dbi.h"

const char* g_ver = "PreStore: Version 1.07.0425 released.";

int main(int argc, char* argv[])
{
	if ( argc != 5 )
	{
		std::cerr << "[usage] " << argv[0] << " task_id start_datetime ccm_id cfg_file " << std::endl;
		return -1;
	}

	// Daemon process ?
	int task_id = Helper::Str2Int(argv[1]);
	if ( task_id > 0 )		// For monitor
	{
		pid_t fpid = fork();
		if ( fpid < 0 )		// fork error!
		{
			std::cerr << "[FORK] ERROR: " << fpid << std::endl;
			return -1;
		}
		else if ( fpid > 0 )	// Parent process end
		{
			return 0;
		}
	}

	// The type of CCM_ID is: [long long]
	if ( !Log::SetCCMID(Helper::Str2LLong(argv[3])) )
	{
		std::cerr << "[LOG] Set CCM_ID fail!" << std::endl;
		return -1;
	}

	DBI dbi;
	AutoTask atask;

	AutoLogger alog;
	Log* pLog = alog.Get();

	try
	{
		Config cfg;
		cfg.SetCfgFile(argv[4]);
		cfg.RegisterItem("SYS", "LOG_PATH");
		cfg.RegisterItem("DATABASE", "DB_NAME");
		cfg.RegisterItem("DATABASE", "SVR_NAME");
		cfg.ReadConfig();

		pLog->SetPath(cfg.GetCfgValue("SYS", "LOG_PATH"));
		pLog->Init();

		char version[128] = "";
		sprintf(version, "%s Compiled at %s on %s", g_ver, __TIME__, __DATE__);
		std::cout << version << std::endl;
		pLog->Output(version);

		std::string db_name  = cfg.GetCfgValue("DATABASE", "DB_NAME");
		std::string svr_name = cfg.GetCfgValue("DATABASE", "SVR_NAME");
		if ( dbi.dbi_init("oracle", db_name.c_str(), svr_name.c_str()) != DBI_SUCCESS )
		{
			pLog->Output("[ERROR] DBI init failed! ERROR: %s", dbi.dbi_error());
			return -1;
		}
		if ( dbi.dbi_connect() != DBI_SUCCESS )
		{
			pLog->Output("[ERROR] DBI connect <DB_NAME:%s, SVR_NAME:%s> failed! ERROR: %s", db_name.c_str(), svr_name.c_str(), dbi.dbi_error());
			return -1;
		}
		pLog->Output("DBI connect <DB_NAME:%s, SVR_NAME:%s> OK.", db_name.c_str(), svr_name.c_str());
		
		if ( task_id > 0 )		// Monitor
		{
			atask.Reset(new Task);
			
			Task* pTask = atask.Get();
			if ( pTask->t_init(task_id, getpid(), &dbi) != 0 )
			{
				pLog->Output("[ERROR] Task init error: %s", pTask->t_error());
				return -1;
			}
		}
		else
		{
			if ( !GSignal::Init() )
			{
				std::cerr << "[ERROR] GSignal init fail!" << std::endl;
				pLog->Output("[ERROR] GSignal init fail!");
				return -1;
			}
		}

		Prestore ps(cfg, atask);
		ps.Init();
		ps.Run();
	}
	catch ( Exception& ex )
	{
		std::cerr << "[ERROR] " << ex.What() << ", ERR_CODE: " << ex.ErrorCode() << std::endl;
		pLog->Output("[ERROR] %s, ERR_CODE:%d", ex.What().c_str(), ex.ErrorCode());

		Task* pTask = atask.Get();
		if ( pTask != NULL )
		{
			if ( pTask->t_set_task_result(false) != 0 )
			{
				pLog->Output("<WARNING> t_set_task_result error: %s", pTask->t_error());
			}

			if ( pTask->t_set_exit(false) != 0 )
			{
				pLog->Output("<WARNING> t_set_exit error: %s", pTask->t_error());
			}
		}

		dbi.dbi_disconnect();
		return -1;
	}
	catch ( ... )
	{
		std::cerr << "[ERROR] Unknown error!" << std::endl;
		pLog->Output("[ERROR] Unknown error!");

		Task* pTask = atask.Get();
		if ( pTask != NULL )
		{
			if ( pTask->t_set_task_result(false) != 0 )
			{
				pLog->Output("<WARNING> t_set_task_result error: %s", pTask->t_error());
			}

			if ( pTask->t_set_exit(false) != 0 )
			{
				pLog->Output("<WARNING> t_set_exit error: %s", pTask->t_error());
			}
		}

		dbi.dbi_disconnect();
		return -1;
	}

	dbi.dbi_disconnect();

	std::cout << argv[0] << " quit!!!" << std::endl;
	pLog->Output("%s quit!!!", argv[0]);
	return 0;
}

