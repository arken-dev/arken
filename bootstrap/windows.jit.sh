cd tmp
rm -Rf windows
curl http://charonplatform.org/download/windows-jit.zip  -o windows-jit.zip
unzip windows-jit.zip
mv windows-jit/* ../deps/
