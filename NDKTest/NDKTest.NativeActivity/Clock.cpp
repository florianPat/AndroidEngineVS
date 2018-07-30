#include "Clock.h"
#include "stdlib.h"
#include "Utils.h"

Clock::Clock()
{
	srand(time(NULL));
	restart();
}

void Clock::restart()
{
	elapsed = 0.0f;
	firstTime = now();
	lastTime = firstTime;
}

Time Clock::getTime()
{
	update();

	return Time{ elapsed };
}

Time Clock::getElapsedTimeTotal()
{
	update();

	return Time{ elapsedTotal };
}

double Clock::now()
{
	timespec timeVal;
	if (clock_gettime(CLOCK_REALTIME, &timeVal) != 0)
	{
		utilsLogBreak("clock_gettime error!");
	}
	return timeVal.tv_sec + timeVal.tv_nsec / 1000000000;
}

void Clock::update()
{
	double currentTime = now();
	elapsed = currentTime - lastTime;
	elapsedTotal = currentTime - firstTime;
	lastTime = currentTime;
}
