#include "EmuShell.h"

BOOL WINAPI eHandlerRoutine(
	_In_ DWORD dwCtrlType
) {
	switch (dwCtrlType)
	{
	case CTRL_C_EVENT:
		// �� ctrl+c ����Ӧ
		return TRUE; // ������Ĭ�ϻص�����
		break;
	case CTRL_BREAK_EVENT:
		// ͬ��
		return TRUE;
		break;
	}
	return FALSE; // ����Ĭ�Ϻ���
}