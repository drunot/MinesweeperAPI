// dllmain.cpp : Defines the entry point for the DLL application.
//#include "pch.h"
#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <array>
#include "Minesweeper/minesweeper.hpp"
#include "rpc/RPCServer.h"
#include "Minesweeper/Utilities/ConsoleLogging.hpp"

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
            g_hModule = hModule;
            DisableThreadLibraryCalls(hModule);
            CreateThread(NULL, NULL, &MyThread, NULL, NULL, &g_threadID);
            startServer();

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}

DWORD WINAPI MyThread(LPVOID) {
    std::srand(std::time(nullptr));
    int myInt = 1;
    DWORD y_cord = 1;
    DWORD x_cord = 0;
    BOOL wasInjected = minesweeper::InjectFunctions();

    if(!wasInjected) {
        exit(1);
    }

    while(true) {
        if(GetAsyncKeyState(VK_INSERT) & 1) {
            if(minesweeper::gameStarted()) {
                minesweeper::ClickOnTile(++x_cord, y_cord);

                if(!(x_cord % minesweeper::GetWidth())) {
                    x_cord = 0;
                    ++y_cord;
                }

                if((y_cord % minesweeper::GetHight()) == 1) {
                    y_cord = 1;
                }
            }

        } else if(GetAsyncKeyState(VK_DELETE) & 1) {
            minesweeper::ResetField();

        } else if(GetAsyncKeyState(VK_HOME) & 1) {
            minesweeper::EndGame(false);

        } else if(GetAsyncKeyState(VK_END) & 1) {
            minesweeper::EndGame(true);

        } else if(GetAsyncKeyState(VK_PAUSE) & 1) {
            break;

        } else if(GetAsyncKeyState(VK_F10) & 1) {
            LOG_SHOW_CONSOLE();

        } else if(GetAsyncKeyState(VK_F9) & 1) {
            LOG_HIDE_CONSOLE();
        }

        Sleep(100);
    }

    FreeLibraryAndExitThread(g_hModule, 0);
    return 0;
}
