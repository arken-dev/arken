apt-get install        \
  build-essential      \
  libgl1-mesa-dev      \
  curl                 \
  libcurl4-openssl-dev \
  libpq-dev            \
  libmysqlclient-dev   \
  libzip-dev

cd tmp
LUAJITVERSION=2.0.5
wget -c http://luajit.org/download/LuaJIT-$LUAJITVERSION.tar.gz
tar -xzvf LuaJIT-$LUAJITVERSION.tar.gz
cp ../bootstrap/debian/Makefile.jit LuaJIT-$LUAJITVERSION/src/Makefile
cd LuaJIT-$LUAJITVERSION
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
cd ..
rm LuaJIT-$LUAJITVERSION.tar.gz
rm -Rf LuaJIT-$LUAJITVERSION
rm -Rf tmp.tar.gz
rm -Rf luajit
