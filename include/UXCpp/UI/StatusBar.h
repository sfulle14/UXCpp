/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI/Widget.h>
#include <UXCpp/Graphics/Renderer.h>
#include <string>
#include <memory>
#include <vector>

namespace uxcpp::ui {

/**
 * @brief A specialized widget for displaying a status bar at the bottom of a window.
 */
class StatusBar : public Widget {
public:
    explicit StatusBar(std::string name) 
        : Widget(std::move(name), "statusbar") {}

    void setMessage(const std::string& message) {
        m_message = message;
    }

    void onDraw(graphics::Renderer& renderer) override {
        renderer.drawRect(m_bounds, graphics::Color{0.95f, 0.95f, 0.95f, 1.0f});
        renderer.drawText({m_bounds.x + 10, m_bounds.y + 5}, m_message, graphics::Color::Black());
    }

private:
    std::string m_message;
};

} // namespace uxcpp::ui
