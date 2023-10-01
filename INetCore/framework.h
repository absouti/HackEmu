#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件

#include <windows.h>
#include <strsafe.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdexcept>
#include <exception>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")