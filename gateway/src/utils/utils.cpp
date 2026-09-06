#include "utils.h"
#include <argon2.h>
#include <random>

namespace utils {
    drogon::HttpResponsePtr makeBadRequest(const std::string& errorMessage) 
    {
        Json::Value errorRet;
        errorRet["success"] = false;
        errorRet["error"] = errorMessage;

        drogon::HttpResponsePtr resp = drogon::HttpResponse::newHttpJsonResponse(errorRet);
        resp->setStatusCode(drogon::k400BadRequest);
        return resp;
    }

    std::string hash(const std::string& pwd)
    {
        // Generate a secure 16-byte random salt
        std::vector<uint8_t> salt(16);
        std::random_device rd;
        for (auto& b : salt)
            b = static_cast<uint8_t>(rd());

        // Define Argon2 security parameters
        uint32_t t_cost = 2;            // Time Cost (iterations)
        uint32_t m_cost = (1 << 16);    // Memory Cost (64 MB)
        uint32_t parallelism = 1;       // Number of threads
        uint32_t hash_len = 32;         // Output hash length

        // Buffer for the final encoded string
        size_t encoded_len = argon2_encodedlen(
            t_cost,
            m_cost,
            parallelism,
            salt.size(),
            hash_len,
            Argon2_id
        );
        std::string encoded_hash(encoded_len, '\0');
        
        int result = argon2id_hash_encoded(
            t_cost, m_cost, parallelism,
            pwd.data(), pwd.size(),
            salt.data(), salt.size(),
            hash_len,
            encoded_hash.data(), encoded_len
        );

        if (result != ARGON2_OK)
            throw std::runtime_error("Argon2 hashing failed");

        // Remove the null-terminator appended by the C API
        encoded_hash.resize(encoded_len - 1);
        return encoded_hash;
    }

    StringMap parseJsonString(
        const std::shared_ptr<Json::Value>& jsonBody,
        const std::vector<std::string>& expectedKeys
    )
    {
        std::unordered_map<std::string, std::string> extractedData;

        for (const auto& key : expectedKeys)
            extractedData[key] = jsonBody->get(key, "").asString();

        return extractedData;
    }

    drogon::HttpResponsePtr validatePayload(
        const StringMap& parsedData,
        const FieldList& requiredFields
    )
    {
        std::vector<std::string> invalidFields;

        for (const auto& requiredKey : requiredFields)
        {
            auto it = parsedData.find(requiredKey);

            if (it == parsedData.end() || it->second.empty())
                invalidFields.push_back(requiredKey);
        }

        if (!invalidFields.empty())
        {
            Json::Value errorBody;
            errorBody["success"] = false;
            errorBody["error"] = "Missing or empty fields:";

            Json::Value fieldsArray(Json::arrayValue);
            for (const auto& field : invalidFields)
                fieldsArray.append(field);

            errorBody["missing_fields"] = fieldsArray;

            drogon::HttpResponsePtr resp = drogon::HttpResponse::newHttpJsonResponse(errorBody);
            resp->setStatusCode(k400BadRequest);
            return resp;
        }

        return nullptr;
    }
}