

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(MinesweeperAPI VERSION 0.1)
message("CMAKE_VS_PLATFORM_NAME: ${CMAKE_VS_PLATFORM_NAME}")
if(NOT DEFINED CMAKE_VS_PLATFORM_NAME)
set(CMAKE_VS_PLATFORM_NAME x64)
message(${CMAKE_VS_PLATFORM_NAME})
endif()

set(INSTALL_DIR ${PROJECT_SOURCE_DIR}/bin)

message("INSTALL_DIR: " ${INSTALL_DIR})

add_subdirectory(rpc)
add_subdirectory(minesweeperAPI)

if(${CMAKE_VS_PLATFORM_NAME} MATCHES "Win32")
add_subdirectory(injector)
add_subdirectory(minesweeperRuntime)
endif()

install(FILES external/winmine.exe DESTINATION ${INSTALL_DIR})