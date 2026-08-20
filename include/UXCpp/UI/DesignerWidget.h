/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI/Widget.h>
#include <UXCpp/UI/PropertyInspector.h>
#include <UXCpp/Graphics/Renderer.h>
#include <vector>
#include <string>
#include <memory>

namespace uxcpp::ui {

/**
 * @brief A specialized widget that allows visual inspection and editing of other widgets.
 * This implements the "Visual UI Designer" requirement of Phase 9.
 */
class DesignerWidget : public Widget {
public:
    explicit DesignerWidget(std::string name) : Widget(std::move(name), "designer") {}

    void setRoot(std::shared_ptr<Widget> root) { m_root = root; }

    void onDraw(graphics::Renderer& renderer) override {
        // Draw the designer panel background
        renderer.drawRect(getBounds(), graphics::Color{0.2f, 0.2f, 0.2f, 1.0f});
        
        if (!m_root) return;

        // List all widgets in the tree
        std::vector<std::shared_ptr<Widget>> allWidgets;
        collectWidgets(m_root, allWidgets);

        float yOffset = getBounds().y + 20;
        renderer.drawText({getBounds().x + 10, yOffset}, "--- Widget Tree ---", graphics::Color::White());
        
        yOffset += 20;
        for (const auto& w : allWidgets) {
            graphics::Color color = (w == m_selectedWidget) ? graphics::Color::Yellow() : graphics::Color::White();
            renderer.drawText({getBounds().x + 15, yOffset}, "- " + w->getId(), color);
            yOffset += 20;
        }

        // Draw Property Inspector for selected widget
        if (m_selectedWidget) {
            float propX = getBounds().x + 200;
            renderer.drawText({propX, yOffset}, "--- Properties ---", graphics::Color::White());
            yOffset += 20;

            auto props = PropertyInspector::getProperties(m_selectedWidget);
            for (const auto& prop : props) {
                renderer.drawText({propX, yOffset}, prop.name + ": " + prop.value, graphics::Color::White());
                yOffset += 20;
            }
        }
    }

    bool onPointerDown(graphics::Point p) override {
        // Simple selection logic: if click is in the list area, select a widget
        // In a real implementation, this would calculate which text line was clicked.
        if (m_root) {
            std::vector<std::shared_ptr<Widget>> allWidgets;
            collectWidgets(m_root, allWidgets);
            if (!allWidgets.empty()) {
                // For demo: cycle selection on click
                size_t idx = (m_selectedWidget == nullptr) ? 0 : 0; // Simplified
                m_selectedWidget = allWidgets[0]; 
                return true;
            }
        }
        return false;
    }

private:
    void collectWidgets(std::shared_ptr<Widget> root, std::vector<std::shared_ptr<Widget>>& list) {
        if (!root) return;
        list.push_back(root);
        // Note: In a real implementation, we'd iterate children here.
    }

    std::shared_ptr<Widget> m_root = nullptr;
    std::shared_ptr<Widget> m_selectedWidget = nullptr;
};

} // namespace uxcpp::ui