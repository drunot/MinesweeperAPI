#ifndef C4B8195C_0E5C_41BF_AF27_D93C7ACEB385
#define C4B8195C_0E5C_41BF_AF27_D93C7ACEB385
#include <windows.h>
#include <tlhelp32.h>
#include <cstdio>
#include <synchapi.h>
#include <iostream>
#ifdef WIN_X86
#define THREAD_WAIT 1000

#pragma pack(push,1)
struct StartProcAndInject {
    HMODULE hModule;
    LPCWCHAR functionName;
};
#pragma pack(pop)
class startProcAndInject {
  private:
    HANDLE hProcess = nullptr;
    HMODULE hLibrary = nullptr;


  public:
    startProcAndInject(LPCWCHAR exeName, LPCWCHAR dllName) {
        //char temp[255];
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        STARTUPINFOW process_startup_info{ 0 };
        process_startup_info.cb = sizeof(process_startup_info); // setup size of strcture in bytes
        PROCESS_INFORMATION process_info{ 0 };
        wchar_t* commandline_args = new wchar_t[lstrlenW(exeName) + 1];
        std::memcpy(commandline_args, exeName, (lstrlenW(exeName) + 1) * sizeof(wchar_t));
        //std::printf("Opening \"%ls\"\n", commandline_args);

        if(!CreateProcessW(NULL, commandline_args, NULL, NULL, TRUE, 0, NULL, NULL, &process_startup_info,
                           &process_info)) {
            return;
        }

        hProcess = process_info.hProcess;
        // How many bytes we need to hold the whole DLL path
        int bytesToAlloc = (1 + lstrlenW(dllName)) * sizeof(wchar_t);
        // Allocate memory in the remote process for the DLL path
        LPWSTR remoteBufferForLibraryPath = LPWSTR(VirtualAllocEx(
                                                hProcess, NULL, bytesToAlloc, MEM_COMMIT, PAGE_READWRITE));
        // Put the DLL path into the address space of the target process
        WriteProcessMemory(hProcess, remoteBufferForLibraryPath,
                           dllName, bytesToAlloc, NULL);
        // Get the real address of LoadLibraryW in Kernel32.dll
        LPTHREAD_START_ROUTINE loadLibraryFunction = LPTHREAD_START_ROUTINE(
                    GetProcAddress(GetModuleHandleW(L"Kernel32"), "LoadLibraryW"));
        // Create remote thread that calls LoadLibraryW
        HANDLE remoteThreadHandle = CreateRemoteThread(hProcess,
                                    NULL, 0, loadLibraryFunction, remoteBufferForLibraryPath, 0, NULL);
        WaitForSingleObject(remoteThreadHandle, INFINITE);
        // Get hLibrary which is return from the remote thread.
        DWORD tempHandleModule;
        GetExitCodeThread(remoteThreadHandle, &tempHandleModule);
        hLibrary = reinterpret_cast<HMODULE>(tempHandleModule);
        CloseHandle(remoteThreadHandle);
        // Create a argument stuckt
        struct GetExitCodeThreadArgs {
            HANDLE hLib;
            LPDWORD lpExitCodePtr;
            DWORD lpExitCode;
        };

        // Garbage collection for other process.
        if(!VirtualFreeEx(hProcess, remoteBufferForLibraryPath, bytesToAlloc, MEM_DECOMMIT)) {
            printf("Could not free path arguments. Error was: %d\n", GetLastError());
        }
    }
    ~startProcAndInject() {
        TerminateProcess(hProcess, 0);
        CloseHandle(hProcess);
        CloseHandle(hLibrary);
    }

    bool isProcessRunning() {
        if(hProcess == INVALID_HANDLE_VALUE) {
            return false;
        }

        return WaitForSingleObject(hProcess, 0) == WAIT_TIMEOUT;
    }

    bool wasLoaded() {
        return (hProcess && hLibrary);
    }

};
#endif // WIN_X86
#endif // C4B8195C_0E5C_41BF_AF27_D93C7ACEB385
