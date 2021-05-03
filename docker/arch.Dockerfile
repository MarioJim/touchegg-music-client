FROM archlinux:base-devel

RUN echo "Server = https://mirror.osbeck.com/archlinux/\$repo/os/\$arch" > /etc/pacman.d/mirrorlist \
    && pacman -Syu --noconfirm \
        cairo \
        cmake \
        git \
        glibmm \
        gtk3 \
        libinput \
        libpulse \
        libx11 \
        libxrandr \
        libxtst \
        pugixml
