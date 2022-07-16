#include <windows.h>
#include <cstdio>
#include "exported.hpp"
#include "startProcAndInject.hpp"
extern "C" {
#include "minesweeperRPC.h"
}
static unsigned char* pszStringBinding    = NULL;

#ifdef WIN_X86
static startProcAndInject* proc;
void startMinesweeper() {
    proc = new startProcAndInject(L"winmine.exe", L"injector.dll");
}

void dealocateMinesweeper() {
    delete proc;
}

bool isMinesweeperRunning() {
    return proc->isProcessRunning();
}
#endif // WIN_X86
void GetField(unsigned char* fieldBuffer) {
    RPC_GetField(fieldBuffer);
}
unsigned char GetFieldHeight() {
    return RPC_GetFieldHeight();
}
unsigned char GetFieldWidth() {
    return RPC_GetFieldWidth();
}
void ClickOnTile(unsigned char x, unsigned char y) {
    RPC_ClickOnTile(x, y);
}

int ResetField() {
    unsigned long ulCode;
    RpcTryExcept {
        RPC_ResetField();
    }
    RpcExcept(1) {
        ulCode = RpcExceptionCode();
        return ulCode;
    }
    RpcEndExcept
    return 0;
}

int connect() {
    RPC_STATUS status;
    unsigned char* pszUuid              = NULL;
    unsigned char pszProtocolSequence[] = "ncacn_np";
    unsigned char* pszNetworkAddress    = NULL;
    unsigned char pszEndpoint[]         = "\\pipe\\minesweeper";
    unsigned char* pszOptions           = NULL;
    status = RpcStringBindingCompose(pszUuid,
                                     pszProtocolSequence,
                                     pszNetworkAddress,
                                     pszEndpoint,
                                     pszOptions,
                                     &pszStringBinding);

    if(status) {
        return status;
    }

    status = RpcBindingFromStringBinding(pszStringBinding, &minesweeper_IfHandle);

    if(status) {
        return status;
    }

    return 0;
}

int disconnect() {
    RPC_STATUS status;
    status = RpcStringFree(&pszStringBinding);

    if(status) {
        return status;
    }

    status = RpcBindingFree(&minesweeper_IfHandle);

    if(status) {
        return status;
    }

    return 0;
}

game_status_t GetGameStatus() {
    return static_cast<game_status_t>(RPC_GetGameStatus());
}

/******************************************************/
/*         MIDL allocate and free                     */
/******************************************************/

void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len) {
    return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR* ptr) {
    free(ptr);
}
