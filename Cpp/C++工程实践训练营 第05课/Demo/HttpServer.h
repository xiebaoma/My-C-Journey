#pragma once
#include <functional>
#include <string>

class HttpRequest {

};

class HttpResponse {

};

using Handler = std::function<void (const HttpRequest& req, HttpResponse& resp)>;

//typedef void (*Handler)(const HttpRequest* req, HttpResponse* resp);

class HttpServer final {
public:
    HttpServer() = default;
    ~HttpServer() = default;

    HttpServer(const HttpServer& rhs) = delete;
    HttpServer& operator=(const HttpServer& rhs) = delete;

    bool registerPath(const std::string& path, Handler&& handler) {
        m_handler = std::move(handler);
    }
    //bool registerPath2(const std::string& path, std::function<void(const HttpRequest& req, HttpResponse& resp) handler);

private:
    Handler m_handler;
};
