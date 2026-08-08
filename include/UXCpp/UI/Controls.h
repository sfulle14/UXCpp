/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI\Widget.h>

namespace uxcpp::ui {

class Label : public Widget {
public:
    Label(std::string name, std::string text) 
        : Widget(std::move(name), "label"), m_text(std::move(text)) {}

    void setText(std::string text) { m_text = std::move(text); }

    void onDraw(graphics::Renderer& renderer) override {
        renderer.drawText({m_bounds.x, m_bounds.y}, m_text, getStyle().foregroundColor);
    }

private:
    std::string m_text;
};

class Button : public Widget {
public:
    Button(std::string name, std::string label) 
        : Widget(std::move(name), "button"), m_label(std::move(label)) {}

    void onDraw(graphics::Renderer& renderer) override {
        const auto& style = getStyle();
        renderer.drawRect(m_bounds, style.backgroundColor); 
        renderer.drawText({m_bounds.x + 5, m_bounds.y + 5}, m_label, style.foregroundColor);
    }

    bool onPointerDown(graphics::Point p) override {
        if (m_bounds.contains(p)) {
            onClicked.emit();
            return true;
        }
        return false;
    }

private:
    std::string m_label;
};

} // namespace uxcpp::ui
