# ================ Configure =============================
# Openblas
export OPENBLAS_NUM_THREADS=16

# matlab path
export MW_MINGW64_LOC=$(cygpath -w /mingw64/)

# add pkgconfig path
export PKG_CONFIG_PATH=/mingwex/mingw64/lib/pkgconfig/

export EDITOR=notepad
export MINGW_PREFIX=/mingw64

# GO Root Path
export GOROOT=~/.local/share/go

# man-zh
export MANPATH=/usr/man:/usr/share/man:~/.manpages/man/

# GCC & SYSTEM LD dir
# only add mingwex
export C_INCLUDE_PATH=/mingwex/mingw64/include/:/mingw64/include/eigen3
export CPLUS_INCLUDE_PATH=$C_INCLUDE_PATH
export LIBRARY_PATH=/mingwex/mingw64/lib/:/mingwex/mingw64/bin
export LD_LIBRARY_PATH=/mingwex/mingw64/bin
# export LD_PRELOAD=/mingw64/bin/libjemalloc.dll

# ================ Alias =================================
alias acd=addcurrentdir $*
alias def=exportdef $*
alias pbd='ping 36.152.44.96'
alias vi=vim $*
alias ls='ls -hF --color=tty'
alias ll='ls -l'
alias fs='python3 -m http.server' $*

# ================ Functions =============================
# ldd extension
function lddex() {
    ldd $* | sed '/\/c\//d'
}

function addcurrentdir () {
    if [ -z "$*" ]
    then
        PATH=`pwd`:$PATH
    else
        PATH=$*:$PATH
    fi
}

function exportdef () {
    local bn=$(basename ${1} .dll)
    pexports -o ${bn}".dll"  > ${bn}".def"
}

# Compilers
function SDCC () {
    function ihx2bin () {
        sdobjcopy -I ihex -O binary ${1%.*}'.ihx' ${1%.*}'.bin'
    }
    function ihx2hex () {
        packihx ${1%.*}'.ihx' > ${1%.*}y'.hex'
    }
    function 51cc () {
        sdcc ${1%.*}'.c'
        ihx2bin ${1%.*}
        ihx2hex ${1%.*}
        rm ${1%.*}'.map' ${1%.*}'.asm' ${1%.*}'.mem' ${1%.*}'.rel' ${1%.*}'.rst' ${1%.*}'.sym' ${1%.*}'.lk' ${1%.*}'.lst' ${1%.*}'.ihx'
    }
    function hex2bin () {
        objcopy -Iihex -Obinary ${1%.*}.hex ${1%.*}.bin
    }
}

function mcc () {
    cc $* -o ${1%.*}.exe
}

# 很不成熟
function cpip () {
    if [ "$1" == 'restore' ] ; then
        cpip back
        pip uninstall $(cpip diff)
    elif [ "$1" == 'diff' ] ; then
        local files=$(ls ~/.mrc/cpip/bak -1 | tail -n 2 | sed -e 's/^/D:\/MSYS2\/home\/Administrator\/.mrc\/cpip\/bak\//g' -e 's/\n/ /g')
        local libs=$(diff $files -y --suppress-common-lines | sed -e 's/>/\n/g' -e 's/\s//g')
        echo $libs
    elif [ "$1" == 'back' ] ; then
        local timestr=$(date +%F-%H-%M-%S)
        pip freeze > ~/.mrc/cpip/bak/$timestr'.bak'
    elif [ "$1" == 'list' ] ; then
        ls -1 ~/.mrc/cpip/bak
    elif [ "$1" == 'cat' ] ; then
        local tmp=$(ls -1 ~/.mrc/cpip/bak | tail -n 1)
        cat ~/.mrc/cpip/bak/$tmp
    elif [ -n "$1" ] ; then
        cpip back
        pip $*
    fi
}
