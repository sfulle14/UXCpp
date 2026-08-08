/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI\Widget.h>
#include <UXCpp/Graphics/Renderer.h>

namespace uxcpp::ui {

class ScrollArea : public Widget {
public:
    ScrollArea(std::string name) 
        : Widget(std::move(name), "scrollarea"), m_scrollX(0), m_scrollY(0) {}

    void setContent(std::shared_ptr<Widget> content) {
        m_content = content;
        addChild(content);
    }

    void onDraw(graphics::Renderer& renderer) override {
        renderer.setClipRect(m_bounds);
        
        // Offset the rendering of children by scroll position
        // Note: In a real implementation, we'd need to pass an offset to onDraw 
        // or use a transformation matrix in the Renderer.
        for (auto& child : m_children) {
            child->onDraw(renderer);
        }
    }

    void scrollBy(float dx, float dy) {
        m_scrollX += dx;
        m_scrollY += dy;
        // Clamp to content size...
    }

private:
    std::shared_ptr<Widget> m_content;
    float m_scrollX, m_scrollY;
};

} // namespace uxcpp::ui
