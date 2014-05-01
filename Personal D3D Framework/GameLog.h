#pragma once
#include "FileManager.h"

//0 None (Don't use for logging) | 1 Normal | 2 Errors | 3 Warnings
enum DebugLevel
{
	None = 0, 
	Normal,
	Error,
	Warning
};

enum DebugChannel
{
	Main = 0,
	Rendering,
	Audio,
	Input,
	Events
};

class GameLog
{
public:
	~GameLog();
	static GameLog* GetInstance();
	void Log(const char* message, const DebugChannel logChannel, const DebugLevel logLevel);
private:
	GameLog();
	static GameLog* m_pInstance;
	char* AppendNewline(const char* input);
};

