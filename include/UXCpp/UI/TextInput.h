/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI\Widget.h>
#include <UXCpp/Core/InputManager.h>

namespace uxcpp::ui {

class TextInput : public Widget {
public:
    TextInput(std::string name, std::string placeholder = "") 
        : Widget(std::move(name), "textinput"), m_placeholder(std::move(placeholder)) {}

    void onDraw(graphics::Renderer& renderer) override {
        // Draw background
        renderer.drawRect(m_bounds, getStyle().backgroundColor);
        
        // Draw text or placeholder
        std::string display = m_text.empty() ? m_placeholder : m_text;
        graphics::Color color = m_text.empty() ? graphics::Color{0.7f, 0.7f, 0.7f, 1.0f} : getStyle().foregroundColor;
        
        renderer.drawText({m_bounds.x + 5, m_bounds.y + 5}, display, color);
    }

    bool onPointerDown(graphics::Point p) override {
        if (m_bounds.contains(p)) {
            core::InputManager::getInstance().setFocus(shared_from_this());
            return true;
        }
        return false;
    }

    bool onKeyDown(int key) override {
        // Simplified text input: only handles basic ASCII for now
        if (key >= 32 && key <= 126) {
            m_text += static_cast<char>(key);
            return true;
        } else if (key == 13) { // Enter
            m_text += '\n';
            return true;
        } else if (key == 8 && !m_text.empty()) { // Backspace
            m_text.pop_back();
            return true;
        }
        return false;
    }

    const std::string& getText() const { return m_text; }

private:
    std::string m_text;
    std::string m_placeholder;
};

} // namespace uxcpp::ui
