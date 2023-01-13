export QT_MINGW64_DYNAMIC=/e/compiler/qt/mingw64
export QT_MINGW64_STATIC=/mingwex/mingw64

#export QT_MSVC_DYNAMIC=/e/qt/5.15.2/msvc2019_64
#export QT_MSVC_STATIC=/e/compiler/qt/msvc2019_64_static
                    # /e/compiler/qt/msvc2022_64_static

export qmd=$QT_MINGW64_DYNAMIC
export qms=$QT_MINGW64_STATIC

#export qvd=$QT_MSVC_DYNAMIC
#export qvs=$QT_MSVC_STATIC

PATH=$PATH:$QT_MINGW64_DYNAMIC/bin
