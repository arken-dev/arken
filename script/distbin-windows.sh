cd ..
cp -pr oberon oberon-dist
cp oberon/vendors/*.dll oberon-dist/bin
cp $1/bin/libgcc_s_dw2-1.dll oberon-dist/bin
cp $1/bin/libstdc++-6.dll oberon-dist/bin
cp $1/bin/libwinpthread-1.dll oberon-dist/bin
cp $1/bin/Qt5Concurrent.dll oberon-dist/bin
cp $1/bin/Qt5Concurrentd.dll oberon-dist/bin
cp $1/bin/Qt5Core.dll oberon-dist/bin
cp $1/bin/Qt5Cored.dll oberon-dist/bin
cp $1/bin/Qt5Network.dll oberon-dist/bin
cp $1/bin/Qt5Networkd.dll oberon-dist/bin
cp $1/bin/Qt5Widgets.dll oberon-dist/bin
cp $1/bin/Qt5Widgetsd.dll oberon-dist/bin
cp $1/bin/Qt5Gui.dll oberon-dist/bin
cp $1/bin/Qt5Guid.dll oberon-dist/bin
cp -r $1/plugins/platforms oberon-dist/bin
