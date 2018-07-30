#pragma once

#include "Types.h"
#include <time.h>

struct Time
{
	double nanoseconds;

	inline double asNanoseconds()
	{
		return nanoseconds;
	}

	inline double asMicroseconds()
	{
		return nanoseconds / 1000.0;
	}

	inline float asMilliseconds()
	{
		return (float)(nanoseconds / 1000000.0);
	}

	inline float asSeconds()
	{
		return (float)(nanoseconds / 1000000000.0);
	}
};

class Clock
{
public:
	Clock();

	void restart();
	Time getTime();

	Time getElapsedTimeTotal();
private:
	double now();
	void update();
private:
	double firstTime = 0.0;
	double lastTime = 0.0;
	double elapsed = 0.0f;
	double elapsedTotal = 0.0f;
};