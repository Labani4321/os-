#!/bin/bash

while getopts ":e:" opt; do
    case ${opt} in
        e)
            new_extension="${OPTARG}"
            ;;
        \?)
            echo "Usage: $0 -e new_extension"
            exit 1
            ;;
    esac
done

[ -z "${new_extension}" ] && { echo "Usage: $0 -e new_extension"; exit 1; }

for file in *; do
    [[ "$file" == *.sh ]] && continue
    mv -- "$file" "${file%.*}.${new_extension}"
done

echo "Alll files renamed"

