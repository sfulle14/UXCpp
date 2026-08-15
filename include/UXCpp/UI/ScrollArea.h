/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI/Widget.h>
#include <UXCpp/Graphics/Renderer.h>
#include <UXCpp/UI/ScrollBar.h>
#include <memory>

namespace uxcpp::ui {

class ScrollArea : public Widget {
public:
    ScrollArea(std::string name) 
        : Widget(std::move(name), "scrollarea"), m_scrollX(0), m_scrollY(0) {
        m_vBar = std::make_shared<ScrollBar>("VBar", ScrollBar::Orientation::Vertical);
        m_hBar = std::make_shared<ScrollBar>("HBar", ScrollBar::Orientation::Horizontal);
    }

    void setContent(std::shared_ptr<Widget> content) {
        m_content = content;
        addChild(content);
    }

    void onDraw(graphics::Renderer& renderer) override {
        renderer.setClipRect(m_bounds);
        
        // Draw children with offset (conceptual, as Renderer doesn't support offsets yet)
        for (auto& child : m_children) {
            child->onDraw(renderer);
        }

        // Draw scrollbars on top of the content area
        if (m_vBar) {
            m_vBar->setBounds(m_bounds.x + m_bounds.width - 15, m_bounds.y, 15, m_bounds.height);
            m_vBar->onDraw(renderer);
        }
        if (m_hBar) {
            m_hBar->setBounds(m_bounds.x, m_bounds.y + m_bounds.height - 15, m_bounds.width, 15);
            m_hBar->onDraw(renderer);
        }
    }

    void scrollBy(float dx, float dy) {
        m_scrollX += dx;
        m_scrollY += dy;
        updateBars();
    }

private:
    void updateBars() {
        if (m_vBar) m_vBar->setValue(m_scrollY / 1000.0f); // Simplified ratio
        if (m_hBar) m_hBar->setValue(m_scrollX / 1000.0f);
    }

    std::shared_ptr<Widget> m_content;
    std::shared_ptr<ScrollBar> m_vBar;
    std::shared_ptr<ScrollBar> m_hBar;
    float m_scrollX, m_scrollY;
};

} // namespace uxcpp::ui
