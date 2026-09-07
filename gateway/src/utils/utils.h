#ifndef UTILS_H
#define UTILS_H

#include <json/json.h>
#include <drogon/drogon.h>
#include <unordered_map>
#include <vector>
#include <jtw-cpp/jwt.h>
#include <chrono>

namespace utils {
    // Utils Typedefs
    using StringMap = std::unordered_map<std::string, std::string>;     // Map for parsed payload
    using FieldList = std::vector<std::string>;                         // Vector for required field
    using DatabaseClient = std::shared_ptr<drogon::orm::DbClient>;      // DbClient data type

    // Inline functions
    inline std::string generateToken(
        const std::string& userId,
        const std::string& role,
        const std::string& secret
    )
    {
        auto token = jwt::create()
            .set_issuer("eventflow-gateway")
            .set_type("JWS")
            .set_issued_at(
                std::chrono::system_clock::now()
            )
            // Expire the token in 24 hours
            .set_expires_at(
                std::chrono::system_clock::now() +
                std::chrono::hours{24}
            )
            // Custom payload claims (must wrap native types in jwt::claim)
            .set_payload_claim(
                "userId",
                jwt::claim(userId)
            )
            .set_payload_claim(
                "role",
                jwt::claim(role)
            )
            // Sign the token cryptographically using HMAC SHA-256
            .sign(jwt::algorithm::hs256{secret});

        return token;
    }

    // Utils functions
    drogon::HttpResponsePtr errorRequest(
        const std::string& errorMessage,
        drogon::HttpStatusCode statusCode
    );

    std::string hash(const std::string& password);

    std::unordered_map<std::string, std::string> parseJsonString(
        const std::shared_ptr<Json::Value>& jsonBody,
        const std::vector<std::string>& expectedKeys
    );

    drogon::HttpResponsePtr validatePayload(
        const StringMap& parsedData,
        const FieldList& requiredFields
    );
}
#endif