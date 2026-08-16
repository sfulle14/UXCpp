/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Core/TableModel.h>
#include <UXCpp/Core/TableProxyModel.h>
#include <UXCpp/UI/ItemDelegate.h>
#include <memory>
#include <vector>
#include <string>

namespace uxcpp::ui {

/**
 * @brief A virtualized table view capable of rendering massive datasets.
 */
class VirtualTableView : public Widget {
public:
    explicit VirtualTableView(std::string name) 
        : Widget(std::move(name), "virtual_table"), m_model(nullptr), m_delegate(std::make_shared<ui::ItemDelegate>()) {}

    void setModel(std::shared_ptr<core::TableModel> model) {
        m_model = model;
    }

    void setDelegate(std::shared_ptr<ui::ItemDelegate> delegate) {
        m_delegate = delegate;
    }

    void onDraw(graphics::Renderer& renderer) override {
        if (!m_model) return;

        int totalRows = m_model->rowCount();
        int cols = m_model->columnCount();
        float cellH = 25.0f;
        float cellW = m_bounds.width / (cols > 0 ? cols : 1);

        // VIRTUALIZATION: Only render visible rows
        int startRow = std::max(0, (int)(m_scrollY / cellH));
        int endRow = std::min(totalRows, (int)((m_scrollY + m_bounds.height) / cellH) + 1);

        // Draw Headers (Fixed at top)
        for (int i = 0; i < cols; ++i) {
            graphics::Rect headerRect = {m_bounds.x + (i * cellW), m_bounds.y, cellW, cellH};
            renderer.drawRect(headerRect, graphics::Color{0.8f, 0.8f, 0.8f, 1.0f});
            renderer.drawText({headerRect.x + 5, headerRect.y + 2}, m_model->headerData(i, false), graphics::Color::Black());
        }

        // Draw Visible Rows
        for (int i = startRow; i < endRow; ++i) {
            float rowY = m_bounds.y + cellH + (i * cellH) - m_scrollY;
            if (rowY > m_bounds.y + m_bounds.height) break;

            for (int j = 0; j < cols; ++j) {
                graphics::Rect cellRect = {m_bounds.x + (j * cellW), rowY, cellW, cellH};
                // Use delegate for rendering
                m_delegate->renderCell(renderer, cellRect, m_model->data(i, j), false, false);
            }
        }
    }

    void setScrollY(float y) { m_scrollY = y; }

private:
    std::shared_ptr<core::TableModel> m_model;
    std::shared_ptr<ui::ItemDelegate> m_delegate;
    float m_scrollY = 0.0f;
};

} // namespace uxcpp::ui
