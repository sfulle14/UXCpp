/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI/Widget.h>
#include <algorithm>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace uxcpp::ui {

struct PlotSample {
    float x = 0.0f;
    float y = 0.0f;
};

struct PlotSeries {
    std::string name;
    graphics::Color color = graphics::Color::Blue();
    std::vector<PlotSample> samples;
    bool visible = true;

    void addSample(float x, float y) {
        samples.push_back({x, y});
    }
};

class PlotWidget : public Widget {
public:
    explicit PlotWidget(std::string name)
        : Widget(std::move(name), "plot"), m_title(std::move(name)) {}

    void setTitle(std::string title) { m_title = std::move(title); }
    void setShowGrid(bool showGrid) { m_showGrid = showGrid; }
    void setBackgroundColor(graphics::Color color) { m_backgroundColor = color; }
    void setAxisColor(graphics::Color color) { m_axisColor = color; }
    void setGridColor(graphics::Color color) { m_gridColor = color; }
    void setPadding(float padding) { m_padding = std::max(0.0f, padding); }

    PlotSeries& addSeries(std::string seriesName, graphics::Color color = graphics::Color::Blue()) {
        m_series.push_back(PlotSeries{std::move(seriesName), color});
        return m_series.back();
    }

    void clearSeries() { m_series.clear(); }

    void onDraw(graphics::Renderer& renderer) override {
        renderer.drawRect(m_bounds, m_backgroundColor);

        const auto bounds = contentBounds();
        if (bounds.width <= 0.0f || bounds.height <= 0.0f) {
            return;
        }

        updateRanges();
        drawGrid(renderer, bounds);
        drawAxes(renderer, bounds);
        drawTitle(renderer);
        drawSeries(renderer, bounds);
        drawLegend(renderer, bounds);
    }

private:
    graphics::Rect contentBounds() const {
        return {
            m_bounds.x + m_padding,
            m_bounds.y + m_padding + (m_title.empty() ? 0.0f : 18.0f),
            std::max(0.0f, m_bounds.width - 2.0f * m_padding),
            std::max(0.0f, m_bounds.height - 2.0f * m_padding - (m_title.empty() ? 0.0f : 18.0f))
        };
    }

    void updateRanges() {
        if (!m_autoScale) {
            return;
        }

        if (!hasSamples()) {
            m_xMin = 0.0f;
            m_xMax = 1.0f;
            m_yMin = 0.0f;
            m_yMax = 1.0f;
            return;
        }

        float xMin = std::numeric_limits<float>::max();
        float xMax = std::numeric_limits<float>::lowest();
        float yMin = std::numeric_limits<float>::max();
        float yMax = std::numeric_limits<float>::lowest();

        for (const auto& series : m_series) {
            if (!series.visible) continue;
            for (const auto& sample : series.samples) {
                xMin = std::min(xMin, sample.x);
                xMax = std::max(xMax, sample.x);
                yMin = std::min(yMin, sample.y);
                yMax = std::max(yMax, sample.y);
            }
        }

        if (xMin == std::numeric_limits<float>::max()) {
            xMin = 0.0f; xMax = 1.0f; yMin = 0.0f; yMax = 1.0f;
        }

        if (xMin == xMax) { xMin -= 1.0f; xMax += 1.0f; }
        if (yMin == yMax) { yMin -= 1.0f; yMax += 1.0f; }

        m_xMin = xMin;
        m_xMax = xMax;
        m_yMin = yMin;
        m_yMax = yMax;
    }

    bool hasSamples() const {
        for (const auto& series : m_series) {
            if (!series.samples.empty() && series.visible) {
                return true;
            }
        }
        return false;
    }

    graphics::Point mapToScreen(const graphics::Rect& bounds, float x, float y) const {
        float xNorm = (x - m_xMin) / (m_xMax - m_xMin);
        float yNorm = (y - m_yMin) / (m_yMax - m_yMin);
        return {
            bounds.x + xNorm * bounds.width,
            bounds.y + (1.0f - yNorm) * bounds.height
        };
    }

    void drawTitle(graphics::Renderer& renderer) {
        if (!m_title.empty()) {
            renderer.drawText({m_bounds.x + m_padding, m_bounds.y + 2.0f}, m_title, m_axisColor);
        }
    }

    void drawAxes(graphics::Renderer& renderer, const graphics::Rect& bounds) {
        renderer.drawLine({bounds.x, bounds.y + bounds.height}, {bounds.x + bounds.width, bounds.y + bounds.height}, m_axisColor);
        renderer.drawLine({bounds.x, bounds.y}, {bounds.x, bounds.y + bounds.height}, m_axisColor);
    }

    void drawGrid(graphics::Renderer& renderer, const graphics::Rect& bounds) {
        if (!m_showGrid) return;

        constexpr int divisions = 5;
        for (int i = 1; i < divisions; ++i) {
            float t = static_cast<float>(i) / static_cast<float>(divisions);
            float x = bounds.x + bounds.width * t;
            float y = bounds.y + bounds.height * t;
            renderer.drawLine({x, bounds.y}, {x, bounds.y + bounds.height}, m_gridColor);
            renderer.drawLine({bounds.x, y}, {bounds.x + bounds.width, y}, m_gridColor);
        }
    }

    void drawSeries(graphics::Renderer& renderer, const graphics::Rect& bounds) {
        for (const auto& series : m_series) {
            if (!series.visible || series.samples.size() < 2) continue;
            for (size_t i = 1; i < series.samples.size(); ++i) {
                const auto& a = series.samples[i - 1];
                const auto& b = series.samples[i];
                auto p0 = mapToScreen(bounds, a.x, a.y);
                auto p1 = mapToScreen(bounds, b.x, b.y);
                renderer.drawLine(p0, p1, series.color);
            }
        }
    }

    void drawLegend(graphics::Renderer& renderer, const graphics::Rect& bounds) {
        float legendX = bounds.x + bounds.width - 140.0f;
        float legendY = bounds.y + 10.0f;
        for (const auto& series : m_series) {
            if (!series.visible) continue;
            renderer.drawRect({legendX, legendY + 4.0f, 10.0f, 10.0f}, series.color);
            renderer.drawText({legendX + 15.0f, legendY}, series.name, m_axisColor);
            legendY += 18.0f;
        }
    }

    std::string m_title;
    std::vector<PlotSeries> m_series;
    bool m_showGrid = true;
    bool m_autoScale = true;
    float m_padding = 12.0f;
    float m_xMin = 0.0f;
    float m_xMax = 1.0f;
    float m_yMin = 0.0f;
    float m_yMax = 1.0f;
    graphics::Color m_backgroundColor = {0.96f, 0.97f, 0.98f, 1.0f};
    graphics::Color m_axisColor = graphics::Color::Black();
    graphics::Color m_gridColor = {0.82f, 0.84f, 0.88f, 1.0f};
};

} // namespace uxcpp::ui
