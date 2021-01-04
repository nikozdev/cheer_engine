#pragma once
#ifndef SUPPORT_H
#define SUPPORT_H

#include <string>
#include <iostream>
#include <assert.h>
#include <chrono>

//#include <geom/ch_geom.hpp>

#include "ch_memory.h"
#include "ch_managers.h"

namespace SP
{
	struct Timer
	{
	private:
		std::chrono::time_point<std::chrono::steady_clock> start, end;
		std::chrono::duration<float> duration;
	public:
		//Constructors&Destrutors
		Timer();
		~Timer();

		std::chrono::duration<float> getLifeTime();
	};
}

#endif