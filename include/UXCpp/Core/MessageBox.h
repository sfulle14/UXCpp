/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>

namespace uxcpp::core {

enum class MessageBoxType {
    Information,
    Warning,
    Error,
};

class MessageBox {
public:
    static void show(const std::string& title, const std::string& message, MessageBoxType type = MessageBoxType::Information);
};

} // namespace uxcpp::core