#include "AuthController.h"
#include "../utils/utils.h"
#include <json/json.h>
#include <argon2.h>

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

    // 2. Asynchronous Database Query using co_await
    drogon::orm::Result result = co_await DbClientPtr->execSqlCoro(
        "SELECT id, password_hash FROM users WHERE email = $1",
        parsedData["email"]
    );
    // Check if any row was found
    if (result.empty())
        co_return utils::errorRequest("Invalid email or password", drogon::k400BadRequest);

    // Extract values from the first row found
    const drogon::orm::Row row = result[0];

    std::string dbPasswordHash = row["password_hash"].as<std::string>();
    std::string dbUserId = row["id"].as<std::string>();

    int pwdResult = argon2id_verify(dbPasswordHash.c_str(), parsedData["password"].c_str(), parsedData["password"].length());

    if (pwdResult != ARGON2_OK)
        co_return utils::errorRequest("Wrong credentials", drogon::k401Unauthorized);

    // TODO: Send JWT to client
    std::string token = utils::generateToken(dbUserId, "user", std::getenv("JWT_SECRET"));

    Json::Value responseRet;
    responseRet["success"] = true;
    responseRet["token"] = token;

    drogon::HttpResponsePtr resp = drogon::HttpResponse::newHttpJsonResponse(responseRet);
    resp->setStatusCode(drogon::k200OK);
    
    co_return resp;
}

drogon::Task<drogon::HttpResponsePtr> AuthController::registerUser(drogon::HttpRequestPtr req) {
    // Required payload fields
    utils::FieldList requiredFields = {
        "fName", "lName",
        "username",
        "email",
        "password"
    };

    // Database Client point declaration
    utils::DatabaseClient DbClientPtr = drogon::app().getDbClient("default");

    // Parse JSON body
    std::shared_ptr<Json::Value> jsonBody = req->getJsonObject();

    // check if JSON body is empty
    if (!jsonBody)
        co_return utils::errorRequest("Invalid or empty JSON body", drogon::k400BadRequest);

    // Parse fields
    utils::StringMap parsedData = utils::parseJsonString(jsonBody, requiredFields);

    // Check if fields are missing or empty
    if (drogon::HttpResponsePtr errorResp = utils::validatePayload(parsedData, requiredFields))
        co_return errorResp;

    // Hash password
    std::string hashedPwd = utils::hash(parsedData["password"]);

    // Asynchronous database query
    drogon::orm::Result result = co_await DbClientPtr->execSqlCoro(
        "INSERT INTO users (first_name, last_name, username, email, password_hash) VALUES ($1, $2, $3, $4, $5) RETURNING *",
        parsedData["fName"], parsedData["lName"],
        parsedData["username"],
        parsedData["email"],
        hashedPwd
    );

    std::string newUserId = result[0]["id"].as<std::string>();

    // TODO: JWT generation
    std::string token = "jkandkjasnrkjehrkjana";

    Json::Value responseRet;
    responseRet["success"] = true;
    responseRet["token"] = token;
    responseRet["userId"] = newUserId;

    drogon::HttpResponsePtr resp = drogon::HttpResponse::newHttpJsonResponse(responseRet);
    resp->setStatusCode(drogon::k200OK);

    co_return resp;
}