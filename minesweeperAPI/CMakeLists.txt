file(GLOB_RECURSE sources 
                  *.cpp
                  *.c
                  )
file(GLOB_RECURSE headers 
                  *.hpp
                  *.h
                  )

if(NOT DEFINED INSTALL_DIR)
    set(INSTALL_DIR ${PROJECT_SOURCE_DIR}/bin)
endif()

if(${CMAKE_VS_PLATFORM_NAME} MATCHES "Win32")
    set(bin_prefix x86)
    set(define_runtime WIN_X86)
elseif(${CMAKE_VS_PLATFORM_NAME} MATCHES "x64")
    set(bin_prefix x64)
    set(define_runtime WIN_X64)
endif()

include(${PROJECT_SOURCE_DIR}/cmake/MircrosoftRPC.cmake)

add_library(minesweeperAPI${bin_prefix} SHARED ${sources} ${headers} ${__minesweeperRPC_header_path} ${__minesweeperRPC_client_path})

include_directories(minesweeperAPI${bin_prefix} PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${__minesweeperRPC_include_path})

target_compile_definitions(minesweeperAPI${bin_prefix} PUBLIC WINMINE_API_DLL_EXPORTS PLT_WIN ${define_runtime})

target_link_libraries(minesweeperAPI${bin_prefix} "rpcrt4.lib")

install(TARGETS minesweeperAPI${bin_prefix} DESTINATION ${INSTALL_DIR})