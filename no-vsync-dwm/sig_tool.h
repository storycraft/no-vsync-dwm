#pragma once

#include <windows.h>
#include <psapi.h>

typedef unsigned char BYTE;

namespace SigTool {
	bool SigCompare(const BYTE* data, const BYTE* pattern, const char* mask);

	void* FindPattern(uintptr_t addr, ptrdiff_t data_len, const BYTE* pattern, const char* mask);

	void* FindPatternModule(HANDLE process, HMODULE mod, const BYTE* pattern, const char* mask);
	void* FindPatternModule(HANDLE process, const BYTE* pattern, const char* mask);
	void* FindPatternModule(const BYTE* pattern, const char* mask);

	uintptr_t RelativeToAbsoluteAddress(uintptr_t relAddr, void* addr);

	uintptr_t AbsoluteToRelativeAddress(uintptr_t absAddr, void* addr);
}
