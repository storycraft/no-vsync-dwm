#include "sig_tool.h"

namespace SigTool {
	bool SigCompare(const BYTE* data, const BYTE* pattern, const char* mask) {
		for (; *mask; ++mask, ++data, ++pattern) {
			if (*mask == 'x' && *data != *pattern) return false;
		}

		return (*mask) == NULL;
	}

	void* FindPattern(uintptr_t addr, ptrdiff_t data_len, const BYTE* pattern, const char* mask) {
		for (uintptr_t i = 0; i < data_len; i++) {
			if (SigCompare((BYTE*)(addr + i), pattern, mask)) {
				return (void*) (addr + i);
			}
		}

		return NULL;
	}

	void* FindPatternModule(HANDLE process, HMODULE mod, const BYTE* pattern, const char* mask) {
		MODULEINFO info;
		if (!GetModuleInformation(process, mod, &info, sizeof(info))) {
			return NULL;
		}

		MEMORY_BASIC_INFORMATION memInfo;
		if (!VirtualQuery(info.EntryPoint, &memInfo, sizeof(memInfo))) {
			return NULL;
		}

		return FindPattern((size_t)memInfo.AllocationBase, (size_t)info.SizeOfImage - strlen(mask), pattern, mask);
	}

	void* FindPatternModule(const BYTE* pattern, const char* mask) {
		return FindPatternModule(GetCurrentProcess(), NULL, pattern, mask);
	}

	uintptr_t RelativeToAbsoluteAddress(uintptr_t relAddr, void* addr) {
		return relAddr + ((uintptr_t) addr + 4);
	}

	uintptr_t AbsoluteToRelativeAddress(uintptr_t absAddr, void* addr) {
		return absAddr - ((uintptr_t) addr + 4);
	}
}
