#include "UploadController.h"
#include <json/json.h> // Drogon uses jsoncpp natively for JSON handling

using namespace drogon;

void UploadController::getUploadUrl(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> && callback
)
{
    // 1. Extract the JSON body safely from the incoming client request.
    auto jsonBody = req->getJsonObject();

    if (!jsonBody)
    {
        // Fail-safe validation: return a 400 Bad Request if client payload is empty.
        Json::Value errorRet;
        errorRet["success"] = false;
        errorRet["error"] = "Invalid or empty JSON body";

        auto resp = HttpResponse::newHttpJsonResponse(errorRet);
        resp->setStatusCode(k400BadRequest);
        callback(resp);

        return;
    }

    // 2. Parse strictly-typed variables out of the JSON tree with safety fallbacks
    std::string eventId = jsonBody->get("eventId", "").asString();
    std::string filename = jsonBody->get("filename", "").asString();

    if (eventId.empty() || filename.empty())
    {
        Json::Value errorRet;
        errorRet["success"] = false;
        errorRet["error"] = "Missing required fields: eventId or filename";

        auto resp = HttpResponse::newHttpJsonResponse(errorRet);
        resp->setStatusCode(k400BadRequest);
        callback(resp);

        return;
    }

    // 3. Run execution logic (e.g., generate GCS signed URL using GcsSigner utility).
    // Note: This matches the "bypass upload" architecture mapped in the proposal.
    std::string preSignedUrl = 
        "https://storage.googleapis.com/eventflow-ingestion/" +
        eventId + "/" +
        filename + 
        "?GoogleAccessId=...";

    // 4. Formulate the response JSON payload
    Json::Value responseRet;
    responseRet["success"] = true;
    responseRet["uploadUrl"] = preSignedUrl;

    // 5. Build the HTTP response object and send it back to the event-loop thread
    auto resp = HttpResponse::newHttpJsonResponse(responseRet);
    resp->setStatusCode(k200OK);

    // Drogon transfers ownership of the response to the underlying socket channel.
    callback(resp);
}