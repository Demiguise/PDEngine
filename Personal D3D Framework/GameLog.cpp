#include "GameLog.h"

GameLog* GameLog::m_pInstance = 0;

GameLog* GameLog::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new GameLog;
	}
	return m_pInstance;
}

GameLog::GameLog()
{
	logVerbosity = 3;
	writeToLogVerbosity = 3;
	activeChannels = 1; //-1 turns on all bits, useful for activating all 32 channels.
}

GameLog::~GameLog()
{
	delete m_pInstance;
}

//Always use a _SINGLE_ channel.
void GameLog::Log(const char* logLine, const int logChannel,  const DebugLevel logLevel)
{
	if (logLevel <= logVerbosity)
	{
		if ((activeChannels | logChannel) == activeChannels)
		{
			char buffer[512];
			char* newLogLine = AppendNewlineChar(logLine);
			va_list args;
			va_start(args, logLine);
			vsnprintf_s(buffer, 512, newLogLine, args);
			va_end(args);
			OutputDebugStringA((LPCSTR)buffer);
			if (logLevel <= writeToLogVerbosity)
			{
				FileManager::GetInstance()->WriteToLog(buffer);
			}
			delete newLogLine;
		}
	}
}


char* GameLog::AppendNewlineChar(const char* input)
{
	int inputSize = strlen(input);
	char* output = new char[inputSize+2];
	memmove(output, input, inputSize);
	output[inputSize] = '\n';
	output[inputSize+1] = '\0';
	return output;
}