#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include "stdafx.h"

int emush_builtin_nums();
const wchar_t* builtin_cmd[];

int emush_cd(wchar_t** args);
int emush_help(wchar_t** args);
int emush_exit(wchar_t** args);