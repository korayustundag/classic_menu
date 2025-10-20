#include <Windows.h>
#include <TlHelp32.h>

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

int main()
{
	LPCWSTR cmdLine = GetCommandLineW();
	LPCWSTR arg = cmdLine;
	HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written;

	while (*arg && *arg != ' ')
	{
		arg++;
	}

	while (*arg == ' ')
	{
		arg++;
	}

	if (*arg == 0)
	{
		WriteConsoleW(hnd, L"Usage: classic_menu.exe [on|off]\r\n", (DWORD)35, &written, NULL);
		return 1;
	}

	LPCWSTR keyPath = L"Software\\Classes\\CLSID\\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2}\\InprocServer32";

	if (lstrcmpiW(arg, L"on") == 0)
	{
		HKEY hKey;
		LONG result = RegCreateKeyExW(HKEY_CURRENT_USER, keyPath, (DWORD)0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
		if (result == ERROR_SUCCESS)
		{
			RegSetValueExW(hKey, NULL, 0, REG_SZ, (const BYTE*)L"", 1);
			RegCloseKey(hKey);
			RestartExplorer();
			WriteConsoleW(hnd, L"Classic right click menu enabled.\r\n", (DWORD)36, &written, NULL);
		}
		else
		{
			WriteConsoleW(hnd, L"Error: Failed to create registry key.", (DWORD)38, &written, NULL);
		}
	}
	else if (lstrcmpiW(arg, L"off") == 0)
	{
		LPCWSTR baseKey = L"Software\\Classes\\CLSID\\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2}";
		LONG result = RegDeleteTreeW(HKEY_CURRENT_USER, baseKey);

		if (result == ERROR_SUCCESS)
		{
			RestartExplorer();
			WriteConsoleW(hnd, L"New right click menu enabled.\r\n", (DWORD)32, &written, NULL);
		}
		else
		{
			WriteConsoleW(hnd, L"Could not delete registry key (probably does not exist).\r\n", (DWORD)59, &written, NULL);
		}
	}
	else
	{
		WriteConsoleW(hnd, L"Invalid argument. Usage: classic_menu.exe [on|off]\r\n", (DWORD)53, &written, NULL);
	}

	if (hnd)
	{
		CloseHandle(hnd);
	}
	return 0;
}
