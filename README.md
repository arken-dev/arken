# CHARON PLATFORM

Charon is a industrial platform for web development (and others) of extremely fast and scalable

![Charon image](images/logo2.jpg)

# ROADMAP

To follow the development roadmap: https://trello.com/b/27ccvQuw/charon-platform

http://charonplatform.org

# BUILD

### Qt (install qt lib 5.6 or later and add PATH var)
- download qt version in http://download.qt.io/official_releases/qt/
- example: http://download.qt.io/official_releases/qt/5.7/5.7.1/qt-opensource-linux-x64-5.7.1.run
- chmod u+x qt-opensource-linux-x64-5.7.1.run
- ./qt-opensource-linux-x64-5.7.1.run
- install in /opt/Qt/5.7.1
- add PATH var, ex PATH=/opt/Qt5.7.0/5.7/gcc_64/:$PATH

### bootstrap (add libraries and install lua lib/include in deps directory)

- execute bootstrap script
  bootstrap/debian.jit.sh
  bootstrap/debian.lua.sh
  bootstrap/windows.jit.sh
  bootstrap/macos.lua.sh


### cmake (build charon platform)

- mkdir build
- cd build
- cmake ..
- make -j4 (4 is number of cores)
