#include "config.h"
#include <fstream>
#include "def.h"
#include "helper.h"

// class CfgItem
CfgItem::CfgItem()
:m_bFind(false)
{
}

CfgItem::CfgItem(const std::string& segment, const std::string& name, const std::string& val /*= std::string()*/, bool find /*= false*/)
:m_segment(segment)
,m_name(name)
,m_value(val)
,m_bFind(find)
{
}

////////////////////////////////////////////////////////////////////////

// class Config
Config::Config(const std::string& cfg_file /*= std::string()*/)
{
	//SetCfgFile(cfg_file);
	if ( !cfg_file.empty() && Helper::IsRegularFile(cfg_file) )
	{
		m_cfgFile = cfg_file;
	}
}

void Config::SetCfgFile(const std::string& cfg_file) throw(Exception)
{
	if ( cfg_file.empty() )
	{
		throw Exception(CFG_FILE_INVALID, "The configuration file_path is empty! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	if ( !Helper::IsRegularFile(cfg_file) )
	{
		throw Exception(CFG_FILE_INVALID, "The configuration file \"%s\" is not a regular file! [FILE:%s, LINE:%d]", cfg_file.c_str(), __FILE__, __LINE__);
	}

	m_cfgFile = cfg_file;
}

bool Config::RegisterItem(std::string segment, std::string name)
{
	Helper::Trim(segment);
	Helper::Upper(segment);

	Helper::Trim(name);
	Helper::Upper(name);

	if ( FindItem(segment, name) )
	{
		return false;
	}
	
	m_listItems.push_back(CfgItem(segment, name));
	return true;
}

bool Config::UnregisterItem(std::string segment, std::string name)
{
	Helper::Trim(segment);
	Helper::Upper(segment);

	Helper::Trim(name);
	Helper::Upper(name);

	std::list<CfgItem>::iterator it;
	if ( FindItem(segment, name, &it) )
	{
		m_listItems.erase(it);
		return true;
	}

	return false;
}

void Config::InitItems()
{
	for ( std::list<CfgItem>::iterator it = m_listItems.begin(); it != m_listItems.end(); ++it )
	{
		it->m_value.clear();
		it->m_bFind = false;
	}
}

void Config::DeleteItems()
{
	m_listItems.clear();
}

void Config::ReadConfig() throw(Exception)
{
	if ( m_cfgFile.empty() )
	{
		throw Exception(CFG_FILE_INVALID, "The configuration file unset! [FILE:%s, LINE:%d]", __FILE__, __LINE__);
	}

	std::fstream m_fsCfg;
	m_fsCfg.open(m_cfgFile.c_str(),std::ios_base::in);
	if ( !m_fsCfg.is_open() || !m_fsCfg.good() )
	{
		m_fsCfg.close();
		throw Exception(CFG_OPEN_FILE_FAIL, "Open configuration file \"%s\" fail! [FILE:%s, LINE:%d]", m_cfgFile.c_str(), __FILE__, __LINE__);
	}

	std::string strLine;
	std::string strSegment;
	std::string strName;
	std::string strValue;
	while ( !m_fsCfg.eof() )
	{
		strLine.clear();
		std::getline(m_fsCfg, strLine);

		CleanComment(strLine);
		Helper::Trim(strLine);
		if ( strLine.empty() )
		{
			continue;
		}

		if ( TryGetSegment(strLine, strSegment) )
		{
			continue;
		}

		if ( TryGetNameValue(strLine, strName, strValue) )
		{
			std::list<CfgItem>::iterator it;
			if ( FindItem(strSegment, strName, &it) )
			{
				it->m_value = strValue;
				it->m_bFind = true;
			}
		}
	}

	m_fsCfg.close();
}

std::string Config::GetCfgValue(std::string segment, std::string name) throw(Exception)
{
	Helper::Trim(segment);
	Helper::Upper(segment);

	Helper::Trim(name);
	Helper::Upper(name);

	std::list<CfgItem>::iterator it;
	if ( FindItem(segment, name, &it) )
	{
		if ( !(it->m_bFind) )
		{
			throw Exception(CFG_ITEM_NOT_FOUND, "Configure item [%s->%s] not found! [FILE:%s, LINE:%d]", segment.c_str(), name.c_str(), __FILE__, __LINE__);
		}
		else if ( it->m_value.empty() )
		{
			throw Exception(CFG_VALUE_INVALID, "Configure item [%s->%s] value is invalid! [FILE:%s, LINE:%d]", segment.c_str(), name.c_str(), __FILE__, __LINE__);
		}

		return it->m_value;
	}

	throw Exception(CFG_UNREGISTER_ITEM, "Configure item [%s->%s] unregistered! [FILE:%s, LINE:%d]", segment.c_str(), name.c_str(), __FILE__, __LINE__);
	return std::string();
}

float Config::GetCfgFloatVal(const std::string& segment, const std::string& name)
{
	return Helper::Str2Float(GetCfgValue(segment, name));
}

long long Config::GetCfgLongVal(const std::string& segment, const std::string& name)
{
	return Helper::Str2LLong(GetCfgValue(segment, name));
}

bool Config::GetCfgBoolVal(const std::string& segment, const std::string& name)
{
	std::string strBool = GetCfgValue(segment, name);
	Helper::Upper(strBool);
	return (std::string("TRUE") == strBool) || (std::string("YES") == strBool);
}

bool Config::FindItem(const std::string& segment, const std::string& name, std::list<CfgItem>::iterator* pItr /*= NULL*/)
{
	const CfgItem ITEM(segment, name);
	for ( std::list<CfgItem>::iterator it = m_listItems.begin(); it != m_listItems.end(); ++it )
	{
		if ( ITEM == *it )
		{
			if ( pItr != NULL )
			{
				*pItr = it;
			}
			return true;
		}
	}
	
	return false;
}

bool Config::TryGetSegment(const std::string& str, std::string& segment) const
{
	const std::string::size_type SIZE = str.size();
	if ( SIZE <= 2 )
	{
		return false;
	}

	if ( str[0] == '[' && str[SIZE-1] == ']' )
	{
		segment = str.substr(1,SIZE-2);
		Helper::Trim(segment);
		Helper::Upper(segment);
		return true;
	}
	return false;
}

bool Config::TryGetNameValue(const std::string& str, std::string& name, std::string& value) const
{
	const std::string::size_type SIZE = str.size();
	if ( SIZE < 3 )
	{
		return false;
	}

	std::string::size_type equal_pos = str.find('=');
	if ( equal_pos != std::string::npos && equal_pos > 0 )
	{
		name = str.substr(0,equal_pos);
		Helper::Trim(name);
		Helper::Upper(name);

		value = str.substr(equal_pos+1);
		Helper::Trim(value);
		return true;
	}
	return false;
}

void Config::CleanComment(std::string& str) const
{
	if ( str.empty() )
	{
		return;
	}

	std::string::size_type first_comm_pos = str.find('#');
	std::string::size_type sec_comm_pos = str.find("//");
	first_comm_pos = first_comm_pos < sec_comm_pos ? first_comm_pos : sec_comm_pos;

	if ( first_comm_pos != std::string::npos )
	{
		str.erase(first_comm_pos);
	}
}

