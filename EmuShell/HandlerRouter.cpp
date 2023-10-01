#include "EmuShell.h"

BOOL WINAPI eHandlerRoutine(
	_In_ DWORD dwCtrlType
) {
	switch (dwCtrlType)
	{
	case CTRL_C_EVENT:
		// 对 ctrl+c 不反应
	case CTRL_BREAK_EVENT:
		// 同上
		return TRUE; // 不调用默认回调函数
		break;
	}
	return FALSE; // 调用默认函数
}