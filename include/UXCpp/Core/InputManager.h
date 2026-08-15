/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/Renderer.h>
#include <UXCpp/Core/Application.h>
#include <UXCpp/UI/Menu.h>
#include <UXCpp/Core/ShortcutManager.h>
#include <UXCpp/UI/Dialog.h>
#include <UXCpp/UI/DropdownList.h>
#include <UXCpp/UI/Widget.h>
#include <memory>
#include <vector>
#include <algorithm>
#include <UXCpp/Core/DragDropManagerImpl.h>

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

    void handleMouseMove(float x, float y) {
        if (!m_root) return;

        ui::graphics::Point p{x, y};
        dispatchPointerMove(m_root, p);

        core::DragDropManager::getInstance().updateDragPosition(m_root, x, y);
    }

    void handleMouseUp(float x, float y) {
        if (!m_root) return;

        ui::graphics::Point p{x, y};
        dispatchPointerUp(m_root, p);

        // If we were dragging, try to drop on the widget under the mouse
        if (core::DragDropManager::getInstance().isDragging()) {
            auto target = findWidgetAt(m_root, p);
            if (target && target->onDrop(core::DragDropManager::getInstance().getPayload())) {
                // Drop accepted
            }
            core::DragDropManager::getInstance().endDrag();
        }
    }

    /**
     * @brief Dispatches a key press event to the focused widget.
     */
    void handleKeyDown(int key, bool ctrl = false, bool shift = false, bool alt = false) {
        if (!m_root) return;

        // 1. Try global shortcuts first
        if (core::ShortcutManager::getInstance().tryTriggerShortcut(key, ctrl, shift, alt)) {
            return;
        }

        // 2. Handle Tab for focus traversal
        if (key == 9 && !ctrl && !shift && !alt) { // ASCII Tab
            handleTabNavigation();
            return;
        }

        if (!m_focusedWidget) return;
        m_focusedWidget->onKeyDown(key);
    }

    void setFocus(std::shared_ptr<ui::Widget> widget) {
        if (m_focusedWidget == widget) return;

        if (m_focusedWidget) m_focusedWidget->onFocusLost();
        m_focusedWidget = widget;
        if (m_focusedWidget) m_focusedWidget->onFocusGained();
    }

private:
    InputManager() = default;

    /**
     * @brief Navigates focus to the next widget based on tab index.
     */
    void handleTabNavigation() {
        if (!m_root) return;

        std::vector<std::shared_ptr<ui::Widget>> focusable;
        collectFocusableWidgets(m_root, focusable);
        sortFocusableWidgets(focusable);

        if (focusable.empty()) return;

        int currentIdx = -1;
        for (int i = 0; i < (int)focusable.size(); ++i) {
            if (focusable[i] == m_focusedWidget) {
                currentIdx = i;
                break;
            }
        }

        int nextIdx = (currentIdx + 1) % focusable.size();
        setFocus(focusable[nextIdx]);
    }

    void collectFocusableWidgets(std::shared_ptr<ui::Widget> root, std::vector<std::shared_ptr<ui::Widget>>& out) {
        out.push_back(root);
        for (const auto& child : root->getChildren()) {
            collectFocusableWidgets(child, out);
        }
    }

    void sortFocusableWidgets(std::vector<std::shared_ptr<ui::Widget>>& widgets) {
        std::sort(widgets.begin(), widgets.end(), [](const auto& a, const auto& b) {
            if (a->getTabIndex() != b->getTabIndex()) {
                return a->getTabIndex() < b->getTabIndex();
            }
            // Fallback to pointer address for stability if indices are same
            return a < b;
        });
    }

    bool dispatchPointerDown(std::shared_ptr<ui::Widget> widget, ui::graphics::Point p) {
        // 1. Check Overlays first (Popups/Dropdowns have priority)
        const auto& overlays = core::Application::getInstance().getOverlays();
        std::shared_ptr<ui::MenuPopup> openMenuPopup;
        if (!overlays.empty()) {
            for (auto it = overlays.rbegin(); it != overlays.rend(); ++it) {
                if (auto menuPopup = std::dynamic_pointer_cast<ui::MenuPopup>(*it)) {
                    if (!menuPopup->getBounds().contains(p)) {
                        openMenuPopup = menuPopup;
                        continue;
                    }
                    if (menuPopup->onPointerDown(p)) return true;
                }

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

        if (openMenuPopup) {
            openMenuPopup->close();
            return true;
        }

        // 2. Check children of the root widget
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

    bool dispatchPointerMove(std::shared_ptr<ui::Widget> widget, ui::graphics::Point p) {
        const auto& children = widget->getChildren();
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            if ((*it)->getBounds().contains(p)) {
                if (dispatchPointerMove(*it, p)) return true;
            }
        }
        return widget->onPointerMove(p);
    }

    bool dispatchPointerUp(std::shared_ptr<ui::Widget> widget, ui::graphics::Point p) {
        const auto& children = widget->getChildren();
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            if ((*it)->getBounds().contains(p)) {
                if (dispatchPointerUp(*it, p)) return true;
            }
        }
        return widget->onPointerUp(p);
    }

    std::shared_ptr<ui::Widget> findWidgetAt(std::shared_ptr<ui::Widget> root, ui::graphics::Point p) {

        // 2. Check children of the root widget
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

    std::shared_ptr<ui::Widget> findWidgetAt(std::shared_ptr<ui::Widget> root, ui::graphics::Point p) {
        if (!root || !root->getBounds().contains(p)) return nullptr;

        for (auto it = root->getChildren().rbegin(); it != root->getChildren().rend(); ++it) {
            auto found = findWidgetAt(*it, p);
            if (found) return found;
        }
        return root;
    }
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
