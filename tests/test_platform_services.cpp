/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Core/Application.h>
#include <UXCpp/UI/Box.h>
#include <UXCpp/UI/Button.h>
#include <UXCpp/UI/TextInput.h>
#include <UXCpp/UI/Label.h>
#include <UXCpp/UI/AccessibilityTree.h>
#include <UXCpp/Core/ShortcutManager.h>
#include <iostream>

int main() {
    auto& app = uxcpp::core::Application::getInstance();
    if (!app.init()) return -1;

    auto root = std::make_shared<uxcpp::ui::Box>("Root", uxcpp::ui::LayoutOrientation::Vertical);
    root->setBounds(0, 0, 800, 600);

    // Test Accessibility & Focus
    auto btn = std::make_shared<uxcpp::ui::Button>("TestBtn", "Click Me");
    btn->setAccessibleName("Submit Button");
    btn->setAccessibleRole(uxcpp::ui::AccessibleRole::Button);
    btn->setTabIndex(1);

    auto input = std::make_shared<uxcpp::ui::TextInput>("TestInput", "Enter text...");
    input->setAccessibleName("Username Field");
    input->setAccessibleRole(uxcpp::ui::AccessibleRole::TextField);
    input->setTabIndex(0);

    root->addChild(input);
    root->addChild(btn);

    // Test Shortcuts
    uxcpp::core::ShortcutManager::getInstance().registerShortcut(
        {'Q', true, false, false}, 
        []() { std::cout << "Global Quit Shortcut Triggered!" << std::endl; }
    );

    std::cout << "[TestHarness] UI Initialized. Testing Accessibility Tree..." << std::endl;
    auto accTree = uxcpp::ui::AccessibilityTree::getInstance().buildTree(root);
    if (accTree) {
        std::cout << "Root Node: " << accTree->name << " Role: " << (int)accTree->role << std::endl;
        for (auto& child : accTree->children) {
            std::cout << " - Child: " << child->name << " Role: " << (int)child->role << std::endl;
        }
    }

    // We can't easily run a full event loop in a headless test, 
    // but we can manually trigger events to verify logic.
    std::cout << "[TestHarness] Verifying Focus Traversal..." << std::endl;
    uxcpp::core::InputManager::getInstance().setRootWidget(root);
    uxcpp::core::InputManager::getInstance().handleKeyDown(9); // Tab
    // Check if input (tabIndex 0) is focused.
    if (input->isFocused()) {
        std::cout << "Focus Traversal: SUCCESS" << std::endl;
    } else {
        std::cout << "Focus Traversal: FAILURE" << std::endl;
    }

    // Test Shortcut Dispatch
    std::cout << "[TestHarness] Verifying Shortcut Dispatch..." << std::endl;
    if (uxcpp::core::ShortcutManager::getInstance().tryTriggerShortcut('Q', true, false, false)) {
        std::cout << "Shortcut Dispatch: SUCCESS" << std::endl;
    } else {
        std::cout << "Shortcut Dispatch: FAILURE" << std::endl;
    }

    return 0;
}
