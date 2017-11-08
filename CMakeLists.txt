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
## Enable warnings
###############################################################################
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # using Clang
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-missing-braces -Wno-long-long \
                       -pedantic")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-long-long -pedantic")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /WX")
endif()

###############################################################################
## Target definitions
###############################################################################
add_executable(objparser ${dotObjParser_SRC})

target_compile_options(objparser PUBLIC -std=c++14 $<$<CONFIG:DEBUG>:-DOBJ_DEBUG>)

target_include_directories(objparser PUBLIC src/)
