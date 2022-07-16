#pragma once
#include <memoryapi.h>
#include <processthreadsapi.h>
namespace asm_cmds {
    enum ASM_CMDS {
        CALL_NEAR = 0xE8
    };
}
#pragma pack(1)
struct patch_t {
    BYTE nPatchType;
    DWORD dwAddress;
};
#pragma pack()
BOOL ApplyPatch(BYTE eType, DWORD dwAddress, const void* pTarget) {
    DWORD dwOldValue, dwTemp;
    patch_t pWrite = {
        eType,
        (DWORD)pTarget - (dwAddress + sizeof(DWORD) + sizeof(BYTE))
    };
    VirtualProtect((LPVOID)dwAddress, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &dwOldValue);
    BOOL bSuccess = WriteProcessMemory(GetCurrentProcess(), (LPVOID)dwAddress, &pWrite, sizeof(pWrite),
                                       NULL);
    VirtualProtect((LPVOID)dwAddress, sizeof(DWORD), dwOldValue, &dwTemp);
    return bSuccess;
}