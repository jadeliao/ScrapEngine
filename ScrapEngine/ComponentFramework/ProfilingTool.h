#ifndef PROFILINGTOOL_H
#define PROFILINGTOOL_H

#include <iostream>
#include <chrono>

class ProfilingTool{

private:
	const char* toolName;
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	bool stopped;
	float duration;

public:

	//ProfilingTool() : toolName("None"), stopped(false) {}

	ProfilingTool(const char* toolName_): toolName(toolName_), stopped(false), duration(0.0f) {}
	~ProfilingTool(){}

	void Start() {
		startTime = std::chrono::high_resolution_clock().now();
	}

	void Stop() {
		auto endTime = std::chrono::high_resolution_clock::now();
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(startTime).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();
		stopped = true;

		duration = (end - start) * 0.001f;
		//std::cout << toolName << ": " << duration << "\n";
	}

	float getDuration() { return duration; }
	const char* getName() { return toolName; }
};


#endif

