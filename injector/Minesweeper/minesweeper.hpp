#ifndef MINESWEEPER_H_
#define MINESWEEPER_H_
#include <array>
#include <windows.h>

namespace minesweeper {

    [[nodiscard]]
    BOOL __stdcall InjectFunctions();

    // void __stdcall PrintField();

    void __stdcall CheckTile(DWORD x_cord, DWORD y_cord);

    void __stdcall ResetField();

    char __stdcall EndGame(bool won);

    void __stdcall ClickOnField();

    void __stdcall ClickOnTile(DWORD& x_cord, DWORD& y_cord);

    void __stdcall DoubleClickOnField(DWORD& x_cord, DWORD& y_cord);

    DWORD __stdcall GetHight();

    DWORD __stdcall GetWidth();

    BOOL __stdcall gameStarted();

    BYTE* __stdcall getPlayingField();

    DWORD __stdcall getFieldHeight();

    DWORD __stdcall getFieldWidth();

    BOOL __stdcall lastGameWon();

    void __stdcall SetSmily(HDC hdc, DWORD SmilyType);

    void __stdcall loadButtons();
}

#endif // MINESWEEPER_H_