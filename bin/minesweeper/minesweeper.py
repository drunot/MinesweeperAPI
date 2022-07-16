import sys
import subprocess
import ctypes
import pathlib
from enum import Enum


class Minesweeper:
    def __init__(self):
        minesweeperAPI = 0
        self.__MineProc = None
        self.__is_64bit = sys.maxsize > 2 ** 32
        if self.__is_64bit:

            minesweeperAPI = ctypes.WinDLL(
                str(pathlib.Path("minesweeperAPIx64.dll").absolute())
            )
        else:
            minesweeperAPI = ctypes.WinDLL(
                str(pathlib.Path("minesweeperAPIx86.dll").absolute())
            )

        c_intReturn_noArgs = ctypes.WINFUNCTYPE(ctypes.c_int)
        c_noReturn_ptrArgs = ctypes.WINFUNCTYPE(ctypes.c_void_p, ctypes.c_char_p)
        c_ucharReturn_noArgs = ctypes.WINFUNCTYPE(ctypes.c_char)
        c_boolReturn_noArgs = ctypes.WINFUNCTYPE(ctypes.c_bool)
        c_noReturn_twoCharArgs = ctypes.WINFUNCTYPE(
            ctypes.c_void_p, ctypes.c_char, ctypes.c_char
        )
        self.__ClickOnTile = c_noReturn_twoCharArgs(
            ("?ClickOnTile@@YAXEE@Z", minesweeperAPI)
        )
        self.__connect = c_intReturn_noArgs(("?connect@@YAHXZ", minesweeperAPI))
        self.__GetGameStatus = c_intReturn_noArgs(
            ("?GetGameStatus@@YA?AW4game_status@@XZ", minesweeperAPI)
        )
        self.__disconnect = c_intReturn_noArgs(("?disconnect@@YAHXZ", minesweeperAPI))
        self.__ResetField = c_intReturn_noArgs(("?ResetField@@YAHXZ", minesweeperAPI))
        self.__GetField = c_noReturn_ptrArgs(("?GetField@@YAXPEAE@Z", minesweeperAPI))
        self.__GetFieldHeight = c_ucharReturn_noArgs(
            ("?GetFieldHeight@@YAEXZ", minesweeperAPI)
        )
        self.__GetFieldWidth = c_ucharReturn_noArgs(
            ("?GetFieldWidth@@YAEXZ", minesweeperAPI)
        )
        if not self.__is_64bit:
            self.__startMinesweeper = c_noReturn_noArgs(
                ("?startMinesweeper@@YAXXZ", minesweeperAPI)
            )
            self.__dealocateMinesweeper = c_noReturn_noArgs(
                ("?dealocateMinesweeper@@YAXXZ", minesweeperAPI)
            )
            self.__isMinesweeperRunning = c_boolReturn_noArgs(
                ("?isMinesweeperRunning@@YA_NXZ", minesweeperAPI)
            )

    def Connect(self):
        self.__connect()

    def Disconnect(self):
        self.__disconnect()

    def ResetField(self):
        self.__ResetField()

    def Start(self):
        if not self.__is_64bit:
            self.__startMinesweeper()
        else:
            self.__MineProc = subprocess.Popen("minesweeperRuntime.exe")

    def Stop(self):
        if not self.__is_64bit:
            self.__dealocateMinesweeper()
        else:
            print("Right now the process can not be killed from 64 bit runtime")

    def IsRunning(self):
        if not self.__is_64bit:
            return self.__isMinesweeperRunning()
        else:
            print(
                "Right now the process can not be checked if it is running from 64 bit runtime"
            )

    def GetField(self):
        height = int.from_bytes(self.__GetFieldHeight(), "little")
        width = int.from_bytes(self.__GetFieldWidth(), "little")
        data = (ctypes.c_char * 720)()
        field = [[0 for i in range(width)] for j in range(height)]
        self.__GetField(data)
        for y in range(height):
            for x in range(width):
                field[y][x] = data[y * 30 + x].decode("utf-8")
        return field

    def PrintField(self):
        field = self.GetField()
        for col in field:
            print(" ".join(col))

    def ClickOnTile(self, x, y):
        self.__ClickOnTile(ctypes.c_char(x), ctypes.c_char(y))

    def GetGameStatus(self):
        status = self.__GetGameStatus()
        return GameStatus(status)


class GameStatus(Enum):
    GAME_RUNNING = 0
    GAME_WON = 1
    GAME_LOST = 2