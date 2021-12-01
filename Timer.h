#pragma once

#include <chrono>
#include <string>
#include <iostream>

class Timer {
public:
	explicit Timer(const std::string& name);
	~Timer();

	void Stop();

private:
	std::chrono::high_resolution_clock::time_point start;
	std::string name;
	bool m_Stopped = false;
};