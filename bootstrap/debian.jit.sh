sudo apt-get install   \
  build-essential      \
  libgl1-mesa-dev      \
  curl                 \
  libcurl4-openssl-dev \
  libpq-dev

cd tmp
curl http://luajit.org/download/LuaJIT-2.0.4.tar.gz -o tmp.tar.gz
tar -xzvf tmp.tar.gz
cp ../bootstrap/debian.jit/Makefile LuaJIT-2.0.4/src/
cd LuaJIT-2.0.4
make
cd src
mkdir tmp
cp libluajit.a tmp
cd tmp
ar -x libluajit.a
gcc -fPIC -shared *.o -o liblua.so
cp liblua.so ../../../../deps/liblua.so
