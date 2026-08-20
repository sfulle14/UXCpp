/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <UXCpp/Graphics/Renderer.h>

namespace uxcpp::graphics {

enum class ChartType { Line, Scatter, Bar, Area, Pie, Candlestick };

struct DataPoint {
    float x, y;
    float value2 = 0; // For candlestick (high/low) or area
};

struct ChartSeries {
    std::string name;
    Color color;
    std::vector<DataPoint> data;
};

class BaseChart {
public:
    virtual ~BaseChart() = default;
    virtual void render(Renderer& renderer, const Rect& bounds) = 0;
    
    void setZoom(float zoomX, float zoomY) { m_zoomX = zoomX; m_zoomY = zoomY; }
    float getZoomX() const { return m_zoomX; }
    float getZoomY() const { return m_zoomY; }

    void setPan(float panX, float panY) { m_panX = panX; m_panY = panY; }
    float getPanX() const { return m_panX; }
    float getPanY() const { return m_panY; }

protected:
    float m_zoomX = 1.0f, m_zoomY = 1.0f;
    float m_panX = 0.0f, m_panY = 0.0f;

    Rect calculatePlotArea(const Rect& bounds) {
        // Leave space for axes/legends
        return {bounds.x + 40, bounds.y + 20, bounds.width - 60, bounds.height - 40};
    }
};

class LineChart : public BaseChart {
public:
    void render(Renderer& renderer, const Rect& bounds) override {
        Rect plotArea = calculatePlotArea(bounds);
        renderer.beginPath();
        for (size_t i = 0; i < m_series[0].data.size(); ++i) {
            float px = plotArea.x + (m_series[0].data[i].x * m_zoomX) + m_panX;
            float py = plotArea.y + (m_series[0].data[i].y * m_zoomY) + m_panY;
            if (i == 0) renderer.moveTo(px, py); else renderer.lineTo(px, py);
        }
        renderer.strokePath(m_series[0].color, 2.0f);
    }

    void addSeries(const ChartSeries& series) { m_series.push_back(series); }

private:
    std::vector<ChartSeries> m_series;
};

class BarChart : public BaseChart {
public:
    void render(Renderer& renderer, const Rect& bounds) override {
        Rect plotArea = calculatePlotArea(bounds);
        float barWidth = (plotArea.width / m_series[0].data.size()) * 0.8f;
        for (size_t i = 0; i < m_series[0].data.size(); ++i) {
            float h = m_series[0].data[i].y * m_zoomY;
            renderer.drawRect({plotArea.x + i * (barWidth/0.8f), plotArea.y + plotArea.height - h, barWidth, h}, m_series[0].color);
        }
    }

    void addSeries(const ChartSeries& series) { m_series.push_back(series); }

private:
    std::vector<ChartSeries> m_series;
};

class PieChart : public BaseChart {
public:
    void render(Renderer& renderer, const Rect& bounds) override {
        float centerX = bounds.x + bounds.width / 2;
        float centerY = bounds.y + bounds.height / 2;
        float radius = std::min(bounds.width, bounds.height) / 2;
        
        float currentAngle = 0;
        for (const auto& dp : m_series[0].data) {
            float slice = (dp.y / 100.0f) * 2 * 3.14159f; // Assume sum is 100
            renderer.beginPath();
            renderer.moveTo(centerX, centerY);
            // Simplified arc rendering using lineTo for brevity in this prototype
            renderer.lineTo(centerX + cos(currentAngle)*radius, centerY + sin(currentAngle)*radius);
            renderer.lineTo(centerX + cos(currentAngle+slice)*radius, centerY + sin(currentAngle+slice)*radius);
            renderer.closePath();
            renderer.fillPath(m_series[0].color);
            currentAngle += slice;
        }
    }

    void addSeries(const ChartSeries& series) { m_series.push_back(series); }

private:
    std::vector<ChartSeries> m_series;
};

class ScatterChart : public BaseChart {
public:
    void render(Renderer& renderer, const Rect& bounds) override {
        Rect plotArea = calculatePlotArea(bounds);
        for (const auto& series : m_series) {
            for (const auto& dp : series.data) {
                float px = plotArea.x + (dp.x * m_zoomX) + m_panX;
                float py = plotArea.y + (dp.y * m_zoomY) + m_panY;
                renderer.drawCircle({px, py}, 3.0f, series.color);
            }
        }
    }

    void addSeries(const ChartSeries& series) { m_series.push_back(series); }

private:
    std::vector<ChartSeries> m_series;
};

class AreaChart : public BaseChart {
public:
    void render(Renderer& renderer, const Rect& bounds) override {
        Rect plotArea = calculatePlotArea(bounds);
        for (const auto& series : m_series) {
            renderer.beginPath();
            renderer.moveTo(plotArea.x, plotArea.y + plotArea.height);
            for (size_t i = 0; i < series.data.size(); ++i) {
                float px = plotArea.x + (series.data[i].x * m_zoomX) + m_panX;
                float py = plotArea.y + (series.data[i].y * m_zoomY) + m_panY;
                renderer.lineTo(px, py);
            }
            renderer.lineTo(plotArea.x + (series.data.back().x * m_zoomX) + m_panX, plotArea.y + plotArea.height);
            renderer.closePath();
            renderer.fillPath(series.color);
        }
    }

    void addSeries(const ChartSeries& series) { m_series.push_back(series); }

private:
    std::vector<ChartSeries> m_series;
};

class CandlestickChart : public BaseChart {
public:
    void render(Renderer& renderer, const Rect& bounds) override {
        Rect plotArea = calculatePlotArea(bounds);
        float candleWidth = (plotArea.width / m_series[0].data.size()) * 0.6f;
        for (size_t i = 0; i < m_series[0].data.size(); ++i) {
            const auto& dp = m_series[0].data[i];
            float px = plotArea.x + i * (candleWidth/0.6f) + candleWidth/2;
            float high = plotArea.y + (dp.value2 * m_zoomY) + m_panY; // value2 as high
            float low = plotArea.y + (dp.y * m_zoomY) + m_panY;       // y as low
            
            renderer.drawLine({px, high}, {px, low}, m_series[0].color);
            renderer.drawRect({px - candleWidth/2, plotArea.y + (dp.x * m_zoomY) + m_panY, candleWidth, 5.0f}, m_series[0].color);
        }
    }

    void addSeries(const ChartSeries& series) { m_series.push_back(series); }

private:
    std::vector<ChartSeries> m_series;
};

} // namespace uxcpp::graphics