/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <future>
#include <functional>
#include <memory>

namespace uxcpp::services {

struct HttpResponse {
    int statusCode;
    std::string body;
    std::vector<std::pair<std::string, std::string>> headers;
};

class HttpClient {
public:
    virtual ~HttpClient() = default;

    /**
     * @brief Performs an asynchronous GET request.
     */
    virtual std::future<HttpResponse> get(const std::string& url) = 0;

    /**
     * @brief Performs an asynchronous POST request.
     */
    virtual std::future<HttpResponse> post(const std::string& url, const std::string& body, 
                                          const std::string& contentType = "application/json") = 0;

    /**
     * @brief Sets a global timeout for all requests in seconds.
     */
    virtual void setTimeout(int seconds) = 0;
};

class HttpClientImpl : public HttpClient {
public:
    std::future<HttpResponse> get(const std::string& url) override {
        return std::async(std::launch::async, [url]() {
            // Mock implementation for clean-room architecture.
            // In a real production environment, this would wrap libcurl or WinHTTP.
            return HttpResponse{200, "{\"status\": \"success\", \"message\": \"Mock GET response from " + url + "\"}", {{"Content-Type", "application/json"}}};
        });
    }

    std::future<HttpResponse> post(const std::string& url, const std::string& body, 
                                  const std::string& contentType) override {
        return std::async(std::launch::async, [url, body]() {
            // Mock implementation.
            return HttpResponse{201, "{\"status\": \"created\", \"body\": \"" + body + "\"}", {{"Content-Type", "application/json"}}};
        });
    }

    void setTimeout(int seconds) override {
        m_timeout = seconds;
    }

private:
    int m_timeout = 30;
};

} // namespace uxcpp::services