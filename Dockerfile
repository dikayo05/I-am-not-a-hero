# ============================
# Stage 1: Build Environment
# ============================
FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
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
    libegl1-mesa-dev \
    libxrandr2 \
    libxcursor1 \
    libxi6 \
    libfreetype6 \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Build project
RUN cmake -B build && cmake --build build

# ============================
# Stage 2: Runtime Environment
# ============================
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# Install runtime dependencies
RUN apt update && apt install -y \
    libxrandr2 \
    libxcursor1 \
    libxi6 \
    libfreetype6 \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy built binary and resources from builder stage
COPY --from=builder /app/build/bin/main .
COPY --from=builder /app/assets ./assets

# Run the game
CMD ["./main"]

RUN ldd ./main