mkdir tmp; cd tmp
LUAJITVERSION=2.0.5

curl -L -O -C - http://luajit.org/download/LuaJIT-$LUAJITVERSION.tar.gz \
-o LuaJIT-$LUAJITVERSION.tar.gz

tar -xzvf LuaJIT-$LUAJITVERSION.tar.gz

cp ../util/Makefile/Makefile.jit.Darwin LuaJIT-$LUAJITVERSION/src/Makefile

cd LuaJIT-$LUAJITVERSION
make
#make install PREFIX=$PWD/../luajit
#mkdir -p tmp
#cp ../luajit/lib/libluajit.a tmp
#cd tmp
#ar -x libluajit.a
#gcc -fPIC -shared *.o -o libluajit.so
cp src/libluajit.so ../../deps/libluajit.so
#cd ..
mkdir -p ../../deps/include
mkdir -p ../../deps/include/lua
cp -pr src/lua*.h ../../deps/include/lua
cp -pr src/lua.hpp ../../deps/include/lua
cp -pr src/lauxlib.h ../../deps/include/lua

#cp -pr ../luajit/include/luajit-2.0/ ../../deps/include/lua
cd ..
rm -Rf tmp.tar.gz
rm -Rf luajit
rm -Rf LuaJIT-$LUAJITVERSION
#rm LuaJIT-$LUAJITVERSION.tar.gz
