# ========================================================
# Stage 1: The Build Environment (builder)
# ========================================================
FROM ubuntu:22.04 AS builder

# Prevent interactive prompts during package installations
ENV DEBIAN_FRONTEND=noninteractive

# Install core build system dependencies, compilers, and vcpkg prerequisites
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    g++-12 \
    gcc-12 \
    cmake \
    ninja-build \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    ca-certificates \
    libssl-dev \
    uuid-dev \
    zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

# Enforce GCC-12 as the compiler to ensure robust C++20 coroutine support
ENV CC=gcc-12
ENV CXX=g++-12

# Bootstrap vcpkg globally inside the builder stage
ENV VCPKG_ROOT=/opt/vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT} \
    && ${VCPKG_ROOT}/bootstrap-vcpkg.sh

# Set working directory inside the container
WORKDIR /app

# --- ADVANCED CACHING TRICK FOR VCPKG ---
# Copy ONLY the manifest file first. This compiles Drogon and heavy dependencies
# to cache them. If you change your C++ source code later, Docker will NOT
# recompile your dependencies from scratch—saving you hours in CI/CD build times!
COPY vcpkg.json ./

# Trigger a dummy configure to force vcpkg to download and compile manifest dependencies
RUN echo "cmake_minimum_required(VERSION 3.20)\nproject(Dummy CXX)\nfind_package(drogon CONFIG REQUIRED)" > CMakeLists.txt \
    && cmake -B build -S . \
       -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake \
       -G Ninja \
    && rm -rf build CMakeLists.txt
# ----------------------------------------

# Now copy the actual source code and configuration files
COPY . .

# Configure and compile the real executable in Release mode
RUN cmake -B build -S . \
    -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -G Ninja

RUN cmake --build build --target eventflow_gateway


# ========================================================
# Stage 2: The Microscopic Runtime (runner)
# ========================================================
FROM ubuntu:22.04 AS runner

# Install minimal runtime requirements
# ca-certificates is critical for GcsSigner to make secure HTTPS cryptographic calls
RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    libssl3 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy ONLY the final binary from the builder environment
COPY --from=builder /app/build/eventflow_gateway ./eventflow_gateway

# Copy the server configuration file
COPY --from=builder /app/config.json ./config.json

# Expose port 8080 (Cloud Run default target port)
EXPOSE 8080

# Execute the gateway
CMD ["./eventflow_gateway"]