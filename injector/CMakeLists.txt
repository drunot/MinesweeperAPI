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

add_library(injector SHARED ${sources} ${headers} ${__minesweeperRPC_server_path} ${__minesweeperRPC_header_path})

include_directories(injector PUBLIC ${__minesweeperRPC_include_path} ${CMAKE_CURRENT_LIST_DIR})

target_compile_definitions(injector PUBLIC INJECTOR_DLL_EXPORTS PLT_WIN DO_LOGGING)

target_link_libraries(injector "rpcrt4.lib" "Windowscodecs.lib")

install(TARGETS injector DESTINATION ${INSTALL_DIR})
