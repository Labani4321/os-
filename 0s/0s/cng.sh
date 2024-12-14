#!/bin/bash

if [ "$#" -ne 2 ] || [ "$1" != "-e" ]; then
    echo "Usage: $0 -e new_extension"
    exit 1
fi
new_extension="$2"

for file in *; do
    if [ ! -d "$file" ] && [[ "$file" != *.sh ]]; then
        mv -- "$file" "${file%.*}.${new_extension}"
    fi
done

echo "All files except .sh files have been renamed to have the .${new_extension} extension."

