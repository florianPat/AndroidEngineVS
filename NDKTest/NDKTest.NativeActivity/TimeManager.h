#pragma once

#include "Types.h"
#include <time.h>

class TimeManager
{
public:
	TimeManager();

	void reset();
	void update();

	double now();
	float getElapsed() const { return elapsed; }
	float getElapsedTotal() const { return elapsedTotal; };
private:
	double firstTime = 0.0f;
	double lastTime = 0.0f;
	float elapsed = 0.0f;
	float elapsedTotal = 0.0f;
};