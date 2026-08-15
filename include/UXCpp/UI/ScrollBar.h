/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI/Widget.h>
#include <UXCpp/Graphics/Renderer.h>
#include <algorithm>

namespace uxcpp::ui {

class ScrollBar : public Widget {
public:
    enum class Orientation { Vertical, Horizontal };

    ScrollBar(std::string name, Orientation orientation) 
        : Widget(std::move(name), "scrollbar"), m_orientation(orientation), m_value(0.0f) {}

    void setValue(float value) {
        m_value = std::clamp(value, 0.0f, 1.0f);
    }

    float getValue() const { return m_value; }

    void onDraw(graphics::Renderer& renderer) override {
        // Draw track
        renderer.drawRect(m_bounds, graphics::Color{0.9f, 0.9f, 0.9f, 1.0f});

        // Draw thumb
        float thumbSize = (m_orientation == Orientation::Vertical) ? 30.0f : 20.0f;
        float pos = (m_orientation == Orientation::Vertical) 
                    ? m_bounds.y + (m_value * (m_bounds.height - thumbSize))
                    : m_bounds.x + (m_value * (m_bounds.width - thumbSize));

        graphics::Rect thumbRect;
        if (m_orientation == Orientation::Vertical) {
            thumbRect = {m_bounds.x + 2, pos, m_bounds.width - 4, thumbSize};
        } else {
            thumbRect = {pos, m_bounds.y + 2, thumbSize, m_bounds.height - 4};
        }

        renderer.drawRect(thumbRect, graphics::Color{0.6f, 0.6f, 0.6f, 1.0f});
    }

    bool onPointerDown(graphics::Point p) override {
        // Simplified: clicking the bar jumps to that position
        if (m_orientation == Orientation::Vertical) {
            m_value = (p.y - m_bounds.y) / m_bounds.height;
        } else {
            m_value = (p.x - m_bounds.x) / m_bounds.width;
        }
        setValue(m_value);
        return true;
    }

private:
    Orientation m_orientation;
    float m_value;
};

} // namespace uxcpp::ui
