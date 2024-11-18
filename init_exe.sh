CUR_DIR=$(cd $(dirname $0) && pwd)

dos2unix "$CUR_DIR"/export.sh
chmod -v +x "$CUR_DIR"/export.sh

dos2unix "$CUR_DIR"/gen_apidoc.sh
chmod -v +x "$CUR_DIR"/gen_apidoc.sh

chmod -v +x "$CUR_DIR"/binx/test
chmod -v +x "$CUR_DIR"/binxa/test
chmod -v +x "$CUR_DIR"/binm/test
chmod -v +x "$CUR_DIR"/binma/test

cp -vf "$CUR_DIR"/3party/windows/*.* "$CUR_DIR"/bin64/
cp -vf "$CUR_DIR"/3party/windows/*.* "$CUR_DIR"/bin64/debug/

echo "Done. (Spadas/init_exe.sh)"
sleep 1
