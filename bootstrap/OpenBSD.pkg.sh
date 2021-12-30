# required for OpenBSD.lua.sh and OpenBSD.jit.sh
pkg_add gmake gcc-11.2.0p0

# cmake and cmake-gui
pkg_add cmake

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
pkg_add postgresql-client

# arken.Image
pkg_add ImageMagick

# arken.notify
# in working
