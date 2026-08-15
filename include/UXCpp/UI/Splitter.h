/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI/Widget.h>
#include <UXCpp/UI/Box.h>
#include <vector>
#include <memory>

namespace uxcpp::ui {

/**
 * @brief A Splitter widget that allows users to resize adjacent children by dragging a divider.
 */
class Splitter : public Box {
public:
    enum class Orientation { Horizontal, Vertical };

    explicit Splitter(std::string name, Orientation orientation = Orientation::Horizontal) 
        : Box(std::move(name), 
              orientation == Orientation::Horizontal ? LayoutOrientation::Horizontal : LayoutOrientation::Vertical),
          m_orientation(orientation), m_isDraggingDivider(false), m_activeDividerIndex(-1) {}

    /**
     * @brief Adds a child to the splitter.
     */
    void addSplitChild(std::shared_ptr<Widget> child, float initialWeight = 1.0f) {
        m_weights.push_back(initialWeight);
        addChild(child);
        performLayout();
    }

    void onDraw(graphics::Renderer& renderer) override {
        // Draw children (handled by Box usually, but we might want to draw dividers)
        Box::onDraw(renderer);

        // Draw dividers
        float pos = 0;
        if (m_orientation == Orientation::Horizontal) {
            for (size_t i = 0; i < m_children.size() - 1; ++i) {
                float childW = calculateChildWidth(i);
                pos += childW;
                graphics::Rect dividerRect{m_bounds.x + pos - 2, m_bounds.y, 4, m_bounds.height};
                renderer.drawRect(dividerRect, graphics::Color{0.7f, 0.7f, 0.7f, 1.0f});
            }
        } else {
            for (size_t i = 0; i < m_children.size() - 1; ++i) {
                float childH = calculateChildHeight(i);
                pos += childH;
                graphics::Rect dividerRect{m_bounds.x, m_bounds.y + pos - 2, m_bounds.width, 4};
                renderer.drawRect(dividerRect, graphics::Color{0.7f, 0.7f, 0.7f, 1.0f});
            }
        }
    }

    bool onPointerDown(graphics::Point p) override {
        // Check if we clicked a divider
        if (m_orientation == Orientation::Horizontal) {
            float pos = 0;
            for (size_t i = 0; i < m_children.size() - 1; ++i) {
                pos += calculateChildWidth(i);
                if (std::abs(p.x - (m_bounds.x + pos)) < 5.0f) {
                    m_isDraggingDivider = true;
                    m_activeDividerIndex = static_cast<int>(i);
                    return true;
                }
            }
        } else {
            float pos = 0;
            for (size_t i = 0; i < m_children.size() - 1; ++i) {
                pos += calculateChildHeight(i);
                if (std::abs(p.y - (m_bounds.y + pos)) < 5.0f) {
                    m_isDraggingDivider = true;
                    m_activeDividerIndex = static_cast<int>(i);
                    return true;
                }
            }
        }
        return Box::onPointerDown(p);
    }

    bool onPointerMove(graphics::Point p) override {
        if (!m_isDraggingDivider) return false;

        if (m_orientation == Orientation::Horizontal) {
            float relativeX = p.x - m_bounds.x;
            updateWeightsFromPos(relativeX);
        } else {
            float relativeY = p.y - m_bounds.y;
            updateWeightsFromPos(relativeY);
        }
        performLayout();
        return true;
    }

    bool onPointerUp(graphics::Point p) override {
        m_isDraggingDivider = false;
        m_activeDividerIndex = -1;
        return true;
    }

private:
    float calculateChildWidth(size_t index) {
        float totalWeight = 0;
        for (auto w : m_weights) totalWeight += w;
        return (m_weights[index] / totalWeight) * m_bounds.width;
    }

    float calculateChildHeight(size_t index) {
        float totalWeight = 0;
        for (auto w : m_weights) totalWeight += w;
        return (m_weights[index] / totalWeight) * m_bounds.height;
    }

    void updateWeightsFromPos(float pos) {
        if (m_activeDividerIndex < 0 || m_activeDividerIndex >= static_cast<int>(m_weights.size()) - 1) return;

        float totalSize = (m_orientation == Orientation::Horizontal) ? m_bounds.width : m_bounds.height;
        float ratio = pos / totalSize;
        
        if (ratio < 0.05f) ratio = 0.05f;
        if (ratio > 0.95f) ratio = 0.95f;

        m_weights[m_activeDividerIndex] = ratio * 100.0f;
        m_weights[m_activeDividerIndex + 1] = (1.0f - ratio) * 100.0f;
    }

    Orientation m_orientation;
    std::vector<float> m_weights;
    bool m_isDraggingDivider;
    int m_activeDividerIndex;
};

} // namespace uxcpp::ui
