# Makefile to create docker images for building touchegg-music-client
# Mario Jiménez <mario.emilio.j@gmail.com>

# Usage:
# make                    # create and push both images
# make archlinux-image    # create and push archlinux-based image
# make debian-image       # create and push debian-based image

all: archlinux-image debian-image

archlinux-image:
	docker build . -f Dockerfile.arch -t mariojim/touchegg-music-client-deps:archlinux
	docker push mariojim/touchegg-music-client-deps:archlinux

debian-image:
	docker build . -f Dockerfile.debian -t mariojim/touchegg-music-client-deps:debian
	docker push mariojim/touchegg-music-client-deps:debian

.PHONY = all archlinux-image debian-image
