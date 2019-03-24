#pragma once

#include "Clock.h"
#include "String.h"

class Benchmark
{
	bool running = false;
	Clock clock;
	String benchmarkName;

	static Benchmark singleton;
	Benchmark() = default;
public:
	void start(const String& benchmarkNameIn);
	void stop();
	//NOTE: Not thread safe!
	static Benchmark& getBenchmark();
};