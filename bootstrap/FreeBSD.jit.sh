mkdir tmp; cd tmp
LUAJITVERSION=2.0.5

curl -L -O -C - http://luajit.org/download/LuaJIT-$LUAJITVERSION.tar.gz \
-o LuaJIT-$LUAJITVERSION.tar.gz

tar -xzvf LuaJIT-$LUAJITVERSION.tar.gz

cp ../util/Makefile/Makefile.jit LuaJIT-$LUAJITVERSION/src/Makefile

cd LuaJIT-$LUAJITVERSION
gmake CC=/usr/local/bin/gcc10
gmake install PREFIX=$PWD/../luajit CC=/usr/local/bin/gcc10
mkdir -p tmp
cp ../luajit/lib/libluajit-5.1.a tmp
cd tmp
ar -x libluajit-5.1.a
gcc10 -fPIC -shared *.o -o libluajit.so
cp libluajit.so ../../../deps/libluajit.so
cd ..
#mkdir -p ../../deps/include
#cp -pr ../luajit/include/luajit-2.0/ ../../deps/include/lua
cd ..
rm -Rf tmp.tar.gz
rm -Rf luajit
rm -Rf LuaJIT-$LUAJITVERSION
#rm LuaJIT-$LUAJITVERSION.tar.gz
