/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Windowing/Window.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace uxcpp::windowing {

class GLFWWindow : public Window {
public:
    GLFWWindow(const std::string& title, int width = 800, int height = 600)
        : Window(title, width, height) {
        
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!m_window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_window);
    }

    ~GLFWWindow() override {
        if (m_window) {
            glfwDestroyWindow(m_window);
        }
        glfwTerminate();
    }

    void show() override {
        glfwShowWindow(m_window);
    }

    void hide() override {
        glfwHideWindow(m_window);
    }

    bool shouldClose() const override {
        return glfwWindowShouldClose(m_window);
    }

    void pollEvents() override {
        glfwPollEvents();
    }

    void swapBuffers() override {
        glfwSwapBuffers(m_window);
    }

    GLFWwindow* getNativeHandle() const { return m_window; }

protected:
    void setNativeTitle(const std::string& title) override {
        glfwSetWindowTitle(m_window, title.c_str());
    }

    void setNativeSize(int width, int height) override {
        glfwSetWindowSize(m_window, width, height);
    }

    GLFWwindow* m_window;
};

} // namespace uxcpp::windowing
