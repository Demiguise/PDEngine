#pragma once
#include "FileManager.h"

//0 None (Don't use for logging) | 1 Normal | 2 Errors | 3 Warnings
enum DebugLevel
{
	None = 0, 
	Normal,
	Error,
	Warning,
};

class FileManager;

class GameLog
{
public:
	~GameLog();
	static GameLog* getInstance();
	void Log(const char* message, const DebugLevel logLevel);
private:
	GameLog();
	static GameLog* m_pInstance;
	char* AppendNewline(const char* input);
};

