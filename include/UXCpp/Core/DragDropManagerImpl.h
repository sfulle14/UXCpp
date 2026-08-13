/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Core/DragDropManager.h>
#include <UXCpp/UI/Widget.h>
#include <vector>

namespace uxcpp::core {

// Implementation of updateDragPosition which needs Widget definition
inline void DragDropManager::updateDragPosition(std::shared_ptr<ui::Widget> root, float x, float y) {
    if (!m_isDragging || !root) return;

    // Find the widget under the mouse (simplified hit test)
    std::shared_ptr<ui::Widget> currentTarget = nullptr;
    
    // We can reuse a similar logic to InputManager's dispatchPointerDown 
    // but without triggering events. For now, we assume root is the container.
    
    // This is a simplified search for the top-most widget containing (x,y)
    auto findTopWidget = [&](auto self, std::shared_ptr<ui::Widget> w) -> std::shared_ptr<ui::Widget> {
        if (!w->getBounds().contains({x, y})) return nullptr;
        
        for (auto it = w->getChildren().rbegin(); it != w->getChildren().rend(); ++it) {
            auto found = self(self, *it);
            if (found) return found;
        }
        return w;
    };

    currentTarget = findTopWidget(findTopWidget, root);

    if (currentTarget != m_lastTarget) {
        if (m_lastTarget) m_lastTarget->onDragLeave();
        if (currentTarget) currentTarget->onDragEnter(m_payload);
        m_lastTarget = currentTarget;
    }
}

} // namespace uxcpp::core
