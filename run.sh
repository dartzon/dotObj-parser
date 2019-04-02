#!/bin/bash

# Replace spaces with \n = One command per line.
_cmd_list=$(echo $* | tr ' ' '\n')
_this_sh_path="$( cd "$(dirname "$0")" ; pwd -P )"
_run_mode=""
_run_arch=0
_run_gdb=0
_run_tests=0

for _cmd in $_cmd_list
do

    case $_cmd in

	    -d|-debug)
	        if [[ $_run_mode == "release" ]]; then
		        echo "$0: Can't run debug and release builds at the same time."
		        exit
	        fi

	        _run_mode="debug"
	        ;;

	    -gdb)
	        if [[ $_run_mode == "release" ]]; then
		        echo "$0: Can't debug and run release build at the same time."
		        exit
	        fi

	        let _run_gdb=1
	        _run_mode="debug"
	        ;;

	    -r|-release)
	        if [[ $_run_mode == "debug" ]]; then
		        echo "$0: Can't run debug and release builds at the same time."
		        exit
	        fi

	        _run_mode="release"
	        ;;

	    -a32)
	        if [ $_run_arch == 64 ]; then
		        echo "$0: Can't run 32-bit and 64-bit builds at the same time."
		        exit
	        fi

	        let _run_arch=32
	        ;;

	    -a64)
	        if [ $_run_arch == 32 ]; then
		        echo "$0: Can't run 32-bit and 64-bit builds at the same time."
		        exit
	        fi

	        let _run_arch=64
	        ;;

        -t|-test)
	        let _run_tests=1
	        ;;


	    -h|-help)
	        echo "Usage: $0 [OPTION]..."
	        echo "Run the program or the unit-tests."
	        echo -e "Run in release mode for 64-bit architecture if no option is specified.\n"

	        echo -e "Example: $0 -d -a64\n"

	        echo "Optional arguments:"
	        echo -e "  -d, -debug\tRun in DEBUG mode."
	        echo -e "  -gdb\tRun through the GDB debugger."
	        echo -e "  -r, -release\tRun in RELEASE mode."
	        echo -e "  -a32\tRun for 32-bit architecture"
	        echo -e "  -a64\tRun for 64-bit architecture"
	        echo -e "  -h, -help\tDisplay this help and exit."
	        echo -e "\nReport bugs to: dartzon@gmail.com"

	        exit
	        ;;

	    *)
	        echo "$0: invalid option -- '$_cmd'"
	        echo "Try '$0 -h' for more information."

	        exit
	        ;;

    esac

done

if [ -z $_run_mode ]; then

    _run_mode="release"

fi

if [ $_run_arch == 0 ]; then

    _run_arch=64

fi

# Assemble the correct build dir path.
_build_dir=$_this_sh_path/build/$_run_mode/$_run_arch
if [ ! -e $_build_dir ]; then

    echo "$0: no $_run_mode $_run_arch build found."
    echo "Please build the project first with: 'build.sh'"
    exit

fi

if [ $_run_tests == 1 ]; then

    _cmake_path=$_this_sh_path/tests

else

    _cmake_path=$_this_sh_path

fi

_bin_name=$(cat $_cmake_path/CMakeLists.txt | grep "add_executable(" | \
                cut -d'(' -f2 | cut -d'$' -f1 | tr -d [[:blank:]])

_exe_path=$_build_dir/bin/$_bin_name

if [ $_run_gdb == 1 ]; then

    gdb -tui -x $_this_sh_path/.gdb-args --args $_exe_path

else

    $_exe_path

fi
