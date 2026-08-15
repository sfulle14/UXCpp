/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace uxcpp::ui {

/**
 * @brief Semantic roles for accessibility.
 */
enum class AccessibleRole {
    None,
    Window,
    Button,
    TextField,
    Checkbox,
    Slider,
    List,
    Tree,
    Label,
    Dialog,
    Toolbar,
    MenuBar
};

/**
 * @brief State of an accessible element.
 */
struct AccessibleState {
    bool focused = false;
    bool enabled = true;
    bool selected = false;
    bool expanded = false;
    bool checked = false;
};

/**
 * @brief A snapshot of a widget's accessibility properties.
 */
struct AccessibilityNode {
    std::string name;
    std::string description;
    AccessibleRole role = AccessibleRole::None;
    AccessibleState state;
    std::vector<std::shared_ptr<AccessibilityNode>> children;
};

/**
 * @brief Manages the accessibility tree for the application.
 */
class AccessibilityTree {
public:
    static AccessibilityTree& getInstance() {
        static AccessibilityTree instance;
        return instance;
    }

    /**
     * @brief Builds a snapshot of the current UI hierarchy for screen readers.
     */
    std::shared_ptr<AccessibilityNode> buildTree(std::shared_ptr<class Widget> root);

    /**
     * @brief Notifies the OS that an accessibility property has changed.
     */
    void notifyEvent(const std::string& eventType, std::shared_ptr<class Widget> widget);

private:
    AccessibilityTree() = default;
};

} // namespace uxcpp::ui
