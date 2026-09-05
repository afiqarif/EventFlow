#!/bin/bash

# This is a script for ease of build and run

BUILD_DIR="build"
EXECUTABLE="${BUILD_DIR}/eventflow_worker"

# Use VCPKG_ROOT if already set.
# Otherwise, try the common location ~/vcpkg.
VCPKG_ROOT="${VCPKG_ROOT:-$HOME/vcpkg}"

configure() {
    # Already configured
    if [ -f "$BUILD_DIR/CMakeCache.txt" ]; then
        echo "CMake is already configured. Skipping configuration."
        return 0
    fi

    # Check Ninja
    if ! command -v ninja &> /dev/null; then
        echo "Error: Ninja is not installed."
        echo "Install it with:"
        echo "  sudo apt install ninja-build"
        return 1
    fi

    # Check vcpkg
    if [ ! -f "$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" ]; then
        echo "Error: vcpkg toolchain file not found:"
        echo "  $VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
        echo
        echo "Current VCPKG_ROOT:"
        echo "  $VCPKG_ROOT"
        echo
        echo "Set VCPKG_ROOT to your vcpkg installation:"
        echo "  export VCPKG_ROOT=/path/to/vcpkg"
        return 1
    fi

    echo "Configuring with CMake + Ninja..."
    echo "vcpkg: $VCPKG_ROOT"

    cmake \
        -B "$BUILD_DIR" \
        -S . \
        -G Ninja \
        -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
}

build() {
    # Make sure CMake has been configured
    if [ ! -f "$BUILD_DIR/CMakeCache.txt" ]; then
        echo "Build directory is not configured."
        echo "Running CMake configuration..."
        
        configure || return 1
    fi

    echo "Building..."
    cmake --build "$BUILD_DIR"
}

run() {
    if [ ! -f "$EXECUTABLE" ]; then
        echo "Error: Executable not found:"
        echo "  $EXECUTABLE"
        echo
        echo "Build the project first."
        return 1
    fi

    echo "Running EventFlow Worker..."
    echo "=========================="

    "$EXECUTABLE"
}

echo
echo "EventFlow Worker"
echo "================"
echo "1) Configure"
echo "2) Build"
echo "3) Run"
echo "4) Build & Run"
echo "5) Configure, Build & Run"
echo "q) Quit"
echo

read -p "Select an option: " choice

case "$choice" in
    1)
        configure
        ;;

    2)
        build
        ;;

    3)
        run
        ;;

    4)
        build && run
        ;;

    5)
        configure && build && run
        ;;

    q|Q)
        echo "Bye!"
        exit 0
        ;;

    *)
        echo "Invalid option."
        ;;
esac