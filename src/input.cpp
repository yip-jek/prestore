#include "input.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "def.h"
#include "helper.h"
#include "log.h"
#include "zcomp.h"
#include "prestore.h"

#ifdef AIX
#include "TMq.h"
#endif

Input::Input(const std::string& paths, int packet)
:m_paths(paths)
,m_packets(packet)
,m_pZip(NULL)
{
	m_pZip = new CZip;
}

Input::~Input()
{
	if ( m_pZip != NULL )
	{
		delete m_pZip;
		m_pZip = NULL;
	}
}


//////////////////////////////////////////////////////////////////
#ifdef AIX
AutoCloseMQ::AutoCloseMQ(TMq* pMq)
:m_pMq(pMq)
{}

AutoCloseMQ::~AutoCloseMQ()
{
	if ( m_pMq != NULL )
	{
		m_pMq->Close();
	}
}


InputMQ::InputMQ(const std::string& mq, int packet)
:Input(mq, packet)
,m_sIter(m_sMQQueue.end())
,m_pMQ(NULL)
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

		if ( m_sMQQueue.find(*it) != m_sMQQueue.end() )
		{
			throw Exception(PS_CFG_ITEM_INVALID, "The input (Type:MQ) configuration [MQ_Queue:%s] duplication! [FILE:%s, LINE:%d]", it->c_str(), __FILE__, __LINE__);
		}

		m_sMQQueue.insert(*it);
	}

	m_sIter = m_sMQQueue.end();

	m_pMQ = new TMq;
	try
	{
		m_pMQ->Connect(m_sMQMgr.c_str());
	}
	catch ( TException& t_ex )
	{
		throw Exception(INPUT_CONNECT_MQ_FAIL, "Connect MQ manager [%s] fail: %s [FILE:%s, LINE:%d]", m_sMQMgr.c_str(), t_ex.getErrMsg(), __FILE__, __LINE__);
	}
}

bool InputMQ::GetPacket(Packet* p) throw(Exception)
{
	if ( m_sMQQueue.empty() )
	{
		return false;
	}

	NextIter(m_sIter);

	std::set<std::string>::iterator it = m_sIter;
	do
	{
		try
		{
			if ( GetMQMsg(*it, p) )
			{
				m_sIter = it;
				return true;
			}
		}
		catch ( TException& t_ex )
		{
			m_pMQ->Rollback();

			throw Exception(INPUT_GET_MQ_FAIL, "Get MQ [%s->%s] fail: %s [FILE:%s, LINE:%d]", m_sMQMgr.c_str(), it->c_str(), t_ex.getErrMsg(), __FILE__, __LINE__);
		}

		NextIter(it);
	} while ( it != m_sIter );

	return false;
}

void InputMQ::DelSrcPacket()
{
	if ( m_sIter != m_sMQQueue.end() )
	{
		try
		{
			m_pMQ->Commit();
		}
		catch ( TException& t_ex )
		{
			throw Exception(INPUT_COMMIT_MQ_FAIL, "Commit MQ [%s->%s] fail: %s [FILE:%s, LINE:%d]", m_sMQMgr.c_str(), m_sIter->c_str(), t_ex.getErrMsg(), __FILE__, __LINE__);
		}
	}
}

void InputMQ::Close()
{
	if ( !m_sMQQueue.empty() )
	{
		m_sMQQueue.clear();
	}

	if ( m_pMQ != NULL )
	{
		try
		{
			m_pMQ->Disconnect();
		}
		catch ( TException& t_ex )
		{
			Log::Instance()->Output("Disconnect MQ manager [%s] fail: %s [FILE:%s, LINE:%d]", m_sMQMgr.c_str(), t_ex.getErrMsg(), __FILE__, __LINE__);
		}
	
		delete m_pMQ;
	}
}

void InputMQ::NextIter(std::set<std::string>::iterator& it)
{
	if ( it != m_sMQQueue.end() )
	{
		++it;
		if ( m_sMQQueue.end() == it )
		{
			it = m_sMQQueue.begin();
		}
	}
	else
	{
		it = m_sMQQueue.begin();
	}
}

bool InputMQ::GetMQMsg(const std::string& q_name, Packet* p)
{
	m_pMQ->Begin();
	m_pMQ->Open(q_name.c_str(), TMq::MQ_GET);
	m_pMQ->SetMsgId();

	AutoCloseMQ(m_pMQ);

	if ( m_pMQ->GetMsg(p->m_pZipBuf, Packet::ZIP_MAX_SIZE, &(p->m_nZipSize), MQ_WAIT_SEC) < 0 )
	{
		return false;	// NO Msg
	}

	if ( 0 == p->m_nZipSize )	// Zip file size = 0
	{
		Log::Instance()->Output("<WARNING> Get MQ [%s->%s] packet size = 0!", m_sMQMgr.c_str(), q_name.c_str());
		return false;
	}

	p->m_nUncomSize = m_pZip->decompress(p->m_pUncomBuf, p->m_pZipBuf, CZip::MEMORY, p->m_nZipSize);
	if ( p->m_nUncomSize < 0 )
	{
		throw TException(INPUT_GET_MQ_FAIL, "Uncompress MQ [%s->%s] packet fail! [FILE:%s, LINE:%d]", m_sMQMgr.c_str(), q_name.c_str(), __FILE__, __LINE__);
	}
	p->m_pUncomBuf[p->m_nUncomSize] = '\0';

	return true;
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


AutoFile::AutoFile()
:m_fp(NULL)
{
}

AutoFile::~AutoFile()
{
	Close();
}

bool AutoFile::Open(const std::string& file)
{
	Close();

	m_fp = fopen(file.c_str(), "r");

	return (m_fp != NULL);
}

void AutoFile::Close()
{
	if ( m_fp != NULL )
	{
		fclose(m_fp);
		m_fp = NULL;
	}
}

bool AutoFile::GetSize(const std::string& file, int& size)
{
	struct stat f_stat;

	if ( stat(file.c_str(), &f_stat) != 0 )
	{
		return false;
	}

	size = f_stat.st_size;
	return true;
}

bool AutoFile::Read(char* pBuf, int file_size)
{
	if ( NULL == m_fp )
	{
		return false;
	}

	if ( fread(pBuf, file_size, 1, m_fp) != 1 )
	{
		return false;
	}

	return true;
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

bool InputPath::GetPacket(Packet* p) throw(Exception)
{
	std::string file_name;

	while ( GetFile(file_name) )
	{
		char buf[512] = "";
		if ( readlink(file_name.c_str(), buf, sizeof(buf)) > 0 )
		{
			file_name = buf;
		}

		p->m_srcFilePath = file_name;

		AutoFile af;
		if ( !af.Open(file_name) )
		{
			throw Exception(INPUT_OPEN_FILE_FAIL, "Open file \"%s\" fail! %s [FILE:%s, LINE:%d]", file_name.c_str(), strerror(errno), __FILE__, __LINE__);
		}

		if ( !af.GetSize(file_name, p->m_nZipSize) )
		{
			throw Exception(INPUT_STAT_FILE_FAIL, "Stat file \"%s\" fail! %s [FILE:%s, LINE:%d]", file_name.c_str(), strerror(errno), __FILE__, __LINE__);
		}

		if ( 0 == p->m_nZipSize )
		{
			Log::Instance()->Output("<WARNING> Get file \"%s\" packet size = 0!", file_name.c_str());
			continue;		// Next file
		}

		if ( !af.Read(p->m_pZipBuf, p->m_nZipSize) )
		{
			throw Exception(INPUT_READ_FILE_FAIL, "Read file \"%s\" fail! %s [FILE:%s, LINE:%d]", file_name.c_str(), strerror(errno), __FILE__, __LINE__);
		}

		p->m_nUncomSize = m_pZip->decompress(p->m_pUncomBuf, p->m_pZipBuf, CZip::MEMORY, p->m_nZipSize);
		if ( p->m_nUncomSize < 0 )
		{
			throw Exception(INPUT_UNCOM_FILE_FAIL, "Uncompress file \"%s\" packet fail! [FILE:%s, LINE:%d]", file_name.c_str(), __FILE__, __LINE__);
		}
		p->m_pUncomBuf[p->m_nUncomSize] = '\0';

		return true;
	}

	return false;
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

bool InputPath::GetFile(std::string& file_name)
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
			file_name = m_qFullName.front();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		file_name = m_qFullName.front();
		return true;
	}
}

