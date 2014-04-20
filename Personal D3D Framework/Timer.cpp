#include "Timer.h"


Timer::Timer()
{
	UINT64 countsPerSecond = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
	secsPerCount = 1.0f / (float)countsPerSecond;
}


Timer::~Timer()
{
}

float Timer::elapsedTime()
{
	UINT64 tempCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&tempCount);
	float diff = ((tempCount - startCount) * secsPerCount);
	return diff;
}

void Timer::startTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&startCount);
}

void Timer::stopTimer()
{

}

void Timer::resetTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&startCount);
}