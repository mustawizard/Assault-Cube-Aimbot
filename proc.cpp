#include "proc.h"

DWORD getProcID(const wchar_t* procName)
{
	DWORD procID = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		// dwSize()?
		procEntry.dwSize = sizeof(procEntry);
		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procID = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procID;
}

uintptr_t getModuleBaseAddress(DWORD procID, const wchar_t* modName)
{
	uintptr_t moduleBaseAddress = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					moduleBaseAddress = (uintptr_t)modEntry.th32ModuleID;
					break;
				}
			} while (Module32First(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return moduleBaseAddress;
}

uintptr_t findDMAAddress(HANDLE handle, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(handle, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}