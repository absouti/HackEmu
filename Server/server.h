#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <combaseapi.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;
#include "..\INetCore\core.h"

// ¡Ÿ ± π”√
#pragma comment(lib, R"(..\x64\Debug\INetCore.lib)")

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