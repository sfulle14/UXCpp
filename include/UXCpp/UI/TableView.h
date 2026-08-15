/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI\Widget.h>
#include <UXCpp/Core/TableModel.h>
#include <memory>

namespace uxcpp::ui {

class TableView : public Widget {
public:
    TableView(std::string name) 
        : Widget(std::move(name), "tableview"), m_model(nullptr) {}

    void setModel(std::shared_ptr<core::TableModel> model) {
        m_model = model;
    }

    void onHeaderClicked(int column) {
        if (!m_model) return;

        // If the current model is a proxy, we can trigger sorting
        if (auto proxy = std::dynamic_pointer_cast<core::TableProxyModel>(m_model)) {
            static bool ascending = true;
            proxy->setSortColumn(column, ascending);
            ascending = !ascending;
        }
    }

    bool onPointerDown(graphics::Point p) override {
        if (!m_model) return false;

        float cellW = m_bounds.width / (m_model->columnCount() > 0 ? m_model->columnCount() : 1);
        float cellH = 25.0f;

        // Check if header was clicked
        if (p.y < m_bounds.y + cellH) {
            int col = static_cast<int>((p.x - m_bounds.x) / cellW);
            if (col >= 0 && col < m_model->columnCount()) {
                onHeaderClicked(col);
                return true;
            }
        }

        // Check if cells were clicked
        if (p.y >= m_bounds.y + cellH && p.y <= m_bounds.y + (m_model->rowCount() + 1) * cellH) {
            int row = static_cast<int>((p.y - (m_bounds.y + cellH)) / cellH);
            int col = static_cast<int>((p.x - m_bounds.x) / cellW);
            if (row >= 0 && row < m_model->rowCount() && col >= 0 && col < m_model->columnCount()) {
                return true; // Selection logic would go here
            }
        }

        return false;
    }

    void onDraw(graphics::Renderer& renderer) override {
        if (!m_model) return;

        int rows = m_model->rowCount();
        int cols = m_model->columnCount();
        float cellW = m_bounds.width / (cols > 0 ? cols : 1);
        float cellH = 25.0f;

        // Draw Headers
        for (int i = 0; i < cols; ++i) {
            graphics::Rect headerRect = {m_bounds.x + (i * cellW), m_bounds.y, cellW, cellH};
            renderer->drawRect(headerRect, graphics::Color{0.8f, 0.8f, 0.8f, 1.0f});
            renderer->drawText({headerRect.x + 5, headerRect.y + 2}, m_model->headerData(i, false), graphics::Color::Black());
        }

        // Draw Rows
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                graphics::Rect cellRect = {m_bounds.x + (j * cellW), m_bounds.y + ((i + 1) * cellH), cellW, cellH};
                renderer->drawRect(cellRect, graphics::Color::White());
                renderer->drawText({cellRect.x + 5, cellRect.y + 2}, m_model->data(i, j), graphics::Color::Black());
            }
        }
    }

private:
    std::shared_ptr<core::TableModel> m_model;
};

} // namespace uxcpp::ui
