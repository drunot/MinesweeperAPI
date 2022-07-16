#include "pch.h"
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include "patch.hpp"
#include <thread>
#include <chrono>
#include <map>
#include <condition_variable>
#include <filesystem>

#include "Utilities/WICImageLoader.hpp"
#include "minesweeper.hpp"
#include "Utilities/ConsoleLogging.hpp"

namespace minesweeper {
    static WICImageLoader buttonIdle = WICImageLoader();
    static WICImageLoader buttonChecking = WICImageLoader();
    static WICImageLoader buttonDown = WICImageLoader();
    static WICImageLoader buttonLost = WICImageLoader();
    static WICImageLoader buttonWon = WICImageLoader();

    static const std::array<const std::string, 7> supportedImageTypes = {
        ".bmp",
        ".gif",
        ".ico",
        ".jpg",
        ".jpeg",
        ".png",
        ".tiff"
    };

    static const std::array<const std::string, 5> imageNames = {
        "ButtonChecking",
        "ButtonDown",
        "ButtonIdle",
        "ButtonLost",
        "ButtonWon"
    };
    static const std::map<const std::string, WICImageLoader*> imgNameToLoader {
        {"ButtonChecking", &buttonChecking},
        {"ButtonDown", &buttonDown},
        {"ButtonIdle", &buttonIdle},
        {"ButtonLost", &buttonLost},
        {"ButtonWon", &buttonWon}
    };
    static const std::map<const DWORD, WICImageLoader*> numToLoader {
        {1, &buttonChecking},
        {4, &buttonDown},
        {0, &buttonIdle},
        {2, &buttonLost},
        {3, &buttonWon}
    };


    bool _lastGameWon = false;
    DWORD* isRunning = reinterpret_cast<DWORD*>(0x010060BC);
    DWORD* notFirstRound = reinterpret_cast<DWORD*>(0x1006660);
    DWORD* numOfBooms = reinterpret_cast<DWORD*>(0x01006184);
    DWORD* fieldHight = reinterpret_cast<DWORD*>(0x01006188);
    DWORD* fieldWidth = reinterpret_cast<DWORD*>(0x0100618C);

    BYTE* playingField = reinterpret_cast<BYTE*>(0x01006670);

    static BOOL cheat = false;

    static std::array<DWORD, 5> resetFieldCalls = { 0x010014BA,
                                                    0x01001673,
                                                    0x010017F2,
                                                    0x01001956,
                                                    0x010019E5
                                                  };
    static std::array<DWORD, 5> endGameCalls = { 0x0100401A,
                                                 0x01004044,
                                                 0x010041DA,
                                                 0x010041F7,
                                                 0x0100497D
                                               };
    static std::array<DWORD, 2> smiliyCalls = {
        0x01003690,
        0x01003922
    };

    BYTE* __stdcall getPlayingField() {
        return playingField;
    }

    DWORD __stdcall getFieldHeight() {
        return *fieldHight;
    }

    DWORD __stdcall getFieldWidth() {
        return *fieldWidth;
    }

    // void __stdcall PrintField() {
    //     if(cheat) {
    //         system("cls");

    //         for(DWORD y = 1; y <= *fieldHight; y++) {
    //             for(DWORD x = 1; x <= *fieldWidth; x++) {
    //                 auto toPrint = playingField[(y << 5) + x];

    //                 if((toPrint & 0x0F) == 0x0E) {
    //                     std::printf("F ");

    //                 } else if((toPrint & 0xF0) == 0x80) {
    //                     std::printf("B ");

    //                 } else if((toPrint & 0x0F) == 0x0D) {
    //                     std::printf("? ");

    //                 } else if((toPrint & 0xF0) == 0x40) {
    //                     if(!(toPrint & 0x0F)) {
    //                         std::printf("  ");

    //                     } else {
    //                         std::printf("%d ", (toPrint & 0x0F));
    //                     }

    //                 } else {
    //                     std::printf("# ");
    //                 }
    //             }

    //             std::printf("\n");
    //         }
    //     }
    // }

    void __stdcall CheckTile(DWORD x_cord, DWORD y_cord) {
        typedef void(__stdcall * pFunctionAddress)(DWORD, DWORD);
        pFunctionAddress pMySecretFunction = (pFunctionAddress)(0x01003F30);
        pMySecretFunction(x_cord, y_cord);
    }

    void __stdcall ResetField() {
        typedef void(__stdcall * pFunctionAddress)();
        pFunctionAddress pMySecretFunction = (pFunctionAddress)(0x01004210);
        pMySecretFunction();
        LOG_INFO("Field reset");
    }

    char __stdcall EndGame(bool won) {
        _lastGameWon = won;

        if(won) {
            LOG_INFO("You won!");

        } else {
            LOG_INFO("You lost!");
        }

        typedef char(__stdcall * pFunctionAddress)(bool);
        pFunctionAddress pMySecretFunction = (pFunctionAddress)(0x01003C30);
        return pMySecretFunction(won);
    }
    BOOL __stdcall InjectFunctions() {
        BOOL toReturn = true;

        for(auto ptr : minesweeper::resetFieldCalls) {
            if(!ApplyPatch(0xE8, ptr, ResetField)) {
                LOG_ERROR("Patch at address %p faild", reinterpret_cast<void*>(ptr));
                toReturn = false;
            }
        }

        for(auto ptr : minesweeper::endGameCalls) {
            if(!ApplyPatch(0xE8, ptr, EndGame)) {
                LOG_ERROR("Patch at address %p faild", reinterpret_cast<void*>(ptr));
                toReturn = false;
            }
        }

        for(auto ptr : minesweeper::smiliyCalls) {
            if(!ApplyPatch(0xE8, ptr, SetSmily)) {
                LOG_ERROR("Patch at address %p faild", reinterpret_cast<void*>(ptr));
                toReturn = false;
            }
        }

        if(!ApplyPatch(0xE8, 0x01004A96, CheckTile)) {
            LOG_ERROR("Patch at address %p faild", reinterpret_cast<void*>(0x01004A96));
            toReturn = false;
        }

        if(!ApplyPatch(0xE8, 0x01001FB0, ClickOnField)) {
            LOG_ERROR("Patch at address %p faild", reinterpret_cast<void*>(0x01001FB0));
            toReturn = false;
        }

        return toReturn;
    }


    void __stdcall ClickOnField() {
        ClickOnTile(*reinterpret_cast<DWORD*>(0x10060C0), *reinterpret_cast<DWORD*>(0x10060C4));
    }

    void __stdcall ClickOnTile(DWORD& x_cord, DWORD& y_cord) {
        LOG_INFO("Tile clicked, x: %lu, u: %lu", x_cord, y_cord);
        typedef void(__stdcall * pFunctionAddress)(DWORD);

        if(x_cord > 0 && x_cord <= *fieldWidth && y_cord > 0 && y_cord <= *fieldHight) {
            if(!*reinterpret_cast<DWORD*>(0x1006664) && !*notFirstRound) {
                pFunctionAddress pUnknownFunction1 = (pFunctionAddress)(0x01004F10);
                pUnknownFunction1(1);
                (*notFirstRound)++;
                typedef void(__stdcall * pFunctionAddress1)();
                pFunctionAddress1 pUnknownFunction2 = (pFunctionAddress1)(0x010035E0);
                pUnknownFunction2();
                *reinterpret_cast<DWORD*>(0x10060BC) = 1;
            }

            // No idea what this does!
            if(!*reinterpret_cast<DWORD*>(0x10060BC)) {
                y_cord = -2;
                x_cord = y_cord;

                if(y_cord != 0) {
                    typedef void(__stdcall * pFunctionAddress1)(DWORD, DWORD);
                    pFunctionAddress1 pUnknownFunction3 = (pFunctionAddress1)(0x010040D0);
                    pUnknownFunction3(x_cord, y_cord);
                }

            } else {
                // If tile not flagged nor already cleared.
                if((playingField[(y_cord << 5) + x_cord] & 0x1F) != 0x0E
                        && (playingField[(y_cord << 5) + x_cord] & 0xE0) != 0x40) {
                    CheckTile(x_cord, y_cord);

                } else if((playingField[(y_cord << 5) + x_cord] & 0xE0) == 0x40) {
                    DoubleClickOnField(x_cord, y_cord);
                }
            }
        }

        pFunctionAddress pSetSmileyButtonToSmile = (pFunctionAddress)(0x01003670);
        pSetSmileyButtonToSmile(*reinterpret_cast<DWORD*>(0x10060B8));
    }


    static DWORD x_double = 0;
    static DWORD y_double = 0;
    static std::thread* resetDoubleClick = nullptr;
    static std::condition_variable cv;
    static std::mutex cv_m;
    static bool stopWait = false;
    void __stdcall ClearNeighbours(DWORD x_cord, DWORD y_cord);
    void __stdcall DoubleClickOnField(DWORD& x_cord, DWORD& y_cord) {
        if(x_cord == x_double && y_cord == y_double) {
            ClearNeighbours(x_cord, y_cord);

        } else {
            x_double = x_cord;
            y_double = y_cord;

            if(resetDoubleClick) {
                stopWait = true;
                cv.notify_all();
                resetDoubleClick->join();
                stopWait = false;
            }

            resetDoubleClick = new std::thread([] {
                std::unique_lock<std::mutex> lk(cv_m);
                cv.wait_for(lk, std::chrono::milliseconds(200), [] {return stopWait; });

                if(!stopWait) {
                    x_double = 0;
                    y_double = 0;
                }
            });
        }
    }

    void __stdcall ClearNeighbours(DWORD x_cord, DWORD y_cord) {
        auto x_start = x_cord - 1 < 1 ? 1 : x_cord - 1;
        auto x_end = x_cord + 1 > *fieldWidth ? *fieldWidth : x_cord + 1;
        auto y_start = y_cord - 1 < 1 ? 1 : y_cord - 1;
        auto y_end = y_cord + 1 > *fieldHight ? *fieldHight : y_cord + 1;
        auto numberOfFlags = 0;

        for(auto x = x_start; x <= x_end; x++) {
            for(auto y = y_start; y <= y_end; y++) {
                if((playingField[(y << 5) + x] & 0x1F) == 0x0E) {
                    numberOfFlags += 1;
                }
            }
        }

        if(numberOfFlags < (playingField[(y_cord << 5) + x_cord] & 0x1F)) {
            return;
        }

        for(auto x = x_start; x <= x_end; x++) {
            for(auto y = y_start; y <= y_end; y++) {
                if((playingField[(y << 5) + x] & 0x1F) != 0x0E
                        && (playingField[(y << 5) + x] & 0xE0) != 0x40) {
                    CheckTile(x, y);
                }
            }
        }
    }
    DWORD __stdcall GetHight() {
        return *fieldHight;
    }

    DWORD __stdcall GetWidth() {
        return *fieldWidth;
    }

    BOOL __stdcall gameStarted() {
        return *isRunning || !*notFirstRound;
    }

    BOOL __stdcall lastGameWon() {
        return static_cast<BOOL>(_lastGameWon);
    }


    /*
     * 0 - :-)
     * 1 - :-O
     * 2 - x-(
     * 3 - B-)
     * 4 - pressed-smiley
     */
    void __stdcall SetSmily(HDC hdc, DWORD SmilyType) {
        minesweeper::loadButtons();
        constexpr DWORD w = 24;
        constexpr DWORD h = 24;
        constexpr int yDest = 16;
        auto windowWidth = *reinterpret_cast<DWORD*>(0x10060F4);
        int xDest = ((windowWidth - w) / 2);
        auto imgIt = numToLoader.find(SmilyType);

        if(imgIt != numToLoader.end()) {
            if(imgIt->second->IsLoaded()) {
                imgIt->second->Render(hdc, xDest, yDest, w, h);
                return;
            }
        }

        constexpr int xSrc = 0;
        constexpr int ySrc = 0;
        constexpr UINT StartScan = 0;
        constexpr UINT cLines = 24;
        const BITMAPINFO* lpbmi = reinterpret_cast<BITMAPINFO*>(0x100922C);
        constexpr UINT ColorUse = DIB_RGB_COLORS;
        const DWORD offset = *reinterpret_cast<DWORD*>((SmilyType * 4) + 0x1006A30);
        const DWORD base =  *reinterpret_cast<DWORD*>(0x10069E0);
        const VOID* lpvBits = reinterpret_cast<VOID*>(base + offset);
        SetDIBitsToDevice(
            hdc,
            xDest,
            yDest,
            w,
            h,
            xSrc,
            ySrc,
            StartScan,
            cLines,
            lpvBits,
            lpbmi,
            ColorUse
        );
    }

    bool __stdcall fileExists(std::string path) {
        std::filesystem::path p {path};
        return std::filesystem::exists(p);
    }

    void __stdcall loadButtons() {
        for(auto name : imageNames) {
            for(auto ext : supportedImageTypes) {
                if(!fileExists(name + ext)) {
                    continue;
                }

                auto temp = imgNameToLoader.find(name);

                if(temp != imgNameToLoader.end()) {
                    temp->second->Open(name + ext);
                    break;
                }
            }
        }
    }
}
