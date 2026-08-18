/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI/Widget.h>
#include <UXCpp/UI/ItemDelegate.h>
#include <memory>
#include <vector>
#include <string>

namespace uxcpp::ui {

/**
 * @brief A virtualized list view capable of rendering massive datasets.
 */
class VirtualListView : public Widget {
public:
    explicit VirtualListView(std::string name) 
        : Widget(std::move(name), "virtual_list"), m_model(nullptr) {}

    void setModel(std::shared_ptr<core::TableModel> model) {
        m_model = model;
    }

    void setDelegate(std::shared_ptr<ui::ItemDelegate> delegate) {
        m_delegate = delegate;
    }

    void onDraw(graphics::Renderer& renderer) override {
        if (!m_model) return;

        int totalItems = m_model->rowCount();
        float itemH = 25.0f;

        // VIRTUALIZATION: Only render visible items
        int startIdx = std::max(0, (int)(m_scrollY / itemH));
        int endIdx = std::min(totalItems, (int)((m_scrollY + m_bounds.height) / itemH) + 1);

        for (int i = startIdx; i < endIdx; ++i) {
            float itemY = m_bounds.y + (i * itemH) - m_scrollY;
            if (itemY > m_bounds.y + m_bounds.height) break;

            graphics::Rect itemRect = {m_bounds.x, itemY, m_bounds.width, itemH};
            
            // Use delegate for rendering the list item
            // We use column 0 as the primary data source for lists
            m_delegate->renderCell(renderer, itemRect, m_model->data(i, 0), false, false);
        }
    }

    void setScrollY(float y) { m_scrollY = y; }

private:
    std::shared_ptr<core::TableModel> m_model;
    std::shared_ptr<ui::ItemDelegate> m_delegate = std::make_shared<ui::ItemDelegate>();
    float m_scrollY = 0.0f;
};

} // namespace uxcpp::ui
