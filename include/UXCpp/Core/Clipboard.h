/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>

struct GLFWwindow;

namespace uxcpp::core {

class Clipboard {
public:
    static void setText(GLFWwindow* window, const std::string& text);
    static std::string getText(GLFWwindow* window);
};

} // namespace uxcpp::core
