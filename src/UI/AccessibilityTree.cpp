/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/UI/AccessibilityTree.h>
#include <UXCpp/UI/Widget.h>
#include <memory>

namespace uxcpp::ui {

std::shared_ptr<AccessibilityNode> AccessibilityTree::buildTree(std::shared_ptr<Widget> root) {
    if (!root) return nullptr;

    auto node = std::make_shared<AccessibilityNode>();
    node->name = root->getAccessibleName();
    node->description = root->getAccessibleDescription();
    node->role = root->getAccessibleRole();
    
    // Map widget state to accessibility state
    node->state.focused = root->isFocused();
    node->state.enabled = true; // Simplified

    for (const auto& child : root->getChildren()) {
        auto childNode = buildTree(child);
        if (childNode) {
            node->children.push_back(childNode);
        }
    }

    return node;
}

void AccessibilityTree::notifyEvent(const std::string& eventType, std::shared_ptr<Widget> widget) {
    // In a real implementation, this would call OS APIs like UIA (Windows) or NSAccessibility (macOS)
    std::cout << "[Accessibility] Event: " << eventType << " on Widget: " << widget->getName() << std::endl;
}

} // namespace uxcpp::ui
