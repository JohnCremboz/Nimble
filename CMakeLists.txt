cmake_minimum_required(VERSION 3.10)
project(NimbleNanoEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(nimble
    src/main.cpp
    src/editor.cpp
    src/editor.h
    src/filebrowser.cpp
    src/filebrowser.h
)

target_link_libraries(nimble ${CURSES_LIBRARIES})
