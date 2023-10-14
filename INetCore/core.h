#pragma once

#include "pch.h"
#include "macro.h"

#ifdef INETCORE_EXPORTS
#define INETCORE_API __declspec(dllexport)
#else
#define INETCORE_API __declspec(dllimport)
#endif

INETCORE_API int ClientOpen(PCSTR pNodeName, PCSTR pDesPort);
//INETCORE_API int ClientClose(HINTERNET hInternet);

INETCORE_API int ServerOpen(const char* lpDeskPort);

DWORD ErrorFunc(LPTSTR lpszFunction);
int _InitWinsock();
int _ConnectSocket(SOCKET ConnectSocket, addrinfo* ptr);
SOCKET _CreateSocket(PCSTR pNodeName, _Out_ addrinfo** addr, PCSTR DesPort);