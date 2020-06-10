#include "pch.h"
#incldue "syshookproc.h"

HHOOK hhk;	//this parameter is ignored
LRESULT SysCBTProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode < 0) return CallNextHookEx(hhk, nCode, wParam, lParam);
	if (nCode == HCBT_CREATEWND) {
		/*CHAR szCode[128];
		HRESULT hResult = StringCchCopyA(szCode, 128 / sizeof(TCHAR), "HCBT_CREATEWND");*/
		std::cout << "create win" << std::endl;
	}
	return CallNextHookEx(hhk, nCode, wParam, lParam);
}