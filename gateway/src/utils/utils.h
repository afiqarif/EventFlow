#ifndef UTILS_H
#define UTILS_H

#include <json/json.h>
#include <drogon/drogon.h>

namespace utils {
    // Generates a standardized 400 Bad Request JSON response
    drogon::HttpResponsePtr makeBadRequest(const std::string& errorMessage);

    std::string hash(std::string password);
}
#endif