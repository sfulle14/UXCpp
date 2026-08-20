/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Services/SqlDatabase.h>
#include <future>

namespace uxcpp::services {

bool SqlDatabaseImpl::open(const std::string& connectionString) {
    m_isOpen = true;
    return true;
}

void SqlDatabaseImpl::close() {
    m_isOpen = false;
}

std::future<SqlResult> SqlDatabaseImpl::executeQuery(const std::string& sql) {
    return std::async(std::launch::async, [this, sql]() {
        if (!m_isOpen) return SqlResult{false, "Database not open", {}};
        return SqlResult{true, "", {{"ID", "Name"}, {"1", "UXCpp User"}}};
    });
}

std::future<int> SqlDatabaseImpl::executeCommand(const std::string& sql) {
    return std::async(std::launch::async, [this, sql]() {
        if (!m_isOpen) return -1;
        return 1;
    });
}

} // namespace uxcpp::services