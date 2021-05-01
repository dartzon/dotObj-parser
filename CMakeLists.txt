cmake_minimum_required(VERSION 3.0)
project(dotObjParser)

# Enable c, and c++ language
enable_language(C CXX)

###############################################################################
## Use C++ 17.
###############################################################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

###############################################################################
## Output directories.
###############################################################################
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

###############################################################################
## Various flags.
###############################################################################
include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckIncludeFiles)
include(ExternalProject)

###############################################################################
## Enable all warnings and warnings as errors.
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
## libobjparser definitions.
###############################################################################
file(GLOB_RECURSE LIBOBJPARSER_SRC_LST ${PROJECT_SOURCE_DIR}/src/libobjparser/src/*.cpp)
add_library(objparser_static STATIC ${LIBOBJPARSER_SRC_LST})
target_compile_options(objparser_static PUBLIC $<$<CONFIG:DEBUG>:-DOBJ_DEBUG>)
target_include_directories(objparser_static PUBLIC ${PROJECT_SOURCE_DIR}/src/libobjparser/include)
target_link_libraries(objparser_static stdc++fs)

add_library(objparser_shared SHARED ${LIBOBJPARSER_SRC_LST})
target_compile_options(objparser_shared PUBLIC $<$<CONFIG:DEBUG>:-DOBJ_DEBUG>)
target_include_directories(objparser_shared PUBLIC ${PROJECT_SOURCE_DIR}/src/libobjparser/include)
target_link_libraries(objparser_shared stdc++fs)

###############################################################################
## Target definitions.
###############################################################################
file(GLOB_RECURSE EXEC_SRC_LST ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(objparser ${EXEC_SRC_LST})
target_compile_options(objparser PUBLIC $<$<CONFIG:DEBUG>:-DOBJ_DEBUG>)
target_include_directories(objparser PUBLIC ${PROJECT_SOURCE_DIR}/src/ ${PROJECT_SOURCE_DIR}/src/libobjparser/include)
target_link_libraries(objparser stdc++fs objparser_shared)

###############################################################################
## Unit test target.
###############################################################################
option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
  enable_testing()
  include_directories(${PROJECT_SOURCE_DIR}/src)
  set(PROJECT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
  add_subdirectory(tests)
endif()
