#include "utils.h"

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

    std::string hash(std::string password)
    {
        std::string hashed = password;
        // TODO: password hashing

        return hashed;
    }
}