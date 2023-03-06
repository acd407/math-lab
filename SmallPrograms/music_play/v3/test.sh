#! env bash

source ./def.sh

if [ -f "$EXE" ] || [ -f "${EXE}.exe" ]; then
    for res in ./demo/*; do
        echo "./$EXE $res"
        ./$EXE $res
        printf '\n\n'
    done
else
    echo executable file not found
fi
