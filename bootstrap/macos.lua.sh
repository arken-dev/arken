cd tmp
curl http://www.lua.org/ftp/lua-5.1.5.tar.gz -o tmp.tar.gz
tar -xzvf tmp.tar.gz
cd lua-5.1.5
make macosx test
make install INSTALL_TOP=$PWD/../lua
mkdir -p ../../deps/include
cp -pr ../lua/include ../../deps/include/lua
cd src
mkdir tmp
cp liblua.a tmp
cd tmp
ar -x liblua.a
gcc -shared *.o -o liblua.so
cp liblua.so ../../../../deps/liblua.so
