#pragma once
#include "Windows.h"
#include "tlhelp32.h"
#include <vector>

DWORD getProcID(const wchar_t* procName);
uintptr_t getModuleBaseAddress(DWORD procID, const wchar_t* modName);
uintptr_t findDMAAddress(HANDLE handle, uintptr_t ptr, std::vector<unsigned int> offsets);