/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <iostream>

namespace uxcpp::windowing {

/**
 * @brief Abstract base class for a Window.
 * 
 * This provides the interface for window management regardless of the backend (GLFW, Win32, etc).
 */
class Window {
public:
    struct Size {
        int width;
        int height;
    };

    explicit Window(const std::string& title, int width = 800, int height = 600)
        : m_title(title), m_width(width), m_height(height) {}

    virtual ~Window() = default;

    // Non-virtual interface (NVI) pattern for common window operations
    void setTitle(const std::string& title) {
        m_title = title;
        setNativeTitle(title);
    }

    void setSize(int width, int height) {
        m_width = width;
        m_height = height;
        setNativeSize(width, height);
    }

    virtual void show() = 0;
    virtual void hide() = 0;
    virtual bool shouldClose() const = 0;
    virtual void pollEvents() = 0;
    virtual void swapBuffers() = 0;

    // Window State Management
    virtual void setPosition(int x, int y) = 0;
    virtual void getPosition(int& x, int& y) const = 0;
    virtual void setMinimized(bool minimized) = 0;
    virtual bool isMinimized() const = 0;
    virtual void setMaximized(bool maximized) = 0;
    virtual bool isMaximized() const = 0;

    // Getters
    const std::string& getTitle() const { return m_title; }
    Size getSize() const { return {m_width, m_height}; }

protected:
    virtual void setNativeTitle(const std::string& title) = 0;
    virtual void setNativeSize(int width, int height) = 0;

    std::string m_title;
    int m_width;
    int m_height;
};

} // namespace uxcpp::windowing
