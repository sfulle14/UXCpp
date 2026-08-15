/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace uxcpp::ui {

/**
 * @brief A toolbar widget that holds a collection of buttons for quick actions.
 */
class Toolbar : public Widget {
public:
    explicit Toolbar(std::string name) 
        : Widget(std::move(name), "toolbar"), m_buttonWidth(32.0f) {}

    void addButton(const std::string& iconId, std::function<void()> action) {
        m_buttons.push_back({iconId, action});
    }

    void onDraw(graphics::Renderer& renderer) override {
        renderer.drawRect(m_bounds, graphics::Color{0.9f, 0.9f, 0.9f, 1.0f});

        float currentX = m_bounds.x + 5.0f;
        for (const auto& btn : m_buttons) {
            graphics::Rect btnRect{currentX, m_bounds.y + 2, m_buttonWidth, m_bounds.height - 4};
            renderer.drawRect(btnRect, graphics::Color{0.8f, 0.8f, 0.8f, 1.0f});
            // In a real system, we'd draw the icon from ResourceManager here
            renderer.drawText({btnRect.x + 4, btnRect.y + 4}, "[I]", graphics::Color::Black());
            currentX += m_buttonWidth + 5.0f;
        }
    }

    bool onPointerDown(graphics::Point p) override {
        float currentX = m_bounds.x + 5.0f;
        for (const auto& btn : m_buttons) {
            graphics::Rect btnRect{currentX, m_bounds.y + 2, m_buttonWidth, m_bounds.height - 4};
            if (btnRect.contains(p)) {
                if (btn.action) btn.action();
                return true;
            }
            currentX += m_buttonWidth + 5.0f;
        }
        return false;
    }

private:
    struct ButtonInfo {
        std::string iconId;
        std::function<void()> action;
    };

    std::vector<ButtonInfo> m_buttons;
    float m_buttonWidth;
};

} // namespace uxcpp::ui
