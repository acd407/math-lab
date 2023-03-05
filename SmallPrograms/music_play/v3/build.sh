CC=g++

EXE=mu

INCS=./include
SRC="$(echo ./src/*.cpp)"

LIBS="winmm fmt"

for inc in $INCS
do
    INC="$INC -I$inc"
done

for file in $SRC
do
    obj=${file%.*}
    $CC -o ${obj}.o $INC -c $file
done

for lib in $LIBS
do
    LINK="$LINK -l$lib"
done

OBJ="$(echo ./src/*.o)"
$CC -o $EXE $OBJ $LINK -s
