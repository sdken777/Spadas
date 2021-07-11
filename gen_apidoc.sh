cd doc
doxygen Doxyfile-Spadas
cp -vf ./index.html ~/Desktop/D02006_SpadasDoc/

cd ~/Desktop
zip -r D02006_SpadasDoc.zip ./D02006_SpadasDoc/
rm -r ./D02006_SpadasDoc/

sleep 3
