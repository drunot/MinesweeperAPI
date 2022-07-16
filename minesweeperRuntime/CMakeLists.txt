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

add_executable(minesweeperRuntime WIN32 ${sources} ${headers})

include_directories(minesweeperRuntime PRIVATE ${CMAKE_CURRENT_LIST_DIR} ../minesweeperAPI)

target_compile_definitions(minesweeperRuntime PUBLIC PLT_WIN)

target_link_libraries(minesweeperRuntime minesweeperAPIx86)

install(TARGETS minesweeperRuntime DESTINATION ${INSTALL_DIR})