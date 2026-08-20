/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Services/HttpClient.h>
#include <future>

namespace uxcpp::services {

std::future<HttpResponse> HttpClientImpl::get(const std::string& url) {
    return std::async(std::launch::async, [url]() {
        return HttpResponse{200, "{\"status\": \"success\", \"message\": \"Mock GET response from " + url + "\"}", {{"Content-Type", "application/json"}}};
    });
}

std::future<HttpResponse> HttpClientImpl::post(const std::string& url, const std::string& body, 
                                              const std::string& contentType) {
    return std::async(std::launch::async, [url, body]() {
        return HttpResponse{201, "{\"status\": \"created\", \"body\": \"" + body + "\"}", {{"Content-Type", "application/json"}}};
    });
}

void HttpClientImpl::setTimeout(int seconds) {
    m_timeout = seconds;
}

} // namespace uxcpp::services