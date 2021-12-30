# required for FreeBSD.lua.sh and FreeBSD.jit.sh
pkg install gmake

# require for compile arken minimal gcc9
# add in your .profile
# export CC=/usr/pkg/gcc10/bin/gcc
# export CXX=/usr/pkg/gcc10/bin/g++
pkg install gcc10

# cmake and cmake-gui
pkg install cmake cmake-gui # cmake-gui require libxcb

# arken.compress.Zip (required)
pkg install libzip

# arken.digest.(md5|sha1) arken.uuid and others (required)
# installed by default

# arken.net.HttpClient (required)
# installed by default

# arken.net.HttpServer
pkg install libev-dev # default

# mysql
pkg install mysql57-client

# postgres
pkg install postgresql96-client

# arken.Image
pkg install ImageMagick6

# arken.notify
# in working
