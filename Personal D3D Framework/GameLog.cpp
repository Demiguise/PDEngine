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

}

GameLog::~GameLog()
{
	delete m_pInstance;
}

void GameLog::Log(const char* logLine, const DebugChannel logChannel,  const DebugLevel logLevel)
{
	if (logLevel <= g_logVerbosity)
	{
		char buffer[512];
		va_list args;
		va_start(args, logLine);
		vsnprintf_s(buffer, 512, AppendNewline(logLine), args);
		va_end(args);
		OutputDebugStringA((LPCSTR)buffer);
		if (logLevel <= g_writeToLogVerbosity)
		{
			FileManager::GetInstance()->WriteToLog(buffer);
		}
	}
}

char* GameLog::AppendNewline(const char* input)
{
	int inputSize = strlen(input);
	char* output = new char[inputSize+1];
	memmove(output, input, inputSize);
	output[inputSize] = '\n';
	output[inputSize+1] = '\0';
	return output;
}