#!/bin/bash

if [ -z $1 ]; then
            echo "Error: You must provide a file path."
            echo "Usage: check_deps <path_to_executable>"
        elif [ ! -f $1 ]; then
            echo "Error: File $1 not found."
        else
            echo "--- Mapping Process Dependencies $1 ---"
            ldd $1
        fi
