#! env bash

source ./def.sh

if [ -n "$(echo $SRC/*.o)" ]; then
    rm $SRC/*.o
fi
if [ -f "$EXE" ]; then
    rm $EXE
fi
if [ -f "$EXE.exe" ]; then
    rm $EXE.exe
fi
