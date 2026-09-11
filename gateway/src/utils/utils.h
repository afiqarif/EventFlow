#ifndef UTILS_H
#define UTILS_H

#include <json/json.h>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/open-source-parsers-jsoncpp/traits.h>
#include <drogon/drogon.h>
#include <unordered_map>
#include <vector>
#include <chrono>

namespace utils {
    // Utils Typedefs
    using StringMap = std::unordered_map<std::string, std::string>;     // Map for parsed payload
    using FieldList = std::vector<std::string>;                         // Vector for required field
    using DatabaseClient = std::shared_ptr<drogon::orm::DbClient>;      // DbClient data type
    using jsoncpp_traits = jwt::traits::open_source_parsers_jsoncpp;    // JsonCpp traits
    using jwt_claim = jwt::basic_claim<jsoncpp_traits>;                       // Claim alias for jwt-cpp using JsonCpp

    // Inline functions
    inline auto jwt_create() {
        return jwt::create<jsoncpp_traits>();
    }

    inline std::string generateToken(
        const std::string& userId,
        const std::string& role,
        const std::string& secret
    )
    {
        return jwt_create()
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
                jwt_claim(Json::Value(userId))
            )
            .set_payload_claim(
                "role",
                jwt_claim(Json::Value(role))
            )
            // Sign the token cryptographically using HMAC SHA-256
            .sign(jwt::algorithm::hs256{secret});
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
