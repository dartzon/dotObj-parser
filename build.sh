#!/bin/bash

# Replace spaces with \n = One command per line.
_cmd_list=$(echo $* | tr ' ' '\n')
_build_mode=""
_build_arch=0
_this_sh_path="$( cd "$(dirname "$0")" ; pwd -P )"
_cmakelist_dir=$_this_sh_path
_do_clean=0
_test_mode="OFF"

for _cmd in $_cmd_list
do

    case $_cmd in

	    -c|-clean)
	        let _do_clean=1
	        ;;

	    -tc|-total-clean)
	        let _do_clean=2
	        ;;

	    -d|-debug)
	        if [[ $_build_mode == "release" ]]; then
		        echo "$0: Can't build for debug and release modes at the same time."
		        exit
	        fi

	        _build_mode="debug"
	        ;;

	    -r|-release)
	        if [[ $_build_mode == "release" ]]; then
		        echo "$0: Can't build for debug and release modes at the same time."
		        exit
	        fi

	        _build_mode="release"
	        ;;

	    -a32)
	        if [ $_build_arch == 64 ]; then
		        echo "$0: Can't build for 32-bit and 64-bit architectures " \
		             "at the same time."
		        exit
	        fi

	        let _build_arch=32
	        ;;

	    -a64)
	        if [ $_build_arch == 32 ]; then
		        echo "$0: Can't build for 32-bit and 64-bit architectures " \
		             "at the same time."
		        exit
	        fi

	        let _build_arch=64
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

	        exit
	        ;;

	    *)
	        echo "$0: invalid option -- '$_cmd'"
	        echo -e "Try '$0 -h' for more information."

	        exit
	        ;;

    esac

done

if [ -z $_build_mode ]; then

    _build_mode="release"

fi

if [ $_build_arch == 0 ]; then

    _build_arch=64

fi

# Assemble the correct build dir path.
_build_dir=build/$_build_mode/$_build_arch

# Clean build directory if requested.
case $_do_clean in

    1)
	    if [ -f $_build_dir/CMakeCache.txt ]; then
	        rm $_build_dir/CMakeCache.txt
	        echo "$_build_mode $_build_arch CMakeCache.txt removed"
	    fi
	    ;;

    2)
	    if [ -e $_build_dir ]; then
	        rm -r $_build_dir
	        echo "$_build_mode $_build_arch build directory removed"
	    fi

esac

# Create build directory if it doesn't exist.
if [ ! -e $_build_dir ]; then
    mkdir -p $_build_dir
fi
cd $_build_dir

# Run cmake if necessary.
if [ ! -e $_build_dir ] || [ ! -f $_build_dir/CMakeCache.txt ]; then

    # Use clang compiler.
    export CC=clang
    export CXX=clang++

    cmake -G "Unix Makefiles" "$_cmakelist_dir" -DCMAKE_BUILD_TYPE="$_build_mode" \
          -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DBUILD_TESTS="$_test_mode"
fi

# Generate .dir-locals.el
_project_name=$(cat $_this_sh_path/CMakeLists.txt | grep "project(" | cut -d'(' -f2 | cut -d')' -f1)
source $_this_sh_path/.generate-dir-locals.sh $_build_mode $_build_arch \
       $_this_sh_path $_project_name $*

# Place .clang-format in the project's root.
ln -sf ~/.dotfiles/not-installable/clang-tools-config/clang-format $_this_sh_path/.clang-format

# Place .clang-tidy in the project's root.
ln -sf ~/.dotfiles/not-installable/clang-tools-config/clang-tidy $_this_sh_path/.clang-tidy

# Run make.
_jobs_count=$(grep -c ^processor /proc/cpuinfo)
make -j$_jobs_count

# Copy the compilation database to the project root for cquery.
ln -sf $_build_dir/compile_commands.json $_this_sh_path/compile_commands.json
