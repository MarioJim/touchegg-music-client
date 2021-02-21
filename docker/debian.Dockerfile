FROM debian:bullseye

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        git \
        libcairo2-dev \
        libglib2.0-dev \
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
