# ==========================================
# Stage 1: Build Environment
# ==========================================
FROM nvidia/cuda:13.3.0-devel-ubuntu22.04 AS builder

# Prevent interactive prompts during apt installations
ENV DEBIAN_FRONTEND=noninteractive

# Install system build dependencies, CMake, and TensorRT development libraries
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    ninja-build \
    libnvinfer-dev \
    && rm -rf /var/lib/apt/lists/*

# Install vcpkg
ENV VCPKG_ROOT=/opt/vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT} \
    && ${VCPKG_ROOT}/bootstrap-vcpkg.sh -disableMetrics

# Set up the working directory
WORKDIR /app

# Copy the vcpkg manifest and CMake configuration first to cache dependency builds
COPY vcpkg.json CMakeLists.txt ./

# Run a dummy CMake configure to trigger vcpkg dependency installation
# This uses the dependencies listed in vcpkg.json (opencv, google-cloud-cpp)
RUN cmake -B build -S . \
    -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_BUILD_TYPE=Release

# Copy the rest of the source code (globbed under src/*.cpp)
COPY src/ ./src/

# Build the eventflow_worker executable
RUN cmake --build build --config Release -j$(nproc)

# ==========================================
# Stage 2: Runtime Environment
# ==========================================
FROM nvidia/cuda:13.3.0-runtime-ubuntu22.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

# Install runtime dependencies for TensorRT and OpenCV
RUN apt-get update && apt-get install -y \
    libnvinfer8 \
    libglib2.0-0 \
    libsm6 \
    libxext6 \
    libxrender-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the compiled binary from the builder stage
COPY --from=builder /app/build/eventflow_worker /app/eventflow_worker

# Set the entrypoint to the executable
ENTRYPOINT ["/app/eventflow_worker"]