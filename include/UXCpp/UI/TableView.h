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
