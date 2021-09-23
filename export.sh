SPADAS_VERSION=8
TARGET_NAME=SpadasSDK

CUR_DIR=$(dirname "$0")
CUR_DATE=`date +%Y%m%d`

EXPORT_DEVELOPER=y
EXPORT_RUNTIME_DEBUG=y
if [ "$1" = "" ]; then
    TARGET_DIR=~/Desktop/$CUR_DATE-$TARGET_NAME
    GEN_DESKTOP_ZIP=y
else
    TARGET_DIR="$1"
    if [ "$2" = "runtime-with-debug" ]; then
        EXPORT_DEVELOPER=n
    fi
    if [ "$2" = "runtime-without-debug" ]; then
        EXPORT_DEVELOPER=n
        EXPORT_RUNTIME_DEBUG=n
    fi
fi

mkdir -vp $TARGET_DIR/bin64
cp -vf $CUR_DIR/bin64/spadas.dll $TARGET_DIR/bin64/
cp -vf $CUR_DIR/3party/windows/* $TARGET_DIR/bin64/

mkdir -vp $TARGET_DIR/binx
cp -vf $CUR_DIR/binx/libspadas.so.$SPADAS_VERSION $TARGET_DIR/binx/

mkdir -vp $TARGET_DIR/binxa
cp -vf $CUR_DIR/binxa/libspadas.so.$SPADAS_VERSION $TARGET_DIR/binxa/

if [ "$EXPORT_DEVELOPER" = "y" ]; then
    mkdir -vp $TARGET_DIR/include
    cp -vf $CUR_DIR/include/spadas* $TARGET_DIR/include/

    mkdir -vp $TARGET_DIR/lib64
    cp -vf $CUR_DIR/lib64/spadas.lib $TARGET_DIR/lib64/

    mkdir -vp $TARGET_DIR/libx
    cp -vf $CUR_DIR/libx/libspadas.so $TARGET_DIR/libx/

    mkdir -vp $TARGET_DIR/libxa
    cp -vf $CUR_DIR/libxa/libspadas.so $TARGET_DIR/libxa/
fi

if [ "$EXPORT_RUNTIME_DEBUG" = "y" ]; then
    mkdir -vp $TARGET_DIR/bin64/debug
    cp -vf $CUR_DIR/bin64/spadas.dll $TARGET_DIR/bin64/debug/
    cp -vf $CUR_DIR/3party/windows/* $TARGET_DIR/bin64/debug/

    mkdir -vp $TARGET_DIR/binx/debug
    cp -vf $CUR_DIR/binx/libspadas.so.$SPADAS_VERSION $TARGET_DIR/binx/debug/

    mkdir -vp $TARGET_DIR/binxa/debug
    cp -vf $CUR_DIR/binxa/libspadas.so.$SPADAS_VERSION $TARGET_DIR/binxa/debug/
fi

if [ "$GEN_DESKTOP_ZIP" = "y" ]; then
    cd ~/Desktop
    zip -r $CUR_DATE-$TARGET_NAME.zip ./$CUR_DATE-$TARGET_NAME/
    rm -r ./$CUR_DATE-$TARGET_NAME/
fi

echo "Done. (Spadas/export.sh)"
sleep 1
