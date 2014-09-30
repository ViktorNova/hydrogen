#!/bin/bash
echo "Building Debian package for Stagecraft OS"
cd linux && debuild -us -uc && cd ..

