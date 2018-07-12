#include "TimeManager.h"
#include "Log.h"
#include "stdlib.h"

TimeManager::TimeManager()
{
	srand(time(NULL));
	reset();
}

void TimeManager::reset()
{
	utilsLog("Resetting TimeManager");
	elapsed = 0.0f;
	firstTime = now();
	lastTime = firstTime;
}

void TimeManager::update()
{
	double currentTime = now();
	elapsed = currentTime - lastTime;
	elapsedTotal = currentTime - firstTime;
	lastTime = currentTime;
}

double TimeManager::now()
{
	timespec timeVal;
	clock_gettime(CLOCK_MONOTONIC, &timeVal);
	return timeVal.tv_sec + (timeVal.tv_nsec * 1.0e-9);
}
