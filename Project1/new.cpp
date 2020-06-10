#include<iostream>
#include<Windows.h>
#include<WinUser.h>

using namespace std;
HHOOK hook;
LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		cout << "click" << endl;
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
}
int main() {
	MSG msg;
	//hook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)mouseProc, GetModuleHandleW(0), 0);
	/*while (GetMessageW(&msg, 0, 0, 0)) DispatchMessageW(&msg);
	cout<<GetMessage(&msg, hWnd, 0, 0);
	cout << msg.message << endl;
	LRESULT CALLBACK WindowProc(hWnd, uMsg, 0, 0);
	*/
	HOOKPROC hkprcSysMsg;
	static HINSTANCE hinstDLL;
	static HHOOK hhookSysMsg;

	hinstDLL = LoadLibrary(TEXT("../Debug/SysHookProc.dll"));
	if (hinstDLL == NULL) exit(-1);
	hkprcSysMsg = (HOOKPROC)GetProcAddress(hinstDLL, "SysCBTProc");
	if (hkprcSysMsg == NULL) exit(-1);
	hhookSysMsg = SetWindowsHookEx(
		WH_CBT,
		hkprcSysMsg,
		hinstDLL,
		0);	
	HWND hWnd = GetDesktopWindow();
	while (true) {
		if (!GetMessage(&msg, hWnd, 0, 0)) break;
		DispatchMessage(&msg);
		TranslateMessage(&msg);
	}
	UnhookWindowsHookEx(hhookSysMsg);
}
//HHOOK g_Hook;
//LRESULT CALLBACK LowLevelKeyboardProc(INT nCode, WPARAM wParam, LPARAM lParam)
//{
//	KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*)lParam;
//	BOOL bControlKeyDown = 0;
//
//	switch (nCode)
//	{
//	case HC_ACTION:
//	{
//		// 检查是否按了ctrl键 
//		bControlKeyDown = GetAsyncKeyState(VK_CONTROL) >> ((sizeof(SHORT) * 8) - 1);
//
//		//Disable CTRL+ESC
//		if (pkbhs->vkCode == VK_ESCAPE && bControlKeyDown)
//			return 1;
//		if (wParam == WM_KEYUP)
//			printf("%c", pkbhs->vkCode);
//
//		break;
//	}
//	}
//	return CallNextHookEx(g_Hook, nCode, wParam, lParam); //回调
//	//return 1;
//}
//
//
//int main(int argc, char* argv[])
//{
//	MSG msg;
//	g_Hook = (HHOOK)SetWindowsHookEx(WH_KEYBOARD_LL,
//		(HOOKPROC)LowLevelKeyboardProc, GetModuleHandleW(0), 0);
//	while (GetMessageW(&msg, 0, 0, 0))DispatchMessageW(&msg);
//	return 0;
//}