#pragma once

#include "base.h"
#include "..\INetCore\core.h"

// ¡Ÿ ± π”√
#ifdef _DEBUG
#pragma comment(lib, R"(..\x64\Debug\INetCore.lib)")
#endif _DEBUG

// Timer
struct Timer {
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<double> duration;
	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;
		double ms = duration.count() * 1000.0;
		std::cout << "Timer took " << ms << "ms" << std::endl;
	}
};