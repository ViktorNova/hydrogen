#!/bin/bash
BUNDLE_DIR=hydrogen_windows_32_bit
mkdir $BUNDLE_DIR
cd $BUNDLE_DIR
cp $HYDROGEN_BUILD/src/gui/hydrogen.exe .
cp $HYDROGEN_BUILD/src/core/libhydrogen-core-0.9.6.dll .
cp $HYDROGEN_BUILD/src/cli/h2cli.exe .
cp $HYDROGEN_BUILD/src/player/h2player.exe .
cp $HYDROGEN_BUILD/src/synth/h2synth.exe .

cp $MXE/usr/i686-w64-mingw32.shared/qt/bin/QtCore4.dll .
cp $MXE/usr/i686-w64-mingw32.shared/qt/bin/QtXml4.dll .
cp $MXE/usr/i686-w64-mingw32.shared/qt/bin/QtXmlPatterns4.dll .
cp $MXE/usr/i686-w64-mingw32.shared/qt/bin/QtNetwork4.dll .
cp $MXE/usr/i686-w64-mingw32.shared/qt/bin/QtGui4.dll .

cp $MXE/usr/lib/gcc/i686-w64-mingw32.shared/4.8.2/libgcc_s_sjlj-1.dll .
cp $MXE/usr/lib/gcc/i686-w64-mingw32.shared/4.8.2/libstdc++-6.dll .

cp $MXE/usr/i686-w64-mingw32.shared/bin/libsndfile-1.dll .
cp $MXE/usr/i686-w64-mingw32.shared/bin/libFLAC-8.dll .
cp $MXE/usr/i686-w64-mingw32.shared/bin/libogg-0.dll .
cp $MXE/usr/i686-w64-mingw32.shared/bin/libvorbis-0.dll .
cp $MXE/usr/i686-w64-mingw32.shared/bin/libvorbisenc-2.dll .

cp -r $HYDROGEN/data .
