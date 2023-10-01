#pragma once

#ifdef INETCORE_EXPORTS
#define INETCORE_API __declspec(dllexport)
#else
#define INETCORE_API __declspec(dllimport)
#endif

INETCORE_API int ClientOpen(PCSTR pNodeName, PCSTR pDesPort);
//INETCORE_API int ClientClose(HINTERNET hInternet);
DWORD ErrorFunc(LPTSTR lpszFunction);