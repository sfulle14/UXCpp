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
    std::future<HttpResponse> get(const std::string& url) override;
    std::future<HttpResponse> post(const std::string& url, const std::string& body, 
                                  const std::string& contentType = "application/json") override;
    void setTimeout(int seconds) override;

private:
    int m_timeout = 30;
};

} // namespace uxcpp::services