#include <iostream>
#include <unistd.h>
#include "config.h"
#include "gsignal.h"
#include "helper.h"
#include "log.h"
#include "prestore.h"

const char* g_ver = "PreStore: Version 1.02.0141 released.";

int main(int argc, char* argv[])
{
	if ( argc != 4 )
	{
		std::cerr << "[usage] " << argv[0] << " deamon_flag ccm_id cfg_file " << std::endl;
		return -1;
	}

	// Daemon process ?
	if ( Helper::Str2Int(argv[1]) == 1 )
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
	if ( !Log::SetCCMID(Helper::Str2LLong(argv[2])) )
	{
		std::cerr << "[LOG] Set CCM_ID fail!" << std::endl;
		return -1;
	}

	try
	{
		Config cfg;
		cfg.SetCfgFile(argv[3]);
		cfg.RegisterItem("SYS", "LOG_PATH");
		cfg.ReadConfig();

		Log::Instance()->SetPath(cfg.GetCfgValue("SYS", "LOG_PATH"));
		Log::Instance()->Init();

		char version[128] = "";
		sprintf(version, "%s Compiled at %s on %s", g_ver, __TIME__, __DATE__);
		std::cout << version << std::endl;
		Log::Instance()->Output(version);

		if ( !GSignal::Init() )
		{
			std::cerr << "[ERROR] GSignal init fail!" << std::endl;
			Log::Instance()->Output("[ERROR] GSignal init fail!");
			return -1;
		}

		Prestore ps(cfg);
		ps.Init();
		ps.Run();
	}
	catch ( Exception& ex )
	{
		std::cerr << "[ERROR] " << ex.What() << ", ERR_CODE: " << ex.ErrorCode() << std::endl;
		Log::Instance()->Output("[ERROR] %s, ERR_CODE:%d", ex.What().c_str(), ex.ErrorCode());
		return -1;
	}
	catch ( ... )
	{
		std::cerr << "[ERROR] Unknown error!" << std::endl;
		Log::Instance()->Output("[ERROR] Unknown error!");
		return -1;
	}

	std::cout << argv[0] << " quit!!!" << std::endl;
	Log::Instance()->Output("%s quit!!!", argv[0]);
	return 0;
}

