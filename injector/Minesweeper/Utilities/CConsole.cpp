#include "pch.h"
#include <windows.h>
#include <conio.h>
#include <FCNTL.H>
#include <io.h>
#include "cConsole.hpp"
#include <iostream>

void BindCrtHandlesToStdHandles(bool bindStdIn, bool bindStdOut, bool bindStdErr) {
    // Re-initialize the C runtime "FILE" handles with clean handles bound to "nul". We do this because it has been
    // observed that the file number of our standard handle file objects can be assigned internally to a value of -2
    // when not bound to a valid target, which represents some kind of unknown internal invalid state. In this state our
    // call to "_dup2" fails, as it specifically tests to ensure that the target file number isn't equal to this value
    // before allowing the operation to continue. We can resolve this issue by first "re-opening" the target files to
    // use the "nul" device, which will place them into a valid state, after which we can redirect them to our target
    // using the "_dup2" function.
    if(bindStdIn) {
        FILE* dummyFile;
        freopen_s(&dummyFile, "nul", "r", stdin);
    }

    if(bindStdOut) {
        FILE* dummyFile;
        freopen_s(&dummyFile, "nul", "w", stdout);
    }

    if(bindStdErr) {
        FILE* dummyFile;
        freopen_s(&dummyFile, "nul", "w", stderr);
    }

    // Redirect unbuffered stdin from the current standard input handle
    if(bindStdIn) {
        HANDLE stdHandle = GetStdHandle(STD_INPUT_HANDLE);

        if(stdHandle != INVALID_HANDLE_VALUE) {
            int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);

            if(fileDescriptor != -1) {
                FILE* file = _fdopen(fileDescriptor, "r");

                if(file != NULL) {
                    int dup2Result = _dup2(_fileno(file), _fileno(stdin));

                    if(dup2Result == 0) {
                        setvbuf(stdin, NULL, _IONBF, 0);
                    }
                }
            }
        }
    }

    // Redirect unbuffered stdout to the current standard output handle
    if(bindStdOut) {
        HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        if(stdHandle != INVALID_HANDLE_VALUE) {
            int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);

            if(fileDescriptor != -1) {
                FILE* file = _fdopen(fileDescriptor, "w");

                if(file != NULL) {
                    int dup2Result = _dup2(_fileno(file), _fileno(stdout));

                    if(dup2Result == 0) {
                        setvbuf(stdout, NULL, _IONBF, 0);
                    }
                }
            }
        }
    }

    // Redirect unbuffered stderr to the current standard error handle
    if(bindStdErr) {
        HANDLE stdHandle = GetStdHandle(STD_ERROR_HANDLE);

        if(stdHandle != INVALID_HANDLE_VALUE) {
            int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);

            if(fileDescriptor != -1) {
                FILE* file = _fdopen(fileDescriptor, "w");

                if(file != NULL) {
                    int dup2Result = _dup2(_fileno(file), _fileno(stderr));

                    if(dup2Result == 0) {
                        setvbuf(stderr, NULL, _IONBF, 0);
                    }
                }
            }
        }
    }

    // Clear the error state for each of the C++ standard stream objects. We need to do this, as attempts to access the
    // standard streams before they refer to a valid target will cause the iostream objects to enter an error state. In
    // versions of Visual Studio after 2005, this seems to always occur during startup regardless of whether anything
    // has been read from or written to the targets or not.
    if(bindStdIn) {
    }

    if(bindStdOut) {
        std::wcout.clear();
        std::cout.clear();
    }

    if(bindStdErr) {
        std::wcerr.clear();
        std::cerr.clear();
    }
}


static BOOL WINAPI MyConsoleCtrlHandler(DWORD dwCtrlEvent) {
    return dwCtrlEvent == CTRL_C_EVENT;
}

CConsole::CConsole() : m_OwnConsole(false), m_OldStdin(*stdin), m_OldStdout(*stdout) {
    if(!AllocConsole()) {
        return;
    }

    BindCrtHandlesToStdHandles(true, true, true);
    /*SetConsoleCtrlHandler(MyConsoleCtrlHandler, TRUE);
    RemoveMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_BYCOMMAND);
    const int in = _open_osfhandle(INT_PTR(GetStdHandle(STD_INPUT_HANDLE)), _O_TEXT);
    const int out = _open_osfhandle(INT_PTR(GetStdHandle(STD_OUTPUT_HANDLE)), _O_TEXT);
    *stdin = *_fdopen(in, "r");
    *stdout = *_fdopen(out, "w");
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);*/
    m_OwnConsole = true;
    consoleWindow = FindWindowA("ConsoleWindowClass", NULL);
}

CConsole::~CConsole() {
    printf("Destroied\n");

    if(m_OwnConsole) {
        printf("Destroied\n");
        fclose(stdout);
        fclose(stdin);
        *stdout = m_OldStdout;
        *stdin = m_OldStdin;
        FILE* dummyFile;
        freopen_s(&dummyFile, "nul", "r", stdin);
        freopen_s(&dummyFile, "nul", "w", stdout);
        freopen_s(&dummyFile, "nul", "w", stderr);
        std::wcin.clear();
        std::cin.clear();
        std::wcout.clear();
        std::cout.clear();
        std::wcerr.clear();
        std::cerr.clear();
        //SetConsoleCtrlHandler(MyConsoleCtrlHandler, FALSE);
        m_OwnConsole = false;
        FreeConsole();
    }
}
void CConsole::show() {
    ShowWindow(consoleWindow, 1);
}
void CConsole::hide() {
    ShowWindow(consoleWindow, 0);
}