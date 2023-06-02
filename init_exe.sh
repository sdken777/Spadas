CUR_DIR=$(dirname "$0")

dos2unix "$CUR_DIR"/export.sh
chmod -v +x "$CUR_DIR"/export.sh

dos2unix "$CUR_DIR"/gen_apidoc.sh
chmod -v +x "$CUR_DIR"/gen_apidoc.sh

chmod -v +x "$CUR_DIR"/binx/test
chmod -v +x "$CUR_DIR"/binxa/test
chmod -v +x "$CUR_DIR"/binm/test
chmod -v +x "$CUR_DIR"/binma/test

echo "Done. (Spadas/init_exe.sh)"
sleep 1
