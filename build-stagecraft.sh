#!/bin/bash
echo "Building Debian package for Stagecraft OS"
cd linux && fakeroot dpkg-buildpackage && cd ..

