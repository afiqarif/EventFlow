#ifndef UPLOADCONTROLLER_H
#define UPLOADCONTROLLER_H

#include <drogon/HttpController.h>

// 1. Inherit from the HttpController using CRTP (Curiously Recurring Template Pattern).
class UploadController : public drogon::HttpController<UploadController>
{
    public:
        // 2. Define the routing methodology using Drogon's macro list.
        METHOD_LIST_BEGIN

        // Maps a HTTP POST to "/upload/request-url" directly to the getUploadUrl() method.
        ADD_METHOD_TO(UploadController::getUploadUrl, "/upload/request-url", drogon::Post);

        METHOD_LIST_END

        // 3. Declare the request handler. It receives the HTTP request smart pointer
        // and an asynchronous callback function to trigger when the task is complete.
        void getUploadUrl(
            const drogon::HttpRequestPtr& req,
            std::function<void (const drogon::HttpResponsePtr &)> && callback
        );
};

#endif