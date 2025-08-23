cmake_minimum_required(VERSION 3.10)
project(NimbleNanoEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories for our own headers and third-party libraries
include_directories(src)
include_directories(third_party) # For nlohmann/json.hpp
include_directories(${CMAKE_SOURCE_DIR}/src/nlohmann)

# Find required system libraries
find_package(Curses REQUIRED)
# For cross-platform dynamic library loading on Linux
if(UNIX AND NOT APPLE)
    find_library(DL_LIBRARY dl)
endif()

# --- Main Executable ---
add_executable(nimble
    src/main.cpp
    src/editor.cpp
    src/filebrowser.cpp
    src/pluginmanager.cpp
)

# Link libraries to the executable
target_link_libraries(nimble PRIVATE ${CURSES_LIBRARIES})
if(DL_LIBRARY)
    target_link_libraries(nimble PRIVATE ${DL_LIBRARY})
endif()
# For std::filesystem on some older compilers/linkers
target_link_libraries(nimble PRIVATE stdc++fs)

# --- Plugins ---
# Plugins will be built into the 'plugins' subdirectory of the build folder.
# The executable looks for them in a relative 'plugins' directory at runtime.
#add_library(markdown_plugin SHARED plugins/markdown_plugin.cpp)
#set_target_properties(markdown_plugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
#add_library(csv_plugin SHARED plugins/csv_plugin.cpp)
#set_target_properties(csv_plugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
#add_library(example_plugin SHARED plugins/example_plugin.cpp)
#set_target_properties(example_plugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
