/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Core/Application.h>
#include <UXCpp/Core/Clipboard.h>
#include <UXCpp/Core/FileDialog.h>
#include <UXCpp/Core/SystemTray.h>
#include <UXCpp/Core/MessageBox.h>
#include <UXCpp/Core/WindowStatePersistence.h>
#include <UXCpp/Windowing/GLFWWindow.h>
#include <UXCpp/Graphics/OpenGLRenderer.h>
#include <UXCpp/Core/InputManager.h>
#include <UXCpp/Core/Animator.h>
#include <iostream>
#include <memory>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#endif

namespace uxcpp::core {

static std::unique_ptr<windowing::Window> g_mainWindow;
static std::unique_ptr<graphics::Renderer> g_renderer;

static GLFWwindow* getNativeWindow() {
    auto* glfwWindow = dynamic_cast<windowing::GLFWWindow*>(g_mainWindow.get());
    return glfwWindow ? glfwWindow->getNativeHandle() : nullptr;
}

// GLFW Callbacks
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        InputManager::getInstance().handleMouseDown((float)xpos, (float)ypos);
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        InputManager::getInstance().handleMouseUp((float)xpos, (float)ypos);
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    (void)window;
    InputManager::getInstance().handleMouseMove((float)xpos, (float)ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        InputManager::getInstance().handleKeyDown(
            key,
            (mods & GLFW_MOD_CONTROL) != 0,
            (mods & GLFW_MOD_SHIFT) != 0,
            (mods & GLFW_MOD_ALT) != 0
        );
    }
}

// IME Callback (Conceptual - would be registered with OS)
void ime_composition_callback(const char* text, int caretPos) {
    core::IMEBridge::getInstance().onCompositionUpdate(text, caretPos);
}

void ime_commit_callback(const char* text) {
    core::IMEBridge::getInstance().onCandidateSelected(text);
}

bool Application::init() {
    std::cout << "[UXCpp] Initializing Application..." << std::endl;
    try {
        // Load window state if available
        int x = 100, y = 100, w = 1280, h = 720;
        Win32WindowStatePersistence persistence;
        persistence.loadState("MainWindow", x, y, w, h);

        g_mainWindow = std::make_unique<windowing::GLFWWindow>("UXCpp Window", w, h);
        g_renderer = std::make_unique<graphics::OpenGLRenderer>();

        // Set GLFW callbacks
        GLFWwindow* nativeWin = static_cast<windowing::GLFWWindow*>(g_mainWindow.get())->getNativeHandle();
        glfwSetMouseButtonCallback(nativeWin, mouse_button_callback);
        glfwSetCursorPosCallback(nativeWin, cursor_position_callback);
        glfwSetKeyCallback(nativeWin, key_callback);

        m_running = true;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[UXCpp] Initialization failed: " << e.what() << std::endl;
        return false;
    }
}

void Application::run() {
    std::cout << "[UXCpp] Entering Main Loop..." << std::endl;

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (m_running && g_mainWindow && !g_mainWindow->shouldClose()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        g_mainWindow->pollEvents();

        Animator::getInstance().update(deltaTime);

        g_renderer->beginFrame();
        g_renderer->clear(graphics::Color::White());

        for (auto& overlay : m_overlays) {
            overlay->onDraw(*g_renderer);
        }

        g_renderer->endFrame();
        g_mainWindow->swapBuffers();
    }

    quit();
}

void Application::quit() {
    std::cout << "[UXCpp] Quit requested." << std::endl;
    m_running = false;
}

void Application::setClipboardText(const std::string& text) {
    auto* nativeWindow = getNativeWindow();
    if (nativeWindow) {
        Clipboard::setText(nativeWindow, text);
    }
}

std::string Application::getClipboardText() const {
    auto* nativeWindow = getNativeWindow();
    if (nativeWindow) {
        return Clipboard::getText(nativeWindow);
    }
    return {};
}

std::string Application::openFileDialog(const std::string& title, const std::vector<std::string>& filters) {
    return FileDialog::openFile(title, filters);
}

std::string Application::saveFileDialog(const std::string& title, const std::vector<std::string>& filters) {
    return FileDialog::saveFile(title, filters);
}

std::shared_ptr<uxcpp::core::SystemTrayIcon> Application::createSystemTrayIcon(const std::string& tooltip) {
    return std::make_shared<uxcpp::core::SystemTrayIcon>(uxcpp::core::SystemTrayIcon::Config{tooltip});
}

void Application::showMessageBox(const std::string& title, const std::string& message, MessageBoxType type) {
    MessageBox::show(title, message, type);
}

} // namespace uxcpp::core
