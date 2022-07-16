#ifndef B86E5476_803A_4BFF_9F8D_174332DA9F84
#define B86E5476_803A_4BFF_9F8D_174332DA9F84
#include <stdio.h>
#include <windows.h>

class CConsole {
    FILE m_OldStdin, m_OldStdout;
    bool m_OwnConsole;
    HWND consoleWindow;
  public:
    CConsole();
    ~CConsole();
    void show();
    void hide();
};


#endif // B86E5476_803A_4BFF_9F8D_174332DA9F84
