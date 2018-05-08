#!/bin/bash

_cmd_list=$(echo $* | tr ' ' '\n')
_build_mode="debug"
_test_mode="OFF"
_should_build=1

for _cmd in $_cmd_list
do

    case $_cmd in

	-c|-clean)
	    if [ -e build/CMakeCache.txt ]; then
		rm build/CMakeCache.txt
		echo "CMakeCache.txt removed"
	    fi
	    ;;

	-tc|-total-clean)
	    if [ -e build ]; then
		rm -r build
		echo "Build directory removed"
	    fi
	    ;;

	-d|-debug)
	    _build_mode="debug"
	    ;;

	-r|-release)
	    _build_mode="release"
	    ;;

	-t|-test)
	    _test_mode="ON"
	    ;;

	-h|-help)
	    echo "Usage: $0 [OPTION]..."
	    echo "Run CMake and make in the build directory."
	    echo -e "Build in debug mode and no tests if none of -d -r -t is specified.\n"

	    echo -e "Example: $0 -tc -r -t\n"

	    echo "Optional arguments:"
	    echo -e "  -d, -debug\tBuild in DEBUG mode."
	    echo -e "  -r, -release\tBuild in RELEASE mode."
	    echo -e "  -t, -test\tBuild tests."
	    echo -e "  -c, -clean\tRemove the CMakeCache.txt file."
	    echo -e "  -tc, -total-clean\tRemove the entire build directory."
	    echo -e "  -h, -help\tDisplay this help and exit."
	    echo -e "\nReport bugs to: dartzon@gmail.com"

	    let _should_build=0
	    break
	    ;;

	*)
	    echo "$0: invalid option -- '$_cmd'"
	    echo -e "Try '$0 -h' for more information."
	    let _should_build=0
	    break
	    ;;

    esac

done

if [ $_should_build == 1 ]; then

    # Use clang instead of gcc.
    export CC=clang
    export CXX=clang++

    CMAKELIST_DIR=$(pwd)

    BUILD_DIR=build/
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR

    # Cmake command
    cmake -G "Unix Makefiles" "$CMAKELIST_DIR" -DCMAKE_BUILD_TYPE="$_build_mode" -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DBUILD_TESTS="$_test_mode"

    # Compile command
    _jobs_count=$(grep -c ^processor /proc/cpuinfo)
    make -j$_jobs_count

fi
