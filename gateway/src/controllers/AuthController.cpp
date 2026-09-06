#include "AuthController.h"
#include "../utils/utils.h"
#include <json/json.h>

void AuthController::loginUser(
    const drogon::HttpRequestPtr& req,
    std::function<void (const drogon::HttpResponsePtr &)> && callback
)
{
    // Required payload fields
    utils::FieldList requiredFields = {
        "email",
        "password"
    };

    // Database Client pointer declaration
    std::shared_ptr<drogon::orm::DbClient> DbClientPtr = drogon::app().getDbClient("default");
    
    // 1. Extract JSON body
    std::shared_ptr<Json::Value> jsonBody = req->getJsonObject();

    // Check if JSON body is empty
    if (!jsonBody)
    {
        drogon::HttpResponsePtr errorResp = utils::makeBadRequest(
            "Invalid or empty JSON body"
        );
        callback(errorResp);
        return;
    }

    // Parse payload
    utils::StringMap parsedData = utils::parseJsonString(jsonBody, requiredFields);

    // Check for missing fields or empty fields
    if (drogon::HttpResponsePtr errorResp = utils::validatePayload(parsedData, requiredFields))
    {
        callback(errorResp);
        return;
    }

    // Password Hash
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
    // Required payload fields
    utils::FieldList requiredFields = {
        "fName", "lName",
        "username",
        "email",
        "password"
    };

    // Parse JSON body
    std::shared_ptr<Json::Value> jsonBody = req->getJsonObject();

    // Check if JSON body is empty
    if (!jsonBody)
    {
        drogon::HttpResponsePtr errorResp = utils::makeBadRequest(
            "Invalid or empty JSON body"
        );
        callback(errorResp);
        return;
    }

    // Parse fields
    utils::StringMap parsedData = utils::parseJsonString(jsonBody, requiredFields);

    // Check if fields are missing or empty
    if (drogon::HttpResponsePtr errorResp = utils::validatePayload(parsedData, requiredFields))
    {
        callback(errorResp);
        return;
    }

    std::string hashedPwd = utils::hash(parsedData["password"]);
}