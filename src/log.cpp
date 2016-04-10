#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <dirent.h>
#include <unistd.h>
#include "def.h"
#include "simpletime.h"

Log* Log::_spLogger = NULL;		// single log pointer

long long Log::_sLogCcmID = 0;
unsigned long Log::_sMaxLogFileSize = 10*1024*1024;	// default log file size 10M

Log::Log()
:m_sCurrentFileSize(0)
{
}

Log::~Log()
{
	TryCloseFileLogger();
}

Log* Log::Instance()
{
	if ( NULL == _spLogger )
	{
		_spLogger = new Log();
	}

	return _spLogger;
}

void Log::Release()
{
	if ( _spLogger != NULL )
	{
		delete _spLogger;
		_spLogger = NULL;
	}
}

bool Log::SetCCMID(long long ccm_id)
{
	if ( ccm_id > 0 )
	{
		_sLogCcmID = ccm_id;
		return true;
	}

	return false;
}

bool Log::ResetFileSize(unsigned long fsize)
{
	if ( fsize > 0 )
	{
		_sMaxLogFileSize = fsize;
		return true;
	}

	return false;
}

void Log::Init() throw(Exception)
{
	if ( m_sLogPath.empty() )
	{
		throw Exception(LE_INIT_FAIL, "[LOG] The log path is not configured! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	TryCloseFileLogger();

	OpenNewLogger();
}

void Log::SetPath(const std::string& path) throw(Exception)
{
	if ( path.empty() )
	{
		throw Exception(LE_FILE_PATH_EMPTY, "[LOG] The log path is empty! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	DIR* p_dir = opendir(path.c_str());
	if ( NULL == p_dir )
	{
		throw Exception(LE_FILE_PATH_INVALID, "[LOG] The log path is invalid: %s [FILE:%s, LINE:%d]", path.c_str(), __FILE__, __LINE__);
	}
	closedir(p_dir);

	const char LAST_CH = path[path.size()-1];
	if ( LAST_CH == '/' || LAST_CH == '\\' )
	{
		m_sLogPath = path;
	}
	else
	{
		m_sLogPath = path + "/";
	}
}

bool Log::Output(const char* format, ...)
{
	if ( !m_fsLogger.is_open() || !m_fsLogger.good() )
	{
		return false;
	}

	char buf[2048] = "";
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vsnprintf(buf, sizeof(buf), format, arg_ptr);
	va_end(arg_ptr);

	std::string str_out = SimpleTime::Now().TimeStamp() + std::string("\x20\x20\x20\x20") + buf + std::string("\n");
	m_fsLogger.write(str_out.c_str(), str_out.size());
	m_fsLogger.flush();

	m_sCurrentFileSize += str_out.size();
	// Maximum file size
	if ( m_sCurrentFileSize >= _sMaxLogFileSize )
	{
		m_sCurrentFileSize = 0;

		str_out = "\n\n\x20\x20\x20\x20( MAXIMUM FILE SIZE )\n";
		m_fsLogger.write(str_out.c_str(), str_out.size());
		m_fsLogger.flush();
		m_fsLogger.close();

		OpenNewLogger();
	}
	return true;
}

void Log::TryCloseFileLogger()
{
	if ( m_fsLogger.is_open() )
	{
		m_fsLogger.flush();
		m_fsLogger.close();
	}
}

void Log::OpenNewLogger() throw(Exception)
{
	int log_id = 0;
	char log_name[64] = "";
	std::string fullLogPath;

	const std::string DAY_TIME = SimpleTime::Now().DayTime8();

	do
	{
		sprintf(log_name, "LOG_%lld_%s_%04d.log", _sLogCcmID, DAY_TIME.c_str(), log_id++);

		fullLogPath = m_sLogPath + log_name;

	} while ( access(fullLogPath.c_str(), 0) == 0 );

	m_fsLogger.open(fullLogPath.c_str(), std::fstream::out);
	if ( !m_fsLogger.is_open() || !m_fsLogger.good() )
	{
		throw Exception(LE_OPEN_FILE_FAIL, "[LOG] Can not open log file: %s [FILE:%s, LINE:%d]", fullLogPath.c_str(), __FILE__, __LINE__);
	}
}

