#include "UploadController.h"
#include "../utils/utils.h"
#include <json/json.h> // Drogon uses jsoncpp natively for JSON handling

void UploadController::getUploadUrl(
    const drogon::HttpRequestPtr& req,
    std::function<void (const drogon::HttpResponsePtr &)> && callback
)
{
    // 1. Extract the JSON body safely from the incoming client request.
    std::shared_ptr<Json::Value> jsonBody = req->getJsonObject();

    if (!jsonBody)
    {
        drogon::HttpResponsePtr errorResp = utils::makeBadRequest(
            "Invalid or empty JSON body"
        );
        callback(errorResp);
        return;
    }

    // 2. Parse strictly-typed variables out of the JSON tree with safety fallbacks
    std::string eventId = jsonBody->get("eventId", "").asString();
    std::string filename = jsonBody->get("filename", "").asString();

    if (eventId.empty() || filename.empty())
    {
        drogon::HttpResponsePtr errorResp = utils::makeBadRequest(
            "Missing required field"
        );
        callback(errorResp);
        return;
    }

    // 3. Run execution logic (e.g., generate GCS signed URL using GcsSigner utility).
    // Note: This matches the "bypass upload" architecture mapped in the proposal.
    // TODO: Change to proper GCS signer.
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
    drogon::HttpResponsePtr resp = drogon::HttpResponse::newHttpJsonResponse(responseRet);
    resp->setStatusCode(drogon::k200OK);

    // Drogon transfers ownership of the response to the underlying socket channel.
    callback(resp);
}