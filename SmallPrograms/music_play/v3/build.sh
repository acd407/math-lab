#! env bash

source ./def.sh

for inc in $INC; do
    INC_char="$INC_char -I$inc"
done

for lib in $LIB; do
    LIB_char="$LIB_char -L$lib"
done

for file in $SRC/*.cpp; do
    obj=${file%.*}
    if [ -f "${obj}.o" ]; then
        if (( "$(stat -c %Y $file)" < "$(stat -c %Y ${obj}.o)" )); then
            echo -e $file is up to date, skipping
            continue
        fi
    fi
    echo $CC -o ${obj}.o $INC_char -c $file
    $CC -o ${obj}.o $INC_char -c $file
done

# sh 就是一坨翔
for _lib in $libs; do
    libs_char="$libs_char -l$_lib"
done

OBJ="$(echo $SRC/*.o)"
echo $CC -o $EXE $OBJ $LIB_char $libs_char -s
$CC -o $EXE $OBJ $LIB_char $libs_char -s
