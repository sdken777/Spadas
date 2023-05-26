CUR_DIR=$(dirname "$0")

cd "$CUR_DIR"/doc
doxygen Doxyfile-Spadas
cp -vf ./index.html ~/Desktop/D02007_SpadasDoc/

cd ~/Desktop
zip -r D02007_SpadasDoc.zip ./D02007_SpadasDoc/
rm -r ./D02007_SpadasDoc/

echo "Done. (Spadas/gen_apidoc.sh)"
sleep 1
