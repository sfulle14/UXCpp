/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Core/Clipboard.h>
#include <GLFW/glfw3.h>

namespace uxcpp::core {

void Clipboard::setText(GLFWwindow* window, const std::string& text) {
    if (window) {
        glfwSetClipboardString(window, text.c_str());
    }
}

std::string Clipboard::getText(GLFWwindow* window) {
    if (!window) {
        return {};
    }

    const char* clipboard = glfwGetClipboardString(window);
    return clipboard ? clipboard : std::string{};
}

} // namespace uxcpp::core