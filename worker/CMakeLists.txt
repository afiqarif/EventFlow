cmake_minimum_required(VERSION 3.20)

# Set vcpkg variables BEFORE the project() call
# set(VCPKG_OVERLAY_TRIPLETS "${CMAKE_CURRENT_SOURCE_DIR}/triplet")
# set(VCPKG_TARGET_TRIPLET "x64-linux-eventflow-worker")

project(EventFlowWorker CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages resolved by vcpkg
find_package(OpenCV REQUIRED)
find_package(google_cloud_cpp_storage REQUIRED)
find_package(google_cloud_cpp_pubsub REQUIRED)

# Glob all source files under src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(eventflow_worker ${SOURCES})

# Link dependencies to the worker binary
target_link_libraries(eventflow_worker PRIVATE
    ${OpenCV_LIBS}
    google-cloud-cpp::storage
    google-cloud-cpp::pubsub
)