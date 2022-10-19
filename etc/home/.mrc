# ================ Path ==================================
PATH=/usr/bin:/mingw64/bin:/mingwex/mingw64/bin:$PATH
PATH=$PATH:/e/Compiler/TCC
# add registry paths
system_PATH_registry='/proc/registry/HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Control/Session Manager/Environment/Path'
system_PATH=$(cat "$system_PATH_registry" | xargs -0 cygpath -up )
user_PATH_registry='/proc/registry/HKEY_CURRENT_USER/Environment/Path'
user_PATH=$(cat "$user_PATH_registry" | xargs -0 cygpath -up )
PATH=$system_PATH:$user_PATH:$PATH

PATHW=$(rmrpt "$PATH")
PATH=$(cygpath -up "$PATHW")

# lua path
export LUA_CPATH=$PATHW
LUA_CPATH=$(setluap)
# Openblas
export OPENBLAS_NUM_THREADS=16

# ================ Alias =================================
alias acd=addcurrentdir $*
alias def=exportdef $*
alias erc='notepad ~/.mrc &'
alias clpkg='pacman -R $(pacman -Qdtq)'
alias pbd='ping 36.152.44.96'
alias cc=gcc $*
alias ls='ls -hF --color=tty'
alias make=mingw32-make $*
alias tp='PATH=$(chenv)'

# ================ Functions =============================
# C Interpreter
export C_CACHE_SIZE=$((10*1024)) # 10 MB
export C_CACHE_PATH=$temp"/c_cache"
# man-zh
export MANPATH=/usr/man:/usr/share/man:~/.manpages/man/
# Compilers
function SDCC () {
    PATH=/E/Compiler/SDCC/bin:$PATH
    function ihx2bin () {
        local body
        body=`echo $1 | cut -d . -f 1`
        sdobjcopy -I ihex -O binary $body'.ihx' $body'.bin'
    }
    function ihx2hex () {
        local body
        body=`echo $1 | cut -d . -f 1`
        packihx $body'.ihx' > $body'.hex'
    }
    function 51dbg () {
        java -jar F:\Documents\GitHub\math-lab\51-Single-chip\edsim51di\edsim51di.jar
    }
    function 51cc () {
        local pathbak
        pathbak=$PATH
        
        PATH=/E/Compiler/SDCC/bin:$PATH
        body=`echo $1 | cut -d . -f 1`
        
        sdcc $body'.c'
        ihx2bin $body
        ihx2hex $body
        
        rm $body'.map' $body'.asm' $body'.mem' $body'.rel' $body'.rst' $body'.sym' $body'.lk' $body'.lst' $body'.ihx'
        PATH=$pathbak
    }
    function hex2bin () {
        objcopy -Iihex -Obinary ${1%.*}.hex ${1%.*}.bin
    }
}

function sdl () {
    g++ $* `sdl2-config --libs` `sdl2-config --cflags` -lSDL2_image -o ${1%.*}.exe
}

function addcurrentdir () {
    if [[ -z $* ]]
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


# ================ GCC Search Dir ========================
# only add mingwex
export C_INCLUDE_PATH=/mingwex/mingw64/include/:/mingw64/include/eigen3
export CPLUS_INCLUDE_PATH=$C_INCLUDE_PATH
export LIBRARY_PATH=/mingwex/mingw64/lib/:/mingwex/mingw64/bin
export LD_LIBRARY_PATH=$LIBRARY_PATH


# ================ Auto Tasks ============================
if [[ $SSH_TTY ]] ; then
    echo Secure Shell
elif [[ -z $(pgrep sshd) ]] ; then
    printf 'starting sshd: \t'
    /usr/bin/sshd
    echo -e '\e[31m[\e[32mOK\e[31m]\e[0m'
fi
