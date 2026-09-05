#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <drogon/drogon.h>

class AuthController : public drogon::HttpController<AuthController>
{
    public:
        METHOD_LIST_BEGIN

        ADD_METHOD_TO(AuthController::authenticate, "/authenticate", drogon::Post);

        METHOD_LIST_END

        void authenticate(
            const drogon::HttpRequestPtr& req,
            std::function<void (const drogon::HttpResponsePtr &)> && callback
        );
};

#endif