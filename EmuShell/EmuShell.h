#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include "stdafx.h"

int emush_builtin_nums();
const wchar_t* builtin_cmd[];

int emush_cd(std::vector<std::wstring>& args);
int emush_help(std::vector<std::wstring>& args);
int emush_exit(std::vector<std::wstring>& args);