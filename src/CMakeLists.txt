
# CMake version
cmake_minimum_required (VERSION 3.0.1)

# CMake modules directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# Set up compiler definitions
add_definitions(-std=c++0x
                -pthread
                -Wall
                -Wextra)

# Frame
add_subdirectory(frame/src/frame)


# Sandbox program
add_subdirectory(metabox)
