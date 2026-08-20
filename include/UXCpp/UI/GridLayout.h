/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI\Widget.h>
#include <UXCpp/UI\LayoutParams.h>
#include <vector>

namespace uxcpp::ui {

class GridLayout : public Widget {
public:
    GridLayout(std::string name, int rows, int cols) 
        : Widget(std::move(name), "grid"), m_rows(rows), m_cols(cols) {}

    void addWidget(std::shared_ptr<Widget> widget, int row, int col, int rowSpan = 1, int colSpan = 1) {
        m_cells.push_back({widget, row, col, rowSpan, colSpan});
        addChild(widget);
    }

    void onDraw(graphics::Renderer& renderer) override {
        for (auto& cell : m_cells) {
            cell.widget->onDraw(renderer);
        }
    }

    void performLayout() {
        if (m_rows == 0 || m_cols == 0) return;

        const auto& params = layoutParams();
        float availableW = m_bounds.width - params.padding.left - params.padding.right;
        float availableH = m_bounds.height - params.padding.top - params.padding.bottom;
        
        float cellW = availableW / m_cols;
        float cellH = availableH / m_rows;

        for (auto& cell : m_cells) {
            float x = m_bounds.x + params.padding.left + (cell.col * cellW);
            float y = m_bounds.y + params.padding.top + (cell.row * cellH);
            float w = cell.colSpan * cellW;
            float h = cell.rowSpan * cellH;
            cell.widget->setBounds(x, y, w, h);
        }
    }

private:
    struct Cell {
        std::shared_ptr<Widget> widget;
        int row, col, rowSpan, colSpan;
    };

    int m_rows, m_cols;
    std::vector<Cell> m_cells;
};

} // namespace uxcpp::ui
