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

        float cellW = m_bounds.width / m_cols;
        float cellH = m_bounds.height / m_rows;

        for (auto& cell : m_cells) {
            float x = m_bounds.x + (cell.col * cellW);
            float y = m_bounds.y + (cell.row * cellH);
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
