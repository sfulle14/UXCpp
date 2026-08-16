/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <map>

namespace uxcpp::core {

/**
 * @brief A generic service module for async operations.
 */
class AsyncService {
public:
    virtual ~AsyncService() = default;
    virtual void execute(std::function<void()> task) = 0;
};

/**
 * @brief Simple HTTP Client abstraction.
 */
class HttpClient {
public:
    static HttpClient& getInstance() {
        static HttpClient instance;
        return instance;
    }

    void get(const std::string& url, std::function<void(std::string)> callback) {
        // Mock implementation
        callback("HTTP/1.1 200 OK\r\n\r\nMock Response from " + url);
    }

private:
    HttpClient() = default;
};

/**
 * @brief Simple SQL Database abstraction.
 */
class SqlDatabase {
public:
    static SqlDatabase& getInstance() {
        static SqlDatabase instance;
        return instance;
    }

    bool open(const std::string& connectionString) { return true; }
    void execute(const std::string& query) { /* Mock */ }
};

} // namespace uxcpp::core
