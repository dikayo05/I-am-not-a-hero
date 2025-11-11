FROM ubuntu:24.04

# Install dependencies
RUN apt update && apt install -y \
    git \
    g++ \
    cmake \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libudev-dev \
    libfreetype-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev 

# clear cache to make image smaller
RUN rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy project files
COPY . .

# Clean previous builds
RUN rm -rf build

# Build project with cmake
RUN cmake -B build && cmake --build build