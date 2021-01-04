#include <support/ch_support.hpp>

SP::Timer::Timer() :
	duration(0.0f)
{
	start = end = std::chrono::high_resolution_clock::now();
}

SP::Timer::~Timer()
{
	duration = getLifeTime();

	float ms = duration.count() * 1000.0f;
	std::cout << "\t---/" << ms << "ms left\\---" << std::endl;
}

std::chrono::duration<float> SP::Timer::getLifeTime()
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	return duration;
}