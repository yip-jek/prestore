#include "channelpath.h"
#include "def.h"

STPath::STPath()
:m_itCurrent(m_sPath.end())
{
}

STPath::~STPath()
{
}

void STPath::Init(const std::string& path, const std::string& sub_path) throw(Exception)
{
	m_sPath.clear();
	m_itCurrent = m_sPath.end();

	if ( path.empty() )
	{
		throw Exception(PS_CFG_ITEM_INVALID, "
	}

}

std::string STPath::GetPath()
{
	return std::string();
}

ChannelPath::ChannelPath()
{
}

ChannelPath::~ChannelPath()
{
}

