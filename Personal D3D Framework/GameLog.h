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

//Since it's an int we're using we have 32 possible channels. Sweet!
enum DebugChannel
{
	All = -1,
	Main = 1,
	Rendering = 2,
	Audio = 4,
	Input = 8,
	Events = 16,
	Physics = 32,
	Unused2 = 64,
	Unused3 = 128,
};

class GameLog
{
public:
	~GameLog();
	static GameLog* GetInstance();
	void Log(const int logChannel, const DebugLevel logLevel, const char* message, ...);
	int logVerbosity;
	int writeToLogVerbosity;
	int activeChannels;
private:
	GameLog();
	static GameLog* m_pInstance;
	char* AppendNewlineChar(const char* input);
};

