#ifndef A5688130_31D3_43B4_A174_1BD854E1CB5F
#define A5688130_31D3_43B4_A174_1BD854E1CB5F

/**
 *  @file   core.hpp
 *  @brief  The core header that defines bacic things.
 *  @author Anton Vigen Smolarz
 *  @date   2021-02-26
 **/

#ifndef CBBE8A2A_5B03_41A8_882D_5C2EB8ED244B
#define CBBE8A2A_5B03_41A8_882D_5C2EB8ED244B

#define API_STATIC_CLASS(classType) \
    classType() = delete; \
    ~classType() = delete; \
    classType& operator=(classType& right) = delete; \
    classType& operator=(classType&& right) = delete

#ifdef PLT_WIN
    #ifdef WINMINE_API_DLL_EXPORTS
        #define WINMINE_API __declspec(dllexport) __cdecl
    #else
        #define WINMINE_API __declspec(dllimport) __cdecl
    #endif
#else
    #error Other platforms not planed for now.
#endif //PLT_WIN

#endif // CBBE8A2A_5B03_41A8_882D_5C2EB8ED244B


#endif // A5688130_31D3_43B4_A174_1BD854E1CB5F
