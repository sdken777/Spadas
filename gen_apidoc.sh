CUR_DIR=$(cd $(dirname $0) && pwd)

cd "$CUR_DIR"/doc
DOCUMENT_LANGUAGE=Chinese doxygen Doxyfile-Spadas
cp -vf ./index.html ~/Desktop/D02006_SpadasDoc_Chinese/
DOCUMENT_LANGUAGE=English doxygen Doxyfile-Spadas
cp -vf ./index.html ~/Desktop/D02006_SpadasDoc_English/

echo "Done. (Spadas/gen_apidoc.sh)"
sleep 1
