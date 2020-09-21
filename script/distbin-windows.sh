cd ..
cp -pr arken arken-dist
cp arken/vendors/*.dll arken-dist/bin
cp $1/bin/libgcc_s_dw2-1.dll arken-dist/bin
cp $1/bin/libstdc++-6.dll arken-dist/bin
cp $1/bin/libwinpthread-1.dll arken-dist/bin
cp $1/bin/Qt5Concurrent.dll arken-dist/bin
cp $1/bin/Qt5Concurrentd.dll arken-dist/bin
cp $1/bin/Qt5Core.dll arken-dist/bin
cp $1/bin/Qt5Cored.dll arken-dist/bin
cp $1/bin/Qt5Network.dll arken-dist/bin
cp $1/bin/Qt5Networkd.dll arken-dist/bin
cp $1/bin/Qt5Widgets.dll arken-dist/bin
cp $1/bin/Qt5Widgetsd.dll arken-dist/bin
cp $1/bin/Qt5Gui.dll arken-dist/bin
cp $1/bin/Qt5Guid.dll arken-dist/bin
cp -r $1/plugins/platforms arken-dist/bin
