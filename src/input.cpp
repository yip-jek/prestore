#include "input.h"
#include <unistd.h>
#include "def.h"
#include "helper.h"
#include "log.h"

Input::Input(const std::string& paths, int packet)
:m_paths(paths)
,m_packets(packet)
{
}


//////////////////////////////////////////////////////////////////
#ifdef AIX
InputMQ::InputMQ(const std::string& mq, int packet)
:Input(mq, packet)
,m_mIter(m_mMQQueue.end())
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
		throw Exception(PS_CFG_ITEM_INVALID, "The [INPUT->INPUT_PATH] (Type:MQ) configuration is invalid! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	std::list<std::string>::iterator it = list_str.begin();
	m_sMQMgr = *it;
	if ( m_sMQMgr.empty() )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:MQ) configuration [MQ_Manager] is blank! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}
	Helper::Upper(m_sMQMgr);

	std::string mq_queues = *(++it);
	Helper::Upper(mq_queues);

	list_str.clear();
	Helper::SplitStr(mq_queues, ",", list_str, true);

	if ( list_str.empty() )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:MQ) configuration [MQ_Queue(s)...] is blank! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	Close();

	for ( it = list_str.begin(); it != list_str.end(); ++it )
	{
		if ( it->empty() )
		{
			throw Exception(PS_CFG_ITEM_INVALID, "There is a blank in input (Type:MQ) configuration [MQ_Queue(s)] ! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
		}

		if ( m_mMQQueue.find(*it) != m_mMQQueue.end() )
		{
			throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:MQ) configuration [MQ_Queue:%s] duplication! [FILE:%s, LINE:%d]", it->c_str(), __FILE__, __LINE__);
		}

		m_mMQQueue[*it] = TMq();
	}

	m_mIter = m_mMQQueue.end();
}

bool InputMQ::GetPacket(std::string& pack) throw(Exception)
{
	return false;
}

void InputMQ::DelSrcPacket()
{
}

void InputMQ::Close()
{
	if ( !m_mMQQueue.empty() )
	{
		m_mMQQueue.clear();
	}
}
#endif


//////////////////////////////////////////////////////////////////
Dir::Dir(const std::string& path)
:m_sPath(path)
,m_pDir(NULL)
,m_bEmpty(true)
{
}

Dir::~Dir()
{
	Close();
}

void Dir::Open() throw(Exception)
{
	Close();

	m_pDir = opendir(m_sPath.c_str());
	if ( NULL == m_pDir )
	{
		throw Exception(INPUT_OPEN_FAIL, "Open path \"%s\" fail! [FILE:%s, LINE:%d]", m_sPath.c_str(), __FILE__, __LINE__);
	}

	m_bEmpty = false;
}

void Dir::Close()
{
	if ( m_pDir != NULL )
	{
		closedir(m_pDir);
		m_pDir = NULL;

		m_bEmpty = true;
	}
}

bool Dir::IsEmpty() const
{
	return m_bEmpty;
}

bool Dir::GetFileName(std::string& file_name)
{
	struct dirent* p_dnt = NULL;

	while ( (p_dnt = readdir(m_pDir)) != NULL )
	{
		if ( 0 == p_dnt->d_ino || '.' == p_dnt->d_name[0] )
		{
			continue;
		}

		if ( Helper::IsDirectory(p_dnt->d_name) )
		{
			continue;
		}

		file_name = p_dnt->d_name;
		m_bEmpty = false;
		return true;
	}

	m_bEmpty = true;
	return false;
}

bool Dir::GetFullName(std::string& full_name)
{
	if ( GetFileName(full_name) )
	{
		full_name = m_sPath + full_name;
		return true;
	}

	return false;
}


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
		throw Exception(PS_CFG_ITEM_INVALID, "The input packets is invalid! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	//Format: [ Path1, Path2, Path3, ...]
	std::list<std::string> list_str;
	Helper::SplitStr(m_paths, ",", list_str, true);

	if ( list_str.empty() )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:FILE) configuration is blank! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	Close();

	for ( std::list<std::string>::iterator it = list_str.begin(); it != list_str.end(); ++it )
	{
		if ( it->empty() )
		{
			throw Exception(PS_CFG_ITEM_INVALID, "There is a blank in input (Type:FILE) configuration! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
		}

		if ( !Helper::IsDirectory(*it) )
		{
			throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:FILE) configuration [%s] is not a valid path! [FILE:%s, LINE:%d]", it->c_str(), __FILE__, __LINE__);
		}

		if ( !Helper::CheckReadPermission(*it) )
		{
			throw Exception(PS_PERMISSION_DENIED, "The input (Type:FILE) configuration [%s] read permission denied! [FILE:%s, LINE:%d]", it->c_str(), __FILE__, __LINE__);
		}

		if ( !Helper::CheckWritePermission(*it) )
		{
			throw Exception(PS_PERMISSION_DENIED, "The input (Type:FILE) configuration [%s] write permission denied! [FILE:%s, LINE:%d]", it->c_str(), __FILE__, __LINE__);
		}

		Helper::AddDirSlash(*it);

		if ( m_mInputDir.find(*it) != m_mInputDir.end() )
		{
			throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:FILE) configuration [%s] duplication! [FILE:%s, LINE:%d]", it->c_str(), __FILE__, __LINE__);
		}

		m_mInputDir[*it] = new Dir(*it);
	}
}

bool InputPath::GetPacket(std::string& pack) throw(Exception)
{
	if ( m_qFullName.empty() )
	{
		std::map<std::string, Dir*>::iterator it;
		for ( it = m_mInputDir.begin(); it != m_mInputDir.end(); ++it )
		{
			it->second->Open();
		}

		bool bNoFile = false;
		int	counter = 0;
		std::string full_name;

		while ( !bNoFile && counter < m_packets )
		{
			bNoFile = true;

			for ( it = m_mInputDir.begin(); it != m_mInputDir.end(); ++it )
			{
				Dir* pDir = it->second;
				if ( !pDir->IsEmpty() && pDir->GetFullName(full_name) )
				{
					m_qFullName.push_back(full_name);

					if ( bNoFile )
					{
						bNoFile = false;
					}

					if ( ++counter >= m_packets )
					{
						break;
					}
				}
			}
		}
		
		for ( it = m_mInputDir.begin(); it != m_mInputDir.end(); ++it )
		{
			it->second->Close();
		}

		if ( m_qFullName.size() > 0 )
		{
			pack = m_qFullName.front();
			return true;
		}
		else
		{
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
				throw Exception(INPUT_DELETE_FAIL, "Delete link-file \"%s\" fail! [FILE:%s, LINE:%d]", full_name.c_str(), __FILE__, __LINE__);
			}

			if ( unlink(buf) < 0 )
			{
				throw Exception(INPUT_DELETE_FAIL, "Delete file \"%s\" fail! [FILE:%s, LINE:%d]", buf, __FILE__, __LINE__);
			}
		}
		else
		{
			if ( unlink(full_name.c_str()) < 0 )
			{
				throw Exception(INPUT_DELETE_FAIL, "Delete file \"%s\" fail! [FILE:%s, LINE:%d]", full_name.c_str(), __FILE__, __LINE__);
			}
		}

		m_qFullName.pop_front();
	}
}

void InputPath::Close()
{
	if ( !m_mInputDir.empty() )
	{
		for ( std::map<std::string, Dir*>::iterator it = m_mInputDir.begin(); it != m_mInputDir.end(); ++it )
		{
			delete it->second;
		}

		m_mInputDir.clear();
	}

	if ( !m_qFullName.empty() )
	{
		m_qFullName.clear();
	}
}

