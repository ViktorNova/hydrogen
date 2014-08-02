# Maintainer: hbdee <hbdee.arch@gmail.com>

pkgname=hydrogen-stagecraft-git
_pkgname=hydrogen
pkgver=0.9.6.beta2.r235.g7d3ccb6
pkgrel=1
pkgdesc="An advanced drum machine - git version"
arch=('i686' 'x86_64')
license=('GPL')
url="https://github.com/viktornova/hydrogen-stagecraft"
depends=('libarchive' 'liblrdf' 'qt4' 'jack')
makedepends=('git' 'cmake' 'optipng')
source=("$_pkgname"::'git://github.com/viktornova/hydrogen-stagecraft.git')
install=$_pkgname.install
options=('emptydirs')
provides=('hydrogen')
conflicts=('hydrogen')
md5sums=('SKIP')

pkgver() {
  cd "$srcdir/$_pkgname"
  # Use the tag of the last commit
  git describe --long | sed -E 's/([^-]*-g)/r\1/;s/-/./g'
}

prepare() {
  if [[ -d build ]]
  then
    rm -rf build
  fi
   mkdir build
  cd "$srcdir/$_pkgname"

  # fix some pngs that break with newer libpng
  # see https://mailman.archlinux.org/pipermail/arch-dev-public/2013-May/024872.html
  msg2 "Fixing PNGs, please wait..."
  find -name '*.png' -exec optipng -quiet -force -fix {} +
}

build() {
  cd "$srcdir/$_pkgname"

  cmake ../$_pkgname \
    -DCMAKE_INSTALL_PREFIX=/usr
  make
}

package() {
  cd "$srcdir/$_pkgname"

  make DESTDIR="${pkgdir}" install
}

# vim:set ts=2 sw=2 et:
