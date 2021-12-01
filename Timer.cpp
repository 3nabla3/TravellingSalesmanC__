//
// Created by alban on 11/4/21.
//

#include "Timer.h"

Timer::Timer(const std::string& n) {
	name = n;
	start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {
	Stop();
}

void Timer::Stop() {
	if (!m_Stopped) {
		std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - start;
		std::cout << "[" << name << "] " << duration.count() * 1000.0f << " ms" << std::endl;
		m_Stopped = true;
	}
}
