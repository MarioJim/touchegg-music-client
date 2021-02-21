FROM archlinux:base-devel

RUN pacman -Syu --noconfirm \
    cairo \
    cmake \
    gcc \
    git \
    glib2 \
    gtk3 \
    libinput \
    libpulse \
    libx11 \
    libxrandr \
    libxtst \
    pugixml \
    systemd-libs
