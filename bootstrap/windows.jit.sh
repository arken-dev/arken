cd tmp
curl http://luajit.org/download/LuaJIT-2.0.4.tar.gz -o tmp.tar.gz
tar -xzvf tmp.tar.gz
cd LuaJIT-2.0.4
mingw32-make
cp src/lua51.dll ../../deps/liblua.dll
