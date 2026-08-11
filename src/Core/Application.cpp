/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Core/Application.h>
#include <UXCpp/Windowing/GLFWWindow.h>
#include <UXCpp/Graphics/OpenGLRenderer.h>
#include <UXCpp/Core/InputManager.h>
#include <UXCpp/Core/Animator.h>
#include <iostream>
#include <memory>
#include <chrono>

namespace uxcpp::core {

static std::unique_ptr<windowing::Window> g_mainWindow;
static std::unique_ptr<graphics::Renderer> g_renderer;

// GLFW Callbacks
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        InputManager::getInstance().handleMouseDown((float)xpos, (float)ypos);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        InputManager::getInstance().handleKeyDown(key);
    }
}

bool Application::init() {
    std::cout << "[UXCpp] Initializing Application..." << std::endl;
    try {
        g_mainWindow = std::make_unique<windowing::GLFWWindow>("UXCpp Window", 1280, 720);
        g_renderer = std::make_unique<graphics::OpenGLRenderer>();

        // Set GLFW callbacks
        GLFWwindow* nativeWin = static_cast<windowing::GLFWWindow*>(g_mainWindow.get())->getNativeHandle();
        glfwSetMouseButtonCallback(nativeWin, mouse_button_callback);
        glfwSetKeyCallback(nativeWin, key_callback);

        m_running = true;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[UXCpp] Initialization failed: " << e.what() << std::endl;
        return false;
    }
}

void Application::run() {
// ...existing code...
    std::cout << "[UXCpp] Entering Main Loop..." << std::endl;
    
    while (m_running && g_mainWindow && !g_mainWindow->shouldClose()) {
        g_mainWindow->pollEvents();
        
        g_renderer->beginFrame();
        // Here we would normally traverse the UI tree and call onDraw() for widgets
        g_renderer->clear(graphics::Color::White());
        g_renderer->endFrame();
        
        g_mainWindow->swapBuffers();
    }
    
    quit();
}

void Application::quit() {
    std::cout << "[UXCpp] Quit requested." << std::endl;
    m_running = false;
}

} // namespace uxcpp::core
