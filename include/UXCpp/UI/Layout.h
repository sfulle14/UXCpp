/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI\Widget.h>

namespace uxcpp::ui {

enum class LayoutOrientation { Vertical, Horizontal };

class Box : public Widget {
public:
    Box(std::string name, LayoutOrientation orientation) 
        : Widget(std::move(name)), m_orientation(orientation) {}

    void onDraw(graphics::Renderer& renderer) override {
        // In a real layout engine, this would calculate child positions before drawing
        for (auto& child : m_children) {
            child->onDraw(renderer);
        }
    }

    /**
     * @brief Simple linear layout calculation.
     */
    void performLayout() {
        float offset = 0;
        for (auto& child : m_children) {
            if (m_orientation == LayoutOrientation::Vertical) {
                child->setBounds(m_bounds.x, m_bounds.y + offset, m_bounds.width, 30.0f);
                offset += 35.0f;
            } else {
                child->setBounds(m_bounds.x + offset, m_bounds.y, 100.0f, m_bounds.height);
                offset += 105.0f;
            }
        }
    }

private:
    LayoutOrientation m_orientation;
};

} // namespace uxcpp::ui
