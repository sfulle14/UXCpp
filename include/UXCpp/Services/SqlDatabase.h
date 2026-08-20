/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <future>
#include <memory>
#include <optional>

namespace uxcpp::services {

struct SqlResult {
    bool success;
    std::string errorMessage;
    std::vector<std::vector<std::string>> rows; // Simple string-based result set
};

class SqlDatabase {
public:
    virtual ~SqlDatabase() = default;

    /**
     * @brief Opens a connection to the database.
     */
    virtual bool open(const std::string& connectionString) = 0;

    /**
     * @brief Closes the current connection.
     */
    virtual void close() = 0;

    /**
     * @brief Executes a query that returns data (SELECT).
     */
    virtual std::future<SqlResult> executeQuery(const std::string& sql) = 0;

    /**
     * @brief Executes a command that modifies data (INSERT, UPDATE, DELETE).
     */
    virtual std::future<int> executeCommand(const std::string& sql) = 0;
};

class SqlDatabaseImpl : public SqlDatabase {
public:
    bool open(const std::string& connectionString) override;
    void close() override;
    std::future<SqlResult> executeQuery(const std::string& sql) override;
    std::future<int> executeCommand(const std::string& sql) override;

private:
    bool m_isOpen = false;
};

} // namespace uxcpp::services