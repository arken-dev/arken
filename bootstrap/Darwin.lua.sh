mkdir tmp; cd tmp
curl -L -O -C - http://www.lua.org/ftp/lua-5.1.5.tar.gz -o lua-5.1.5.tar.gz
tar -xzvf lua-5.1.5.tar.gz
cp ../util/Makefile/Makefile.lua lua-5.1.5/src/Makefile
cd lua-5.1.5
make macosx test
cd src
mkdir tmp
cp liblua.a tmp
cd tmp
ar -x liblua.a
gcc -fPIC -shared *.o -o liblua.so
cp liblua.so ../../../../deps/liblua.so
cd ../../..
rm -Rf lua-5.1.5
