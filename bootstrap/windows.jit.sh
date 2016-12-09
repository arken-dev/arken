cd tmp
rm -Rf windows
curl http://charonplatform.org/download/windows.zip  -o windows.zip
unzip windows.zip
mv windows/* ../deps/

curl http://luajit.org/download/LuaJIT-2.0.4.tar.gz -o tmp.tar.gz
tar -xzvf tmp.tar.gz
cd LuaJIT-2.0.4
mingw32-make
mingw32-make install PREFIX=$PWD/../luajit
cp src/lua51.dll ../../deps/liblua.dll
cp -pr ../luajit/include/luajit-2.0/ ../../deps/include/lua
