/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/Renderer.h>
#include <UXCpp/UI/Widget.h>
#include <memory>

namespace uxcpp::core {

/**
 * @brief Manages the translation of raw OS/Backend events into UI Widget events.
 */
class InputManager {
public:
    static InputManager& getInstance() {
        static InputManager instance;
        return instance;
    }

    void setRootWidget(std::shared_ptr<ui::Widget> root) {
        m_root = root;
    }

    /**
     * @brief Dispatches a mouse click event to the widget tree.
     */
    void handleMouseDown(float x, float y) {
        if (!m_root) return;
        
        ui::graphics::Point p{x, y};
        dispatchPointerDown(m_root, p);
    }

    /**
     * @brief Dispatches a key press event to the focused widget.
     */
    void handleKeyDown(int key) {
        if (!m_root || !m_focusedWidget) return;
        m_focusedWidget->onKeyDown(key);
    }

    void setFocus(std::shared_ptr<ui::Widget> widget) {
        m_focusedWidget = widget;
    }

private:
    InputManager() = default;

    bool dispatchPointerDown(std::shared_ptr<ui::Widget> widget, ui::graphics::Point p) {
        // 1. Check Overlays first (Popups/Dropdowns have priority)
        const auto& overlays = core::Application::getInstance().getOverlays();
        if (!overlays.empty()) {
            for (auto it = overlays.rbegin(); it != overlays.rend(); ++it) {
                // If the top-most overlay is a Modal Dialog, block all other input
                if (auto dialog = std::dynamic_pointer_cast<ui::Dialog>(*it)) {
                    if (dialog->getBounds().contains(p)) {
                        return dialog->onPointerDown(p);
                    } else {
                        // Block input to everything below the modal
                        return true; 
                    }
                }

                if ((*it)->getBounds().contains(p)) {
                    if (auto list = std::dynamic_pointer_cast<ui::DropdownList>(*it)) {
                        int idx = list->handleClick(p);
                        // Selection logic...
                    }
                    if ((*it)->onPointerDown(p)) return true;
                }
            }
        }

        // 2. Check children of the root widget
// ...existing code...
        const auto& children = widget->getChildren();
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            if ((*it)->getBounds().contains(p)) {
                if (dispatchPointerDown(*it, p)) return true;
            }
        }

        // 3. Check this widget itself
        if (widget->onPointerDown(p)) {
            setFocus(widget);
            return true;
        }

        return false;
    }

    std::shared_ptr<ui::Widget> m_root;
    std::shared_ptr<ui::Widget> m_focusedWidget;
};

} // namespace uxcpp::core
