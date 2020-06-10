#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <CommCtrl.h>
#include <stdio.h>
#include<iostream>

#define IDC_EDIT 101
static UINT WM_SHELLHOOKMESSAGE= RegisterWindowMessage("SHELLHOOK");    //�Զ�����Ϣ
// static HWND hwndMain;
static HWND hwndEdit;
static struct
{
	UINT uMsg;
	const char* pMsg;
}shell_list[] =
{
	{HSHELL_GETMINRECT,         "HSHELL_GETMINRECT"},
	{HSHELL_WINDOWACTIVATED,    "HSHELL_WINDOWACTIVATED"},
	{HSHELL_RUDEAPPACTIVATED,   "HSHELL_RUDEAPPACTIVATED"},
	{HSHELL_WINDOWREPLACING,    "HSHELL_WINDOWREPLACING"},
	{HSHELL_WINDOWREPLACED,     "HSHELL_WINDOWREPLACED"},
	{HSHELL_WINDOWCREATED,      "HSHELL_WINDOWCREATED"},
	{HSHELL_WINDOWDESTROYED,    "HSHELL_WINDOWDESTROYED"},
	{HSHELL_ACTIVATESHELLWINDOW,"HSHELL_ACTIVATESHELLWINDOW"},
	{HSHELL_TASKMAN,            "HSHELL_TASKMAN"},
	{HSHELL_REDRAW,             "HSHELL_REDRAW"},
	{HSHELL_FLASH,              "HSHELL_FLASH"},
	{HSHELL_ENDTASK,            "HSHELL_ENDTASK"},
	{HSHELL_APPCOMMAND,         "HSHELL_APPCOMMAND"},
	{0,                         NULL}
};
int bmpwrite(HWND hwndNew) {
	HDC _dc = GetWindowDC(hwndNew);
	HDC dc = CreateCompatibleDC(0);

	RECT rect;
	GetClientRect(hwndNew, &rect);

	POINT p;
	p.x = rect.bottom;
	p.y = rect.right;
	std::cout << p.x << p.y << std::endl;
	DPtoLP(_dc, &p, 1);
	std::cout << p.x << p.y << std::endl;

	long w = p.y;
	long h = p.x;
	
	void* buf = new char[w * h * 4];

	HBITMAP bm = CreateCompatibleBitmap(_dc, w, h);
	SelectObject(dc, bm);

	BitBlt(dc, 0, 0, w, h, _dc, 0, 0, SRCCOPY);
	char filename[] = "test.bmp";
	void* f = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, 0);

	GetObject(bm, 84, buf);

	tagBITMAPINFO bi;
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = w;
	bi.bmiHeader.biHeight = h;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = 0;
	bi.bmiHeader.biSizeImage = 0;

	CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &buf, 0, 0);
	GetDIBits(dc, bm, 0, h, buf, &bi, DIB_RGB_COLORS);

	BITMAPFILEHEADER bif;
	bif.bfType = MAKEWORD('B', 'M');
	bif.bfSize = w * h * 4 + 54;
	bif.bfOffBits = 54;

	BITMAPINFOHEADER bii;
	bii.biSize = 40;
	bii.biWidth = w;
	bii.biHeight = h;
	bii.biPlanes = 1;
	bii.biBitCount = 32;
	bii.biCompression = 0;
	bii.biSizeImage = w * h * 4;

	DWORD r;
	WriteFile(f, &bif, sizeof(bif), &r, NULL);
	WriteFile(f, &bii, sizeof(bii), &r, NULL);
	WriteFile(f, buf, w * h * 4, &r, NULL);

	CloseHandle(f);
	DeleteDC(_dc);
	DeleteDC(dc);
	return 1;
}
void AddEditText(const char* szMsg, HWND hwndNew) //�����ı�������
{
	int len;
	char buffer[1024];
	char szCls[128] = { 0 }, szText[128] = { 0 }, szBmp[128] = { 0 };
	GetWindowText(hwndNew, szText, sizeof(szText));
	GetClassName(hwndNew, szCls, sizeof(szCls));
	len = GetWindowTextLength(hwndEdit);
	_snprintf_s(buffer, sizeof(buffer), "Message:\t%s\r\nClassName:\t%s\r\nWindowName:\t%s\r\n%s\n\r\n", szMsg, szCls, szText,szBmp);
	SendMessage(hwndEdit, EM_SETSEL, len, len);
	SendMessage(hwndEdit, EM_REPLACESEL, 0, (LPARAM)buffer);
	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_SHELLHOOKMESSAGE)
	{
		switch (wParam) {
		case HSHELL_WINDOWCREATED:
			AddEditText("CREATE", (HWND)lParam);
			Sleep(500);
			bmpwrite((HWND)lParam);
			
		case HSHELL_REDRAW:
			AddEditText("REDRAW", (HWND)lParam);
			
		}
		/*int i;
		for (i = 0; shell_list[i].pMsg; i++)
		{
			if (shell_list[i].uMsg == wParam)
			{
				AddEditText(shell_list[i].pMsg, (HWND)lParam);
				return 0;
			}
		}*/
		return 0;
	}
	switch (uMsg)
	{
	case WM_SIZE:
	{	// Make the edit control the size of the window's client area. 
		RECT rect;
		GetClientRect(hwnd, &rect);
		MoveWindow(hwndEdit, 0, 0, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		return 0;
	}
	case  WM_CREATE:
	{
		//hwndMain = hwnd;
		hwndEdit = CreateWindowEx(0, WC_EDIT, // predefined class
			NULL,							  // no window title
			WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_HSCROLL | WS_VSCROLL,
			0, 0, 0, 0,						  // set size in WM_SIZE message
			hwnd,							  // parent window	
			(HMENU)IDC_EDIT, ((CREATESTRUCT*)lParam)->hInstance, NULL);

		if (!RegisterShellHookWindow(hwnd))
		{
			MessageBox(hwnd, "SetShellHook, failed.", NULL, MB_OK);
		}

		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		DeregisterShellHookWindow(hwnd);
		DestroyWindow(hwnd);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = "shell_wnd_class";
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc))
		return 1;

	hWnd = CreateWindowEx(WS_EX_TOPMOST, "shell_wnd_class", "Shell Hook Demo", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 460, NULL, NULL, hInstance, NULL);	

	if (!hWnd)
	{
		MessageBox(NULL, "CreateWindowEx Failed!", NULL, MB_OK);
		return 1;
	}

	UpdateWindow(hWnd);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}