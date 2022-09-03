# required for Linux.lua.sh and Linux.jit.sh
apt-get install build-essential curl

# cmake and cmake-gui
apt-get install cmake cmake-gui

# arken.compress.Zip (required)
apt-get install libzip-dev

# arken.digest.(md5|sha1) arken.uuid and others (required)
apt-get install libssl-dev

# arken.net.HttpClient (required)
apt-get install libcurl4-gnutls-dev
# apt-get install libcurl4-openssl-dev # (alternative for gnutls)

# arken.net.HttpServer
apt-get install libev-dev # default

# mysql
apt-get install default-libmysqlclient-dev
# apt-get install libmysqlclient-dev # olders debian

# postgres
apt-get install libpq-dev

# arken.Image
# apt-get install libmagickcore-dev libmagickwand-dev

# arken.notify
# apt-get install qtbase5-dev libqt5opengl5-dev  # qt
# apt-get install libnotify-dev      # libnotify
# apt-get install libglib2.0-dev     # glib
# apt-get install freeglut3-dev      # opengl
