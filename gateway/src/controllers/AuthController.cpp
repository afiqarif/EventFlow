#include "AuthController.h"
#include "../utils/utils.h"
#include <json/json.h>

void AuthController::authenticate(
    const drogon::HttpRequestPtr& req,
    std::function<void (const drogon::HttpResponsePtr &)> && callback
)
{
    // 1. Extract JSON body
    std::shared_ptr<Json::Value> jsonBody = req->getJsonObject();

    if (!jsonBody)
    {
        drogon::HttpResponsePtr errorResp = utils::makeBadRequest(
            "Invalid or empty JSON body"
        );
        callback(errorResp);
        return;
    }

    std::string email = jsonBody->get("email", "").asString();
    std::string password = jsonBody->get("password", "").asString();

    std::string hashedPassword = utils::hash(password);

    // TODO: Check with database

    // TODO: Send JWT to client
    std::string token = "mvjFSDKjfs13564mklKJSFS432JKFJrfyreu";

    Json::Value responseRet;
    responseRet["success"] = true;
    responseRet["token"] = token;

    drogon::HttpResponsePtr resp = drogon::HttpResponse::newHttpJsonResponse(responseRet);
    resp->setStatusCode(drogon::k200OK);
    callback(resp);
}