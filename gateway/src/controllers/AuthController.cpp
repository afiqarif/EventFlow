#include "AuthController.h"
#include "../utils/utils.h"
#include <json/json.h>

void AuthController::loginUser(
    const drogon::HttpRequestPtr& req,
    std::function<void (const drogon::HttpResponsePtr &)> && callback
)
{
    // Required payload fields
    std::vector<std::string> requiredFields = {
        "email",
        "password"
    };

    // Database Client pointer declaration
    std::shared_ptr<drogon::orm::DbClient> DbClientPtr = drogon::app().getDbClient("default");
    
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

    utils::StringMap parsedData = utils::parseJsonString(jsonBody, requiredFields);

    if (parsedData["email"].empty() || parsedData["password"].empty())
    {
        drogon::HttpResponsePtr resp = utils::makeBadRequest(
            "Missing required fields"
        );
        callback(resp);
        return;
    }

    std::string hashedPwd = utils::hash(parsedData["password"]);

    // TODO: Check with database

    // TODO: Send JWT to client
    std::string token = "mvjFSDKjfs13564mklKJSFS432JKFJrfyreu";

    Json::Value responseRet;
    responseRet["success"] = true;
    responseRet["token"] = hashedPwd;

    drogon::HttpResponsePtr resp = drogon::HttpResponse::newHttpJsonResponse(responseRet);
    resp->setStatusCode(drogon::k200OK);
    callback(resp);
}

void AuthController::registerUser(
    const drogon::HttpRequestPtr& req,
    std::function<void (const drogon::HttpResponsePtr &)> && callback
)
{
    std::shared_ptr<Json::Value> jsonBody = req->getJsonObject();

    if (!jsonBody)
    {
        drogon::HttpResponsePtr errorResp = utils::makeBadRequest(
            "Invalid or empty JSON body"
        );
        callback(errorResp);
        return;
    }
}