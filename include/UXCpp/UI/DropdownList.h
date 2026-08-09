/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI\Widget.h>
#include <UXCpp/UI\Controls.h>
#include <UXCpp/Core/Application.h>
#include <vector>
#include <string>

namespace uxcpp::ui {

class DropdownList : public Widget {
public:
    DropdownList(std::string name, const std::vector<std::string>& items) 
        : Widget(std::move(name), "dropdown_list"), m_items(items) {}

    void setPosition(float x, float y, float w, float h) {
        setBounds(x, y, w, h);
    }

    void onDraw(graphics::Renderer& renderer) override {
        renderer.drawRect(m_bounds, graphics::Color::White());
        
        float itemHeight = 25.0f;
        for (size_t i = 0; i < m_items.size(); ++i) {
            graphics::Rect itemRect = {m_bounds.x, m_bounds.y + (i * itemHeight), m_bounds.width, itemHeight};
            renderer.drawRect(itemRect, graphics::Color{0.9f, 0.9f, 0.9f, 1.0f});
            renderer.drawText({itemRect.x + 5, itemRect.y + 2}, m_items[i], graphics::Color::Black());
        }
    }

    int handleClick(graphics::Point p) {
        float itemHeight = 25.0f;
        for (size_t i = 0; i < m_items.size(); ++i) {
            graphics::Rect itemRect = {m_bounds.x, m_bounds.y + (i * itemHeight), m_bounds.width, itemHeight};
            if (itemRect.contains(p)) return static_cast<int>(i);
        }
        return -1;
    }

private:
    std::vector<std::string> m_items;
};

} // namespace uxcpp::ui
