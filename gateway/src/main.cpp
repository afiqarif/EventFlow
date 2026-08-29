#include <drogon/drogon.h>
#include <iostream>
#include <exception> // Added for std::exception

int main() {
    std::cout << "Starting EventFlow Drogon Gateway on port 8080..." << std::endl;

    try
    {
        // 1. Load Drogon's standard configuration file.
        // This sets ports (e.g. 8080), thread pools, and DB connections.
        drogon::app().loadConfigFile("config.json");
    }
    catch (const std::exception& e)
    {
        std::cerr << "Configuration error: " << e.what() << std::endl;
        return 1;
    }

    // 2. Kick off the asynchronous, non-blocking HTTP and WebSocket runtime.
    // This loop will take over the thread pool and listen for client requests.
    drogon::app().run();

    return 0;
}