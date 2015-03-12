#!/bin/bash
nano linux/debian/changelog && \
echo "Building Debian package for Stagecraft OS" && \
cd linux && fakeroot dpkg-buildpackage && cd .. && ls -l


