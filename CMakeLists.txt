cmake_minimum_required(VERSION 3.0)
project(dotObjParser)

# Enable c, and c++ language
enable_language(C CXX)

include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckIncludeFiles)
include(ExternalProject)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

###############################################################################
## File globbing
###############################################################################
file(GLOB_RECURSE dotObjParser_SRC src/*.cpp src/*.h)

###############################################################################
## Target definitions
###############################################################################
add_executable(objparser ${dotObjParser_SRC})

target_compile_options(objparser PUBLIC -pthread -std=c++14)

target_include_directories(objparser PUBLIC src/)
