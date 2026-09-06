#ifndef UTILS_H
#define UTILS_H

#include <json/json.h>
#include <drogon/drogon.h>
#include <unordered_map>
#include <vector>

namespace utils {
    // Utils Typedefs
    using StringMap = std::unordered_map<std::string, std::string>;


    // Utils functions
    drogon::HttpResponsePtr makeBadRequest(const std::string& errorMessage);

    std::string hash(const std::string& password);

    std::unordered_map<std::string, std::string> parseJsonString(
        const std::shared_ptr<Json::Value>& jsonBody,
        const std::vector<std::string>& expectedKeys
    );
}
#endif