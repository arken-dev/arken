mkdir tmp; cd tmp

git clone https://luajit.org/git/luajit.git
cd luajit
git checkout v2.1
git pull
make MACOSX_DEPLOYMENT_TARGET=12.1
mkdir -p tmp
cp src/libluajit.a tmp
cd tmp
ar -x libluajit.a
gcc -fPIC -shared *.o -o libluajit.so
cp libluajit.so ../../../deps/libluajit.so

echo "create dir /usr/local/lib"
sudo mkdir /usr/local/lib

echo "cp libluajit.so  /usr/local/lib/libluajit.so"
sudo cp libluajit.so  /usr/local/lib/libluajit.so

cd ..

mkdir -p ../../deps/include
mkdir -p ../../deps/include/lua

cp -pr src/lua*.h ../../deps/include/lua
cp -pr src/lua.hpp ../../deps/include/lua
cp -pr src/lauxlib.h ../../deps/include/lua

cd ..
cd luajit
make clean MACOSX_DEPLOYMENT_TARGET=12.1
