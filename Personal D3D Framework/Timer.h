#pragma once
#include "Common.h"

class Timer
{
public:
	Timer();
	~Timer();
	float elapsedTime();
	void startTimer();
	void stopTimer();
	void resetTimer();

private:
	float secsPerCount;
	UINT64 startCount;

};

