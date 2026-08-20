/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI/Widget.h>
#include <UXCpp/Graphics/Charts.h>
#include <memory>

namespace uxcpp::ui {

/**
 * @brief A widget that hosts a chart and provides interactive zoom and pan.
 */
class ChartWidget : public Widget {
public:
    explicit ChartWidget(std::string name, std::unique_ptr<graphics::BaseChart> chart) 
        : Widget(std::move(name)), m_chart(std::move(chart)) {}

    void onDraw(graphics::Renderer& renderer) override {
        if (m_chart) {
            m_chart->render(renderer, getBounds());
        }
    }

    bool onPointerDown(graphics::Point p) override {
        m_isDragging = true;
        m_lastMousePos = p;
        return true;
    }

    bool onPointerMove(graphics::Point p) override {
        if (m_isDragging) {
            float dx = p.x - m_lastMousePos.x;
            float dy = p.y - m_lastMousePos.y;
            
            // Update chart pan based on drag
            // Note: In a real implementation, we'd scale the delta by zoom level
            m_chart->setPan(m_chart->getPanX() + dx, m_chart->getPanY() + dy);
            
            m_lastMousePos = p;
            return true;
        }
        return false;
    }

    bool onPointerUp(graphics::Point p) override {
        m_isDragging = false;
        return true;
    }

    /**
     * @brief Handles mouse wheel for zooming. 
     * Note: This would typically be called from the InputManager or a specialized event.
     */
    void handleZoom(float delta, graphics::Point center) {
        float zoomFactor = (delta > 0) ? 1.1f : 0.9f;
        m_chart->setZoom(m_chart->getZoomX() * zoomFactor, m_chart->getZoomY() * zoomFactor);
    }

private:
    std::unique_ptr<graphics::BaseChart> m_chart;
    bool m_isDragging = false;
    graphics::Point m_lastMousePos{0, 0};
};

} // namespace uxcpp::ui