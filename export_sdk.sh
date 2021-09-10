SPADAS_VERSION=8

CUR_DIR=$(dirname "$0")
CUR_DATE=`date +%Y%m%d`

if [ -z "$1" ]; then
    TARGET_DIR=~/Desktop/$CUR_DATE-SpadasSDK
    GEN_DESKTOP_ZIP=yes
else
    TARGET_DIR="$1"
fi

mkdir -vp $TARGET_DIR/include
cp -vf $CUR_DIR/include/spadas* $TARGET_DIR/include/

mkdir -vp $TARGET_DIR/lib64
cp -vf $CUR_DIR/lib64/spadas.lib $TARGET_DIR/lib64/

mkdir -vp $TARGET_DIR/bin64
cp -vf $CUR_DIR/bin64/spadas.dll $TARGET_DIR/bin64/
cp -vf $CUR_DIR/3party/windows/* $TARGET_DIR/bin64/

mkdir -vp $TARGET_DIR/bin64/debug
cp -vf $CUR_DIR/bin64/spadas.dll $TARGET_DIR/bin64/debug/
cp -vf $CUR_DIR/3party/windows/* $TARGET_DIR/bin64/debug/

mkdir -vp $TARGET_DIR/libx
cp -vf $CUR_DIR/libx/libspadas.so $TARGET_DIR/libx/

mkdir -vp $TARGET_DIR/binx
cp -vf $CUR_DIR/binx/libspadas.so.$SPADAS_VERSION $TARGET_DIR/binx/

mkdir -vp $TARGET_DIR/binx/debug
cp -vf $CUR_DIR/binx/libspadas.so.$SPADAS_VERSION $TARGET_DIR/binx/debug/

mkdir -vp $TARGET_DIR/libxa
cp -vf $CUR_DIR/libxa/libspadas.so $TARGET_DIR/libxa/

mkdir -vp $TARGET_DIR/binxa
cp -vf $CUR_DIR/binxa/libspadas.so.$SPADAS_VERSION $TARGET_DIR/binxa/

mkdir -vp $TARGET_DIR/binxa/debug
cp -vf $CUR_DIR/binxa/libspadas.so.$SPADAS_VERSION $TARGET_DIR/binxa/debug/

if [ ! -z $GEN_DESKTOP_ZIP ]; then
    cd ~/Desktop
    zip -r $CUR_DATE-SpadasSDK.zip ./$CUR_DATE-SpadasSDK/
    rm -r ./$CUR_DATE-SpadasSDK/
fi

sleep 3
