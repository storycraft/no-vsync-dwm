#include <windows.h>
#include <detours.h>
#include "sig_tool.h"

typedef int (__fastcall* WaitForVsync)(void*, unsigned int);
WaitForVsync OriginalWaitForVsync;

int __fastcall PatchedWaitForVsync(void* self, unsigned int a)
{
    return 0;
}

void PatchDwm(HMODULE hModule)
{
    void* addr = SigTool::FindPatternModule(
        GetCurrentProcess(),
        GetModuleHandleA("dwmcore.dll"),
        (BYTE*)"\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x30\x48\x8B\xF1\x8B\xDA\x48\x8B\x49\x40\x48\x8B\x89\xD8\x00\x00\x00",
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
    );

    if (addr)
    {
        OriginalWaitForVsync = (WaitForVsync)addr;
        DetourRestoreAfterWith();
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)OriginalWaitForVsync, PatchedWaitForVsync);
        DetourTransactionCommit();

        MessageBox(NULL, L"Patched", L"NoVsyncDwm", MB_OK);
    }
    else {
        MessageBox(NULL, L"Patch failed", L"NoVsyncDwm", MB_OK | MB_ICONERROR);
        FreeLibrary(hModule);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        PatchDwm(hModule);
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

