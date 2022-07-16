#include"minesweeperAPI.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    startMinesweeper();
    return 0;
}
