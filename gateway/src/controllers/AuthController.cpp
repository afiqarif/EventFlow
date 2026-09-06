#include "AuthController.h"
#include "../utils/utils.h"
#include <json/json.h>

drogon::Task<drogon::HttpResponsePtr> AuthController::loginUser(drogon::HttpRequestPtr req)
{
    // Required payload fields
    utils::FieldList requiredFields = {
        "email",
        "password"
    };

    // Database Client pointer declaration
    utils::DatabaseClient DbClientPtr = drogon::app().getDbClient("default");
    
    // 1. Extract JSON body
    std::shared_ptr<Json::Value> jsonBody = req->getJsonObject();

    // Check if JSON body is empty
    if (!jsonBody)
        // Replaced callback + return with a direct co_return
        co_return utils::errorRequest("Invalid or empty JSON body", drogon::k400BadRequest);

    // Parse payload
    utils::StringMap parsedData = utils::parseJsonString(jsonBody, requiredFields);

    // Check for missing fields or empty fields
    if (drogon::HttpResponsePtr errorResp = utils::validatePayload(parsedData, requiredFields))
        co_return errorResp;

    // Password Hash
    std::string hashedPwd = utils::hash(parsedData["password"]);

    // 2. Asynchronous Database Query using co_await
    drogon::orm::Result result = co_await DbClientPtr->execSqlCoro(
        "SELECT password_hash FROM users WHERE email = $1",
        parsedData["email"]
    );
    // Check if any row was found
    if (result.empty())
        co_return utils::errorRequest("Invalid email or password", drogon::k400BadRequest);

    // Extract values from the first row found
    const drogon::orm::Row row = result[0];

    std::string dbPasswordHash = row["password_hash"].as<std::string>();
    std::string dbUserEmail = row["email"].as<std::string>();

    if (hashedPwd != dbPasswordHash)
        co_return utils::errorRequest("Wrong credentials", drogon::k401Unauthorized);

    // TODO: Send JWT to client
    std::string token = "mvjFSDKjfs13564mklKJSFS432JKFJrfyreu";

    Json::Value responseRet;
    responseRet["success"] = true;
    responseRet["token"] = hashedPwd;

    drogon::HttpResponsePtr resp = drogon::HttpResponse::newHttpJsonResponse(responseRet);
    resp->setStatusCode(drogon::k200OK);
    
    co_return resp;
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

    // Database Client pointer declaration
    utils::DatabaseClient DbClientPtr = drogon::app().getDbClient("default");

    // Parse JSON body
    std::shared_ptr<Json::Value> jsonBody = req->getJsonObject();

    // Check if JSON body is empty
    if (!jsonBody)
    {
        callback(utils::errorRequest("Invalid or empty JSON body", drogon::k400BadRequest));
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