/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI\Widget.h>

namespace uxcpp::ui {

enum class LayoutOrientation { Vertical, Horizontal };

class Box : public Widget {
public:
    Box(std::string name, LayoutOrientation orientation) 
        : Widget(std::move(name)), m_orientation(orientation) {}

    void onDraw(graphics::Renderer& renderer) override {
        // In a real layout engine, this would calculate child positions before drawing
        for (auto& child : m_children) {
            child->onDraw(renderer);
        }
    }

    /**
     * @brief Advanced layout calculation considering margins, padding, and stretch factors.
     */
    void performLayout() {
        const auto& params = layoutParams();
        float currentX = m_bounds.x + params.padding.left;
        float currentY = m_bounds.y + params.padding.top;

        float totalStretch = 0;
        float fixedSizeSum = 0;

        for (auto& child : m_children) {
            const auto& cp = child->layoutParams();
            if (cp.stretch > 0) {
                totalStretch += cp.stretch;
            } else {
                if (m_orientation == LayoutOrientation::Vertical) {
                    fixedSizeSum += (cp.preferredHeight > 0 ? cp.preferredHeight : 30.0f) + cp.margin.top + cp.margin.bottom;
                } else {
                    fixedSizeSum += (cp.preferredWidth > 0 ? cp.preferredWidth : 100.0f) + cp.margin.left + cp.margin.right;
                }
            }
        }

        float availableSpace = (m_orientation == LayoutOrientation::Vertical) 
            ? (m_bounds.height - params.padding.top - params.padding.bottom - fixedSizeSum)
            : (m_bounds.width - params.padding.left - params.padding.right - fixedSizeSum);

        for (auto& child : m_children) {
            const auto& cp = child->layoutParams();
            float finalW, finalH;

            if (m_orientation == LayoutOrientation::Vertical) {
                finalW = (cp.preferredWidth > 0) ? cp.preferredWidth : (m_bounds.width - params.padding.left - params.padding.right);
                finalH = (cp.stretch > 0) ? (availableSpace * (cp.stretch / totalStretch)) : (cp.preferredHeight > 0 ? cp.preferredHeight : 30.0f);
                
                float xPos = currentX + cp.margin.left;
                child->setBounds(xPos, currentY + cp.margin.top, finalW, finalH);
                currentY += finalH + cp.margin.top + cp.margin.bottom;
            } else {
                finalH = (cp.preferredHeight > 0) ? cp.preferredHeight : (m_bounds.height - params.padding.top - params.padding.bottom);
                finalW = (cp.stretch > 0) ? (availableSpace * (cp.stretch / totalStretch)) : (cp.preferredWidth > 0 ? cp.preferredWidth : 100.0f);
                
                float yPos = currentY + cp.margin.top;
                child->setBounds(currentX + cp.margin.left, yPos, finalW, finalH);
                currentX += finalW + cp.margin.left + cp.margin.right;
            }
        }
    }

private:
    LayoutOrientation m_orientation;
};

} // namespace uxcpp::ui
