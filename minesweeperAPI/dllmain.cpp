// dllmain.cpp : Defines the entry point for the DLL application.
//#include "pch.h"
#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <array>
#include "minesweeperAPI/exported.hpp"
typedef unsigned long long QWORD;

DWORD WINAPI MyThread(LPVOID);
DWORD g_threadID;
HMODULE g_hModule;


BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                     ) {
    switch(ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}

