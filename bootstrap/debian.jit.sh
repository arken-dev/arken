apt-get install        \
  build-essential      \
  libgl1-mesa-dev      \
  curl                 \
  libcurl4-openssl-dev \
  libpq-dev

cd tmp
curl http://luajit.org/download/LuaJIT-2.0.4.tar.gz -o tmp.tar.gz
tar -xzvf tmp.tar.gz
#cp ../bootstrap/debian/Makefile.jit LuaJIT-2.0.4/src/Makefile
cd LuaJIT-2.0.4
make
make install PREFIX=$PWD/../luajit
mkdir -p tmp
cp ../luajit/lib/libluajit-5.1.a tmp
cd tmp
ar -x libluajit-5.1.a
gcc -fPIC -shared *.o -o liblua.so
cp liblua.so ../../../deps/liblua.so
cd ..
mkdir -p ../../deps/include
cp -pr ../luajit/include/luajit-2.0/ ../../deps/include/lua
#rm -Rf LuaJIT-2.0.4
#rm -Rf tmp.tar.gz
#rm -Rf luajit
