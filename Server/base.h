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

#ifdef _DEBUG
#define CONSOLELOG OutputDebugString
#else
#define CONSOLELOG __noop
#endif