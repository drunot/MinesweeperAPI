#ifndef AF9FE5D4_EC9A_498E_A8A7_6584DEADF6FB
#define AF9FE5D4_EC9A_498E_A8A7_6584DEADF6FB
#include "core.hpp"
typedef /* [public][public] */
enum game_status {
    GAME_RUNNING    = 0,
    GAME_WON    = 1,
    GAME_LOST   = 2
}   game_status_t;


#ifdef WIN_X86
    void WINMINE_API startMinesweeper();
    bool WINMINE_API isMinesweeperRunning();
    void WINMINE_API dealocateMinesweeper();
#endif // WIN_X86
int WINMINE_API ResetField();
void WINMINE_API GetField(unsigned char* fieldBuffer);
unsigned char WINMINE_API GetFieldHeight();
unsigned char WINMINE_API GetFieldWidth();
void WINMINE_API ClickOnTile(unsigned char x, unsigned char y);
int WINMINE_API connect();
int WINMINE_API disconnect();
game_status_t WINMINE_API GetGameStatus();
#endif // AF9FE5D4_EC9A_498E_A8A7_6584DEADF6FB
