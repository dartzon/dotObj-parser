#!/bin/bash

# Use clang instead for gcc
export CC=clang
export CXX=clang++

CMAKELIST_DIR=$(pwd)

BUILD_DIR=build/
mkdir -p $BUILD_DIR
cd $BUILD_DIR

if [ -f CMakeCache.txt ]; then
    rm CMakeCache.txt
fi

if [ "$1" = "-test" ]; then

    cmake -G "Unix Makefiles" "$CMAKELIST_DIR" -DCMAKE_BUILD_TYPE=release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DBUILD_TESTS=1

else

    cmake -G "Unix Makefiles" "$CMAKELIST_DIR" -DCMAKE_BUILD_TYPE=release -DCMAKE_EXPORT_COMPILE_COMMANDS=1

fi

# Compile command
make -j8
