#include "input.h"
#include <unistd.h>
#include "def.h"
#include "helper.h"

Input::Input(const std::string& paths, int packet)
:m_paths(paths)
,m_packets(packet)
{
}


//////////////////////////////////////////////////////////////////
#ifdef AIX
InputMQ::InputMQ(const std::string& mq, int packet)
:Input(mq, packet)
,m_sIter(m_sMQQueue.end())
{
}

InputMQ::~InputMQ()
{
	Close();
}

void InputMQ::Init() throw(Exception)
{
	//Format: [ QM_Mgr : QM_Queue1, QM_Queue2, QM_Queue3, ...]
	std::list<std::string> list_str;
	Helper::SplitStr(m_paths, ":", list_str, true);

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

	Close();

	for ( it = list_str.begin(); it != list_str.end(); ++it )
	{
		if ( it->empty() )
		{
			throw Exception(PS_CFG_ITEM_INVALID, "There is a blank in input (Type:MQ) configuration [MQ_Queue(s)] !");
		}

		if ( m_sMQQueue.find(*it) != m_sMQQueue.end() )
		{
			throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:MQ) configuration [MQ_Queue:"+*it+"] duplication!");
		}

		m_sMQQueue.insert(*it);
	}

	m_sIter = m_sMQQueue.end();
}

bool InputMQ::GetPacket(std::string& pack)
{
	return false;
}

void InputMQ::DelSrcPacket()
{
}

void InputMQ::Close()
{
	if ( !m_sMQQueue.empty() )
	{
		m_sMQQueue.clear();
	}
}
#endif


//////////////////////////////////////////////////////////////////
InputPath::InputPath(const std::string& paths, int packet)
:Input(paths, packet)
{
}

InputPath::~InputPath()
{
	Close();
}

void InputPath::Init() throw(Exception)
{
	if ( m_packets <= 0 )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The input packets is invalid!");
	}

	//Format: [ Path1, Path2, Path3, ...]
	std::list<std::string> list_str;
	Helper::SplitStr(m_paths, ",", list_str, true);

	if ( list_str.empty() )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:FILE) configuration is blank!");
	}

	Close();

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

		if ( m_mInputDir.find(*it) != m_mInputDir.end() )
		{
			throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:FILE) configuration ["+*it+"] duplication!");
		}

		m_mInputDir[*it] = NULL;
	}
}

bool InputPath::GetPacket(std::string& pack)
{
	if ( m_qFullName.empty() )
	{
		for ( std::map<std::string, DIR*>::iterator it = m_mInputDir.begin(); it != m_mInputDir.end(); ++it )
		{
			it->second = open
			DIR* p_dir = opendir(m_sIter->c_str());
			if ( NULL == p_dir )
			{
				throw Exception(INPUT_OPEN_FAIL, "Open input path (Type:FILE) \"%s\" fail!", m_sIter->c_str());
			}

			struct dirent* p_dnt = NULL;

			while ( (p_dnt = readdir(p_dir)) != NULL )
			{
				if ( 0 == p_dnt->d_ino || '.' == p_dnt->d_name[0] )
				{
					continue;
				}

				if ( Helper::IsDirectory(p_dnt->d_name) )
				{
					continue;
				}

				m_sFullName = *m_sIter + p_dnt->d_name;
				pack = m_sFullName;

				closedir(p_dir);
				++m_sIter;
				return true;
			}

			closedir(p_dir);
			++m_sIter;
			return false;
		}
	}
	else
	{
		pack = m_qFullName.front();
		return true;
	}
}

void InputPath::DelSrcPacket()
{
	if ( !m_qFullName.empty() )
	{
		std::string full_name = m_qFullName.front();

		char buf[512] = "";
		if ( readlink(full_name.c_str(), buf, sizeof(buf)) > 0 )
		{
			if ( unlink(full_name.c_str()) < 0 )
			{
				throw Exception(INPUT_DELETE_FAIL, "Delete link-file \"%s\" fail!", full_name.c_str());
			}

			if ( unlink(buf) < 0 )
			{
				throw Exception(INPUT_DELETE_FAIL, "Delete file \"%s\" fail!", buf);
			}
		}
		else
		{
			if ( unlink(full_name.c_str()) < 0 )
			{
				throw Exception(INPUT_DELETE_FAIL, "Delete file \"%s\" fail!", full_name.c_str());
			}
		}

		m_qFullName.pop_front();
	}
}

void InputPath::Close()
{
	if ( !m_mInputDir.empty() )
	{
		m_mInputDir.clear();
	}

	if ( !m_qFullName.empty() )
	{
		m_qFullName.clear();
	}
}

