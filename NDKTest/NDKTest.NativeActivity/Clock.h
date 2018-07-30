#pragma once

#include "Types.h"
#include <time.h>

struct Time
{
	int64_t nanoseconds;

	inline int64_t asNanoseconds()
	{
		return nanoseconds;
	}

	inline int64_t asMicroseconds()
	{
		return nanoseconds / 1000;
	}

	inline int32_t asMilliseconds()
	{
		return (int32_t)(nanoseconds / 1000000.0);
	}

	inline float asSeconds()
	{
		return (float)(nanoseconds / 1000000000.0);
	}

	static Time seconds(float seconds)
	{
		return Time { (int64_t)(seconds * 1000000000.0) };
	}

	static Time milliseconds(int32_t milliseconds)
	{
		return Time { (int64_t)(milliseconds * 1000000) };
	}

	static Time microseconds(int64_t microseconds)
	{
		return Time{ microseconds * 1000 };
	}

	static Time nanoseconds(int64_t nanoseconds)
	{
		return Time{ nanoseconds };
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
	int64_t now();
	void update();
private:
	double firstTime = 0.0;
	double lastTime = 0.0;
	double elapsed = 0.0f;
	double elapsedTotal = 0.0f;
};