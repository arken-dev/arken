# required for OpenBSD.lua.sh and OpenBSD.jit.sh
pkg_add gmake

# require for compile arken minimal gcc9
# add in your .profile
# export CC=/usr/pkg/gcc10/bin/gcc
# export CXX=/usr/pkg/gcc10/bin/g++
pkg_add gcc10

# cmake and cmake-gui
pkg_add cmake cmake-gui libxcb # cmake-gui require libxcb

# arken.compress.Zip (required)
pkg_add libzip

# arken.digest.(md5|sha1) arken.uuid and others (required)
# installed by default

# arken.net.HttpClient (required)
# installed by default

# arken.net.HttpServer
pkg_add libev-dev # default

# mysql
pkg_add mariadb-client

# postgres
pkg_add libpqxx

# arken.Image
pkg_add ImageMagick6

# arken.notify
# in working
