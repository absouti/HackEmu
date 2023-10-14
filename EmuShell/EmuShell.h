#pragma once

#include "stdafx.h"
#include "audio.h"

#include "..\INetCore\core.h"

// ¡Ÿ ± π”√
#pragma comment(lib, R"(..\x64\Debug\INetCore.lib)")

#define SH_FAIL 2
#define SH_SUCCESS 1

int emush_builtin_nums();
const wchar_t* builtin_cmd[];

int emush_cd(std::vector<std::wstring>& args);
int emush_help(std::vector<std::wstring>& args);
int emush_exit(std::vector<std::wstring>& args);
int ConnectServer(std::vector<std::wstring>& args);

BOOL WINAPI eHandlerRoutine(
	_In_ DWORD dwCtrlType
);

DWORD ErrorFunc(LPTSTR lpszFunction);