cmake_minimum_required(VERSION 3.20)

# Set the project name and declare it as C++
project(eventflow_gateway CXX)

# Inject .env into environment
set(ENV_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../.env")
if(EXISTS ${ENV_FILE})
    message(STATUS "Found .env file at ${ENV_FILE}. Injecting variables...")
    file(STRINGS ${ENV_FILE} ConfigContents)
    foreach(line ${ConfigContents})
        if(NOT line MATCHES "^#" AND line MATCHES "=")
            string(REGEX MATCH "^([^=]+)=(.*)$" _ ${line})
            set(ENV{${CMAKE_MATCH_1}} ${CMAKE_MATCH_2})
        endif()
    endforeach()
endif()

# ========================================================
# 1. Compiler Configuration (Enforcing C++20 & Optimizations)
# ========================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ========================================================
# 2. Resolve Dependencies (Fetched via vcpkg Manifest)
# ========================================================
# Note: These targets map directly to the vcpkg.json dependencies we defined
find_package(unofficial-argon2 CONFIG REQUIRED)
find_package(Drogon CONFIG REQUIRED)
find_package(jwt-cpp CONFIG REQUIRED)
find_package(google_cloud_cpp_storage REQUIRED)
find_package(google_cloud_cpp_pubsub REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(OpenSSL REQUIRED)

# ========================================================
# 3. Source Discovery (Globbing All Src Directories)
# ========================================================
# Automatically scans and gathers your controllers, filters, models, and utilities
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
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
    unofficial::argon2::libargon2
    jwt-cpp::jwt-cpp
)