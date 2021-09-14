CUR_DIR=$(dirname "$0")

dos2unix $CUR_DIR/export.sh
chmod -v +x $CUR_DIR/export.sh
dos2unix $CUR_DIR/gen_apidoc.sh
chmod -v +x $CUR_DIR/gen_apidoc.sh

chmod -v +x $CUR_DIR/binx/test

chmod -v +x $CUR_DIR/binxa/test

sleep 3