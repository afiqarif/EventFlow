#include <drogon/drogon.h>
#include <iostream>

int main() {
    std::cout << "Starting EventFlow Drogon Gateway on port 8080..." << std::endl;

    // Set up a simple endpoint to test that the gateway compiles and works
    drogon::app().registerHandler(
        "/health",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setBody("{\"status\": \"healthy\", \"service\": \"EventFlow API Gateway\"}");
            resp->setContentTypeCode(drogon::CT_APPLICATION_JSON);
            callback(resp);
        },
        {drogon::Get}
    );

    // Run the application
    drogon::app()
        .addListener("0.0.0.0", 8080)
        .run();

    return 0;
}