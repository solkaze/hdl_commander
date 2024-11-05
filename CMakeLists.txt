cmake_minimum_required(VERSION 3.10)

project(hdl_command_decoder)

set(PROJECT_VERSION 1.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE
    src/main.cpp
)

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