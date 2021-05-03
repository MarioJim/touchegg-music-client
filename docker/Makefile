# Makefile to create docker images for building touchegg-music-client
# Mario Jiménez <mario.emilio.j@gmail.com>

# Usage:
# make                 # create and push both images
# make arch_image      # create and push archlinux-based image
# make debian_image    # create and push debian-based image

all: arch_image debian_image

arch_image:
	docker build . -f arch.Dockerfile -t mariojim/touchegg-music-client-deps:archlinux
	docker push mariojim/touchegg-music-client-deps:archlinux

debian_image:
	docker build . -f debian.Dockerfile -t mariojim/touchegg-music-client-deps:debian
	docker push mariojim/touchegg-music-client-deps:debian

.PHONY = all arch_image debian_image
