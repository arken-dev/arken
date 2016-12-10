cd ..
cp -pr charon charon-dist
cp charon/vendors/*.dll charon-dist/bin
cp $1/bin/libgcc_s_dw2-1.dll charon-dist/bin
cp $1/bin/libstdc++-6.dll charon-dist/bin
cp $1/bin/libwinpthread-1.dll charon-dist/bin
cp $1/bin/Qt5Concurrent.dll charon-dist/bin
cp $1/bin/Qt5Concurrentd.dll charon-dist/bin
cp $1/bin/Qt5Core.dll charon-dist/bin
cp $1/bin/Qt5Cored.dll charon-dist/bin
cp $1/bin/Qt5Network.dll charon-dist/bin
cp $1/bin/Qt5Networkd.dll charon-dist/bin
cp $1/bin/Qt5Widgets.dll charon-dist/bin
cp $1/bin/Qt5Widgetsd.dll charon-dist/bin
cp $1/bin/Qt5Gui.dll charon-dist/bin
cp $1/bin/Qt5Guid.dll charon-dist/bin
cp -r $1/plugins/platforms charon-dist/bin
