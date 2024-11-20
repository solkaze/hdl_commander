cmake_minimum_required(VERSION 3.10)

project(hdl_command_decoder)

set(PROJECT_VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE
    src/main.cpp
)

set(DATA_DIR_PATH "${CMAKE_SOURCE_DIR}/data")


# プリプロセッサマクロに設定
add_compile_definitions(DATA_DIR_PATH="${DATA_DIR_PATH}")

add_executable(main ${SOURCE})

target_compile_options(main PRIVATE -g3 -O2 -Wall -Wextra)

message(STATUS "--------------------------------------------")
message(STATUS "Info :")
message(STATUS "  Project Name = ${PROJECT_NAME}")
message(STATUS "  Version =     ${PROJECT_VERSION}")
message(STATUS "  CMAKE_VERSION =        ${CMAKE_VERSION}")
message(STATUS "  CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  CMAKE_BUILD_TYPE =     ${CMAKE_BUILD_TYPE}")
message(STATUS "  CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")