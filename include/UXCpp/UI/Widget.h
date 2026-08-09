/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/Renderer.h>
#include <UXCpp/Core/Signal.h>
#include <UXCpp/UI/Style.h>
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

    LayoutParams& layoutParams() { return m_layoutParams; }
    const LayoutParams& layoutParams() const { return m_layoutParams; }

    // Lifecycle & Events
    virtual void onInit() {}
    virtual void onUpdate(float deltaTime) {}
    virtual void onDraw(graphics::Renderer& renderer) = 0;

    // Input handling
    virtual bool onPointerDown(graphics::Point p) { return false; }
    virtual bool onKeyDown(int key) { return false; }

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
    LayoutParams m_layoutParams;
    bool m_isFocused = false;
    std::weak_ptr<Widget> m_parent;
    std::vector<std::shared_ptr<Widget>> m_children;
};

} // namespace uxcpp::ui
