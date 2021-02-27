# Maintainer: Mario Jiménez <mario.emilio.j@gmail.com>

pkgname=touchegg-music-client
pkgver=0.1.1
pkgrel=1
pkgdesc="Widget for music and volume control, connected to touchegg's daemon"
arch=('i686' 'x86_64')
url="https://github.com/MarioJim/touchegg-music-client"
license=('GPL3')
depends=('touchegg' 'libpulse')
makedepends=('cmake')
source=("$pkgname-$pkgver.tar.gz::$url/archive/v$pkgver.tar.gz")
sha256sums=(5aaa81aa2d3a906eaf437d8af917b0c7b9527be0f3aa3befb98c3ebd97886d9a)

build() {
	cmake -B build -S "$pkgname-$pkgver" \
        -DCMAKE_BUILD_TYPE='None' \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -Wno-dev
	make -C build
}

package() {
    install -Dm 755 "build/touchegg_music_client" -t "${pkgdir}/usr/bin"
}
