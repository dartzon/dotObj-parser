#!/bin/bash

if [ "$1" = "-test" ]; then

    build/bin/objparser_tests

else

    build/bin/objparser $1

fi
