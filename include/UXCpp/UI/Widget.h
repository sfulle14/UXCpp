/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/Renderer.h>
#include <UXCpp/Core/Signal.h>
#include <UXCpp/UI/Style.h>
#include <UXCpp/UI/AccessibilityTree.h>
#include <vector>
#include <memory>
#include <string>
#include <UXCpp/UI/Widget.h>
#include <UXCpp/UI/LayoutParams.h>

namespace uxcpp::ui {

class Widget : public std::enable_shared_from_this<Widget> {
public:
    explicit Widget(std::string name, std::string styleClass = "default") 
        : m_name(std::move(name)), m_styleClass(std::move(styleClass)) {}
    virtual ~Widget() = default;

    // Style access
    const WidgetStyle& getStyle() const {
        return Theme::getInstance().getStyle(m_styleClass);
    }

    void setStyleClass(std::string styleClass) { m_styleClass = std::move(styleClass); }

    // Hierarchy management
    void addChild(std::shared_ptr<Widget> child) {
        child->m_parent = shared_from_this();
        m_children.push_back(child);
    }

    // Geometry
    void setBounds(float x, float y, float w, float h) { m_bounds = {x, y, w, h}; }
    graphics::Rect getBounds() const { return m_bounds; }

    int getTabIndex() const { return m_tabIndex; }
    void setTabIndex(int index) { m_tabIndex = index; }

    // Accessibility
    void setAccessibleName(std::string name) { m_accessibleName = std::move(name); }
    const std::string& getAccessibleName() const { return m_accessibleName; }

    void setAccessibleDescription(std::string desc) { m_accessibleDescription = std::move(desc); }
    const std::string& getAccessibleDescription() const { return m_accessibleDescription; }

    void setAccessibleRole(ui::AccessibleRole role) { m_accessibleRole = role; }
    ui::AccessibleRole getAccessibleRole() const { return m_accessibleRole; }

    LayoutParams& layoutParams() { return m_layoutParams; }
    const LayoutParams& layoutParams() const { return m_layoutParams; }

    // Lifecycle & Events
    virtual void onInit() {}
    virtual void onUpdate(float deltaTime) {}
    virtual void onDraw(graphics::Renderer& renderer) = 0;

    /**
     * @brief Returns the widget's unique identifier for debugging and tooling.
     */
    const std::string& getId() const { return m_id; }
    void setId(std::string id) { m_id = std::move(id); }

    // Input handling
    virtual bool onPointerDown(graphics::Point p) { return false; }
    virtual bool onPointerMove(graphics::Point p) { return false; }
    virtual bool onPointerUp(graphics::Point p) { return false; }
    virtual bool onKeyDown(int key) { return false; }

    /**
     * @brief Triggered when the widget is right-clicked.
     */
    virtual void onContextMenuRequested(int x, int y) {}

    /**
     * @brief Called when a drag-and-drop operation enters this widget.
     */
    virtual void onDragEnter(const core::DragDropPayload& payload) {}

    /**
     * @brief Called when a drag-and-drop operation leaves this widget.
     */
    virtual void onDragLeave() {}

    /**
     * @brief Called when a drag-and-drop operation is dropped onto this widget.
     * @return true if the drop was accepted.
     */
    virtual bool onDrop(const core::DragDropPayload& payload) { return false; }

    /**
     * @brief Called when the widget gains keyboard focus.
     */
    virtual void onFocusGained() {}

    /**
     * @brief Called when the widget loses keyboard focus.
     */
    virtual void onFocusLost() {}

    bool isFocused() const { return m_isFocused; }
    void setFocused(bool focused) { m_isFocused = focused; }

    // Signals for common events
    uxcpp::core::Signal<> onClicked;

    const std::string& getName() const { return m_name; }

    /**
     * @brief Returns the children of this widget.
     */
    const std::vector<std::shared_ptr<Widget>>& getChildren() const {
        return m_children;
    }

    /**
     * @brief Dispatches an event to this widget and its children.
     * @return true if the event was handled.
     */
    virtual bool dispatchEvent(const std::string& eventType, void* data) {
        // Default implementation: check children first (top-down)
        for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
            if ((*it)->dispatchEvent(eventType, data)) return true;
        }
        return false;
    }

protected:
    std::string m_name;
    std::string m_styleClass;
    graphics::Rect m_bounds{0, 0, 0, 0};
    int m_tabIndex = 0;
    std::string m_accessibleName;
    std::string m_accessibleDescription;
    ui::AccessibleRole m_accessibleRole = ui::AccessibleRole::None;
    LayoutParams m_layoutParams;
    bool m_isFocused = false;
    std::weak_ptr<Widget> m_parent;
    std::vector<std::shared_ptr<Widget>> m_children;
};

} // namespace uxcpp::ui
