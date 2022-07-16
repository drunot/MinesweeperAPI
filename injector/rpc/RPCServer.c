#include <windows.h>
#include <processthreadsapi.h>
#include "RPCServer.h"
#include "minesweeperRPC.h"

DWORD WINAPI serverRuntime(LPVOID);

static DWORD threadID;
static HANDLE hThread;

static unsigned char serverRunning = 0;

void stopServer(void) {
    serverRunning = 0;
    DWORD status = 0;

    //Eqvivalent to join.
    do {
        GetExitCodeThread(hThread, &status);
    } while(status == STILL_ACTIVE);
}

void startServer(void) {
    hThread = CreateThread(NULL, 0, serverRuntime, NULL, 0, &threadID);
}

DWORD WINAPI serverRuntime(LPVOID fo) {
    if(serverRunning) {
        return 1;
    }

    RPC_STATUS status;
    unsigned char* pszProtocolSequence = "ncacn_np";
    unsigned char* pszSecurity         = NULL;
    unsigned char* pszEndpoint         = "\\pipe\\minesweeper";
    unsigned int    cMinCalls = 1;
    unsigned int    fDontWait = FALSE;
    status = RpcServerUseProtseqEp(pszProtocolSequence,
                                   RPC_C_LISTEN_MAX_CALLS_DEFAULT,
                                   pszEndpoint,
                                   pszSecurity);

    if(status) {
        exit(status);
    }

    status = RpcServerRegisterIf(minesweeper_v1_0_s_ifspec,
                                 NULL,
                                 NULL);

    if(status) {
        return status;
    }

    serverRunning = 1;

    while(serverRunning) {
        status = RpcServerListen(cMinCalls,
                                 RPC_C_LISTEN_MAX_CALLS_DEFAULT,
                                 fDontWait);

        if(status) {
            serverRunning = 0;
            return status;
        }
    }

    return 0;
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