cmake_minimum_required(VERSION 3.20)

# Set the project name and declare it as C++
project(eventflow_gateway CXX)

# ========================================================
# 1. Compiler Configuration (Enforcing C++20 & Optimizations)
# ========================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Inject rigorous warning flags and maximize Release optimizations
if(NOT MSVC)
    add_compile_options(-Wall -Wextra -O3 -pthread)
else()
    add_compile_options(/W4 /O2)
endif()

# ========================================================
# 2. Resolve Dependencies (Fetched via vcpkg Manifest)
# ========================================================
# Note: These targets map directly to the vcpkg.json dependencies we defined
find_package(Drogon REQUIRED)
find_package(google_cloud_cpp_storage REQUIRED)
find_package(google_cloud_cpp_pubsub REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(OpenSSL REQUIRED)

# ========================================================
# 3. Source Discovery (Globbing All Src Directories)
# ========================================================
# Automatically scans and gathers your controllers, filters, models, and utilities
file(GLOB_RECURSE SOURCE_FILES 
    "src/*.cpp" 
    "src/*.cc" 
    "src/*.h"
)

# ========================================================
# 4. Define and Configure Target Executable
# ========================================================
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Configure compiler search paths.
# This lets you write clean includes like #include "controllers/UploadController.h"
# instead of messy relative paths like #include "../controllers/UploadController.h"
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ========================================================
# 5. Link Libraries
# ========================================================
target_link_libraries(${PROJECT_NAME} PRIVATE
    Drogon::Drogon
    google-cloud-cpp::storage
    google-cloud-cpp::pubsub
    nlohmann_json::nlohmann_json
    OpenSSL::SSL
    OpenSSL::Crypto
)

# ========================================================
# 6. Quality-of-Life Helper for Local Testing
# ========================================================
# This post-build step automatically copies your config.json next to your compiled 
# executable. That way, when you run your binary locally, Drogon can find its 
# configuration file instantly without you having to manually move files!
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/config.json"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/config.json"
    COMMENT "Copying config.json to build target directory..."
)