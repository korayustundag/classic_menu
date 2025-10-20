#include <Windows.h>
#include <CommCtrl.h>
#include <TlHelp32.h>
#include "resource.h"

#define ID_BTN_ON  1001
#define ID_BTN_OFF 1002

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

void RestartExplorer(void)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}

	PROCESSENTRY32W pe;
	pe.dwSize = sizeof(pe);

	if (Process32First(hSnap, &pe))
	{
		do
		{
			if (lstrcmpiW(pe.szExeFile, L"explorer.exe") == 0)
			{
				HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
				if (hProc)
				{
					TerminateProcess(hProc, 0);
					CloseHandle(hProc);
				}
			}
		} while (Process32NextW(hSnap, &pe));
	}

	CloseHandle(hSnap);

	WCHAR windir[MAX_PATH];
	GetEnvironmentVariableW(L"WINDIR", windir, MAX_PATH);
	WCHAR explorerPath[MAX_PATH];
	lstrcpyW(explorerPath, windir);
	lstrcatW(explorerPath, L"\\explorer.exe");

	STARTUPINFOW si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof(si);
	CreateProcessW(explorerPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	if (pi.hProcess)
	{
		CloseHandle(pi.hProcess);
	}

	if (pi.hThread)
	{
		CloseHandle(pi.hThread);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        InitCommonControls();

        HFONT hFont = CreateFontW(
            16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Segoe UI"
        );

        HWND hBtnOn = CreateWindowW(L"BUTTON", L"Enable Classic Menu",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            25, 25, 230, 45, hwnd, (HMENU)ID_BTN_ON, NULL, NULL);

        HWND hBtnOff = CreateWindowW(L"BUTTON", L"Disable Classic Menu",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            25, 80, 230, 45, hwnd, (HMENU)ID_BTN_OFF, NULL, NULL);

        SendMessageW(hBtnOn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessageW(hBtnOff, WM_SETFONT, (WPARAM)hFont, TRUE);
    }
    break;

    case WM_CTLCOLORBTN:
    {
        HDC hdc = (HDC)wParam;
        HWND hwndCtrl = (HWND)lParam;
        SetBkColor(hdc, RGB(255, 255, 255));
        static HBRUSH hBrush = NULL;
        if (!hBrush)
        {
            hBrush = CreateSolidBrush(RGB(255, 255, 255));
        }
        return (LRESULT)hBrush;
    }
    break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BTN_ON)
        {
            HKEY hKey;
            LPCWSTR keyPath = L"Software\\Classes\\CLSID\\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2}\\InprocServer32";
            LONG result = RegCreateKeyExW(HKEY_CURRENT_USER, keyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);

            if (result == ERROR_SUCCESS)
            {
                RegSetValueExW(hKey, NULL, 0, REG_SZ, (const BYTE*)L"", sizeof(WCHAR));
                RegCloseKey(hKey);
                int answer = MessageBoxW(hwnd, L"Classic right-click menu enabled.\r\nShould Explorer restart automatically?", L"Successful", MB_YESNO | MB_ICONQUESTION);
                if (answer == IDYES)
                {
                    RestartExplorer();
                }
            }
            else
            {
                MessageBoxW(hwnd, L"Failed to create registry key.", L"Error", MB_OK | MB_ICONERROR);
            }
        }
        else if (LOWORD(wParam) == ID_BTN_OFF)
        {
            LPCWSTR baseKey = L"Software\\Classes\\CLSID\\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2}";
            LONG result = RegDeleteTreeW(HKEY_CURRENT_USER, baseKey);

            if (result == ERROR_SUCCESS)
            {
                int answer = MessageBoxW(hwnd, L"The new right-click menu has been brought back.\r\nShould Explorer restart automatically?", L"Successful", MB_YESNO | MB_ICONQUESTION);
                if (answer == IDYES)
                {
                    RestartExplorer();
                }
            }
            else
            {
                MessageBoxW(hwnd, L"Could not delete key (probably doesn't exist anyway).", L"Info", MB_OK | MB_ICONWARNING);
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(icex);
    icex.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex);

    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
    wc.lpszClassName = L"ClassicMenuTool";
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wc.hIcon = LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_ICON1));

    RegisterClassW(&wc);

    int x = (GetSystemMetrics(SM_CXSCREEN) - 290) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - 180) / 2;

    HWND hwnd = CreateWindowExW(0, L"ClassicMenuTool", L"Windows 11 Classic Menu Tool",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU ,
        x, y, 290, 180, NULL, NULL, wc.hInstance, NULL);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
