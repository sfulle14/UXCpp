/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI\Widget.h>
#include <UXCpp/Core/Application.h>
#include <string>
#include <memory>

namespace uxcpp::ui {

class Dialog : public Widget {
public:
    Dialog(std::string name, std::string title) 
        : Widget(std::move(name), "dialog"), m_title(std::move(title)) {}

    void setContent(std::shared_ptr<Widget> content) {
        m_content = content;
        addChild(content);
    }

    void show() {
        core::Application::getInstance().addOverlayWidget(shared_from_this());
        m_isVisible = true;
    }

    void close() {
        core::Application::getInstance().removeOverlayWidget(shared_from_this());
        m_isVisible = false;
    }

    void onDraw(graphics::Renderer& renderer) override {
        // Draw backdrop (dimming the background)
        renderer->drawRect({0, 0, 2000, 2000}, graphics::Color{0, 0, 0, 0.5f});

        // Draw dialog window
        renderer->drawRect(m_bounds, graphics::Color::White());
        
        // Draw title bar
        renderer->drawRect({m_bounds.x, m_bounds.y, m_bounds.width, 30}, graphics::Color::Blue());
        renderer->drawText({m_bounds.x + 10, m_bounds.y + 5}, m_title, graphics::Color::White());

        // Draw content
        if (m_content) {
            m_content->onDraw(renderer);
        }
    }

    bool onPointerDown(graphics::Point p) override {
        // If click is outside the dialog bounds, it's handled by the backdrop
        if (!m_bounds.contains(p)) {
            // Optional: close on background click
            return true; 
        }
        return Widget::onPointerDown(p);
    }

private:
    std::string m_title;
    std::shared_ptr<Widget> m_content;
    bool m_isVisible = false;
};

} // namespace uxcpp::ui
