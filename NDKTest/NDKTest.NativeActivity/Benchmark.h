#pragma once

#include "Clock.h"
#include "String.h"

class Benchmark
{
	bool running = false;
	Clock clock;
	String benchmarkName;

	Benchmark() = default;
public:
	void start(const String& benchmarkNameIn);
	void stop();

	static Benchmark getBenchmark();
};