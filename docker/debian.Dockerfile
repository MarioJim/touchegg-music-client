FROM debian:bullseye

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        ca-certificates \
        cmake \
        git \
        libcairomm-1.0-dev \
        libglibmm-2.4-dev \
        libgtk-3-dev \
        libinput-dev \
        libpugixml-dev \
        libpulse-dev \
        libudev-dev \
        libx11-dev \
        libxi-dev \
        libxrandr-dev \
        libxtst-dev \
    && rm -rf /var/lib/apt/lists/*
