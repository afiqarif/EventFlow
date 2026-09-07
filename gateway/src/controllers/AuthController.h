#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <drogon/drogon.h>
#include <drogon/utils/coroutine.h>
#include <drogon/orm/DbClient.h>

class AuthController : public drogon::HttpController<AuthController>
{
    public:
        METHOD_LIST_BEGIN

        ADD_METHOD_TO(AuthController::loginUser, "/auth/login", drogon::Post);
        ADD_METHOD_TO(AuthController::registerUser, "/auth/register", drogon::Post);

        METHOD_LIST_END

        drogon::Task<drogon::HttpResponsePtr> loginUser(drogon::HttpRequestPtr req);

        drogon::Task<drogon::HttpResponsePtr> registerUser(drogon::HttpRequestPtr req);
};

#endif