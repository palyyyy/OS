#!/bin/bash

# Iterate through command line arguments
for file in "$@"
do
    # Check if the file has a .c extension
    if [[ $file == *.c ]]; then
        # Run the file using gcc and execute the output
        gcc "$file" -o "${file%.c}"
        if [ $? -eq 0 ]; then
            "./${file%.c}"
        else
            echo "Failed to compile $file"
        fi
    else
        echo "Skipping $file: Not a C file"
    fi
done
