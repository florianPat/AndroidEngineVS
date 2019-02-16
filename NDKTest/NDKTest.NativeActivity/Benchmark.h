#pragma once

#include "Clock.h"
#include <string>

class Benchmark
{
	bool running = false;
	Clock clock;
	std::string benchmarkName;

	static Benchmark singleton;
	Benchmark() = default;
public:
	void start(const std::string& benchmarkNameIn);
	void stop();
	//NOTE: Not thread safe!
	static Benchmark& getBenchmark();
};