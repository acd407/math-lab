# 所有命令行操作，尽量使用 Alacritty-Bash 完成

# PRE: Shell Judgment
if [[ $SSH_TTY ]] ; then
    echo Secure Shell
else
    echo Local Shell
    if [[ -z $(/usr/bin/pgrep sshd) ]] ; then
        echo -ne 'starting sshd: \t\t'
        /usr/bin/sshd
        echo -e '\e[31m[\e[32mOK\e[31m]\e[0m'
    fi
fi

# ====================== START ===========================
echo -en 'loading mrc: \t\t'

# ====================== Path ============================
#这样tp才能用
PATH=/mingw64/bin:/usr/bin:\
/mingwex/mingw64/bin:$PATH:\
/e/Compiler/TCC:$HOME/.mrc/bin:\
/e/Interpreter/Python:/e/Interpreter/Python/Scripts:\
/d/PATH/Scoop/shims:/d/PATH/bin:/e/MATH/Pari64

# add registry paths
#system_PATH_registry='/proc/registry/HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Control/Session Manager/Environment/Path'
#system_PATH=$(cat "$system_PATH_registry" | xargs -0 cygpath -up )
#user_PATH_registry='/proc/registry/HKEY_CURRENT_USER/Environment/Path'
#user_PATH=$(cat "$user_PATH_registry" | xargs -0 cygpath -up )
#PATH=$PATH:$system_PATH:$user_PATH
#unset system_PATH_registry system_PATH user_PATH_registry user_PATH

# PATH不带最后的斜杠
PATH=${PATH//\/:/:}
# 整理 PATH，删除重复路径
if [ -n "$PATH" ]; then
    old_PATH=$PATH:; PATH=
    while [ -n "$old_PATH" ]; do
        x=${old_PATH%%:*}      
        case $PATH: in
           *:"$x":*) ;;         
           *) PATH=$PATH:$x;;  
        esac
        old_PATH=${old_PATH#*:}
    done
    PATH=${PATH#:}
    unset old_PATH x
fi


# ====================== Alias ===========================
alias clpkg='pacman -R $(pacman -Qdtq)'
alias pc='pkg-config --cflags --libs' $*

# ================== Core Functions ======================
function erc () {
    declare -A map
    map=(
        ['b']='.bashrc'
        ['m']='.mrc/.mrc'
        ['c']='.mrc/cfgrc.sh'
        ['q']='.mrc/qtrc.sh'
    )
    if [ "$1" == '--help' ] ||
       [ "$1" == '-h' ]     ||
       [ "$1" == 'h' ];then
        echo -e 'edit rc file of bash.'
        echo -e 'Usage: erc [options]\nOptions:'
        echo -e '\th/-h/--help: print this help context'
        for key in ${!map[*]};do
            echo -e "\t$key: ~/${map[$key]}"
        done
    elif [ -z "$1" ];then
        notepad $HOME/${map[m]} &
    elif [[ "${!map[*]}" =~ "$1" ]];then
        notepad $HOME/${map[$1]} &
    else
        echo 'options error!'
    fi
}

function tp () {
    str=${PATH:22}
    str1='/mingw64/bin:/usr/bin:'
    str2='/usr/bin:/mingw64/bin:'
    if [ "${PATH:0:22}" == "$str1" ];then
        PATH=$str2$str
    else
        PATH=$str1$str
    fi
}

function chenv () {
    path=${PATH// /#}
    path=(${path//:/ })
    for ((i=0;i<${#path[*]};i++)); do
        if [ '/usr/bin' = "${path[$i]}" ];then
            i1=$i
        elif [ '/mingw64/bin/' = "${path[$i]}" ];then
            i2=$i
        fi
        if [ $i1 ] && [ $i2 ];then
            break
        fi
    done
    tmp_path=${path[$i1]}
    path[$i1]=${path[$i2]}
    path[$i2]=$tmp_path
    path=${path[*]}
    path=${path// /:}
    echo ${path//#/ }
}

# ======================= END ============================
echo -e '\e[31m[\e[32mOK\e[31m]\e[0m'

for _file_ in $(echo $HOME/.mrc/*.sh);do
    _base_=${_file_#$HOME/.mrc/}
    echo -en "loading ${_base_%.sh}: \t\t"
    source $_file_
    echo -e '\e[31m[\e[32mOK\e[31m]\e[0m'
done

source /usr/share/bash-completion/bash_completion