#include <windows.h>
#include <cstdio>
#include "Minesweeper/minesweeper.hpp"
#include "minesweeperRPC.h"

void RPC_ResetField(void) {
    minesweeper::ResetField();
}

void RPC_GetField(unsigned char* fieldBuffer) {
    auto playingField = minesweeper::getPlayingField();

    for(int x = 0; x < 30; x++) {
        for(int y = 0; y < 24; y++) {
            auto toCopy = playingField[((y + 1) << 5) + (x + 1)];

            if((toCopy & 0x0F) == 0x0E) {
                fieldBuffer[y * 30 + x] = 'F';

            } else if((toCopy & 0x0F) == 0x0D) {
                fieldBuffer[y * 30 + x] = '?';

            } else if((toCopy & 0xF0) == 0x40) {
                if(!(toCopy & 0x0F)) {
                    fieldBuffer[y * 30 + x] = ' ';

                } else {
                    fieldBuffer[y * 30 + x] = '0' + (toCopy & 0x0F);
                }

            } else {
                fieldBuffer[y * 30 + x] = '#';
            }
        }
    }
}

unsigned char RPC_GetFieldHeight(void) {
    return static_cast<unsigned char>(minesweeper::getFieldHeight());
}
unsigned char RPC_GetFieldWidth(void) {
    return static_cast<unsigned char>(minesweeper::getFieldWidth());
}

void RPC_ClickOnTile(unsigned char x, unsigned char y) {
    DWORD x_d = static_cast<DWORD>(x);
    DWORD y_d = static_cast<DWORD>(y);
    minesweeper::ClickOnTile(x_d, y_d);
}

RPC_game_status_t RPC_GetGameStatus(void) {
    if(minesweeper::gameStarted()) {
        return RPC_GAME_RUNNING;
    }

    if(minesweeper::lastGameWon()) {
        return RPC_GAME_WON;
    }

    return RPC_GAME_LOST;
}