/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI\Widget.h>
#include <UXCpp/Core/TreeModel.h>
#include <memory>

namespace uxcpp::ui {

class TreeView : public Widget {
public:
    TreeView(std::string name) 
        : Widget(std::move(name), "treeview"), m_model(nullptr) {}

    void setModel(std::shared_ptr<core::TreeModel> model) {
        m_model = model;
    }

    void onDraw(graphics::Renderer& renderer) override {
        if (!m_model) return;
        
        float currentY = 0;
        drawNode(renderer, m_model->getRoot(), 0, currentY);
    }

    bool onPointerDown(graphics::Point p) override {
        if (!m_model) return false;
        
        float currentY = 0;
        return handleNodeClick(m_model->getRoot(), 0, currentY, p);
    }

private:
    void drawNode(graphics::Renderer& renderer, std::shared_ptr<core::TreeNode> node, int depth, float& y) {
        if (!node) return;

        float xPos = m_bounds.x + (depth * 20.0f);
        graphics::Rect nodeRect = {xPos, m_bounds.y + y, m_bounds.width - (depth * 20.0f), 25.0f};
        
        // Draw expand/collapse icon
        if (!node->children.empty()) {
            std::string icon = node->expanded ? "[-] " : "[+] ";
            renderer->drawText({nodeRect.x, nodeRect.y + 2}, icon, graphics::Color::Black());
            renderer->drawText({nodeRect.x + 30, nodeRect.y + 2}, node->text, graphics::Color::Black());
        } else {
            renderer->drawText({nodeRect.x + 20, nodeRect.y + 2}, node->text, graphics::Color::Black());
        }

        y += 25.0f;

        if (node->expanded) {
            for (auto& child : node->children) {
                drawNode(renderer, child, depth + 1, y);
            }
        }
    }

    bool handleNodeClick(std::shared_ptr<core::TreeNode> node, int depth, float& y, graphics::Point p) {
        float xPos = m_bounds.x + (depth * 20.0f);
        graphics::Rect nodeRect = {xPos, m_bounds.y + y, m_bounds.width - (depth * 20.0f), 25.0f};

        if (nodeRect.contains(p)) {
            if (!node->children.empty()) {
                node->expanded = !node->expanded;
                return true;
            }
        }

        y += 25.0f;
        if (node->expanded) {
            for (auto& child : node->children) {
                if (handleNodeClick(child, depth + 1, y, p)) return true;
            }
        }
        return false;
    }

    std::shared_ptr<core::TreeModel> m_model;
};

} // namespace uxcpp::ui
