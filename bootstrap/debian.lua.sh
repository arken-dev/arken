apt-get install        \
  build-essential      \
  libgl1-mesa-dev      \
  curl                 \
  libcurl4-openssl-dev \
  libpq-dev

cd tmp
curl http://www.lua.org/ftp/lua-5.1.5.tar.gz -o tmp.tar.gz
tar -xzvf tmp.tar.gz
cp ../bootstrap/debian/Makefile.lua lua-5.1.5/src/Makefile
cd lua-5.1.5
make linux test
cd src
mkdir tmp
cp liblua.a tmp
cd tmp
ar -x liblua.a
gcc -fPIC -shared *.o -o liblua.so
cp liblua.so ../../../../deps/liblua.so
