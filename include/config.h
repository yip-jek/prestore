#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <list>
#include "exception.h"

class Config;

// Config item
class CfgItem
{
	friend class Config;

public:
	CfgItem();
	CfgItem(const std::string& segment, const std::string& name, const std::string& val = std::string(), bool find = false);
	virtual ~CfgItem() {}

public:
	friend bool operator == (const CfgItem& item1, const CfgItem& item2)
	{ return (item1.m_segment == item2.m_segment) && (item1.m_name == item2.m_name); }

private:
	std::string		m_segment;
	std::string		m_name;
	std::string		m_value;
	bool			m_bFind;		// This cfg_item be found ?
};

////////////////////////////////////////////////////////////////////////
// Config 
class Config
{
public:
	Config(const std::string& cfg_file = std::string());
	virtual ~Config() {}

public:
	void SetCfgFile(const std::string& cfg_file) throw(Exception);
	bool RegisterItem(std::string segment, std::string name);
	bool UnregisterItem(std::string segment, std::string name);
	void InitItems();
	void DeleteItems();
	void ReadConfig() throw(Exception);
	std::string GetCfgValue(std::string segment, std::string name) throw(Exception);
	float GetCfgFloatVal(const std::string& segment, const std::string& name);
	long long GetCfgLongVal(const std::string& segment, const std::string& name);
	bool GetCfgBoolVal(const std::string& segment, const std::string& name);

private:
	bool FindItem(const std::string& segment, const std::string& name, std::list<CfgItem>::iterator* pItr = NULL);
	bool TryGetSegment(const std::string& str, std::string& segment) const;
	bool TryGetNameValue(const std::string& str, std::string& name, std::string& value) const;
	void CleanComment(std::string& str) const;

private:
	std::string			m_cfgFile;
	std::list<CfgItem>	m_listItems;
};

#endif	// _CONFIG_H_

