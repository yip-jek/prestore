#ifndef _INPUT_H_
#define _INPUT_H_

#include <dirent.h>
#include <string>
#include "exception.h"

class Input
{
public:
	Input(const std::string& paths);
	virtual ~Input();

public:
	void Init() throw(Exception) = 0;

private:
	std::string m_paths;
};

// Input for MQ
class InputMQ : public Input
{
public:
	InputMQ(const std::string& paths);
	virtual ~InputMQ();

public:
	void Init() throw(Exception);
};

// Input for path
class InputPath : public Input
{
public:
	InputPath(const std::string& paths);
	virtual ~InputPath();

public:
	void Init() throw(Exception);
};

#endif	// _INPUT_H_

