/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Core/Application.h>
#include <UXCpp/Windowing/Window.h>
#include <UXCpp/UI\Widget.h>
#include <UXCpp/UI\Controls.h>
#include <UXCpp/UI\Layout.h>
#include <iostream>

using namespace uxcpp;

int main() {
    auto& app = core::Application::getInstance();
    if (!app.init()) return -1;

    std::cout << "--- UXCpp Demo Application ---" << std::endl;

    // Create a layout container
    auto rootBox = std::make_shared<ui::Box>("RootBox", ui::LayoutOrientation::Vertical);
    rootBox->setBounds(0, 0, 400, 300);

    // Add some controls
    auto lbl = std::make_shared<ui::Label>("WelcomeLbl", "Welcome to UXCpp!");
    auto btn = std::make_shared<ui::Button>("SubmitBtn", "Click Me");

    rootBox->addChild(lbl);
    rootBox->addChild(btn);

    // Connect signal
    btn->onClicked.connect([]() {
        std::cout << "[Event] Button clicked! Signal received." << std::endl;
    });

    // Perform layout calculation
    rootBox->performLayout();

    std::cout << "UI Tree constructed successfully." << std::endl;
    std::cout << "Label position: " << lbl->getBounds().x << ", " << lbl->getBounds().y << std::endl;
    std::cout << "Button position: " << btn->getBounds().x << ", " << btn->getBounds().y << std::endl;

    // Simulate a click event
    ui::graphics::Point clickPos = {10, 40}; // Assuming button is at y=35
    if (btn->onPointerDown(clickPos)) {
        std::cout << "Simulated click handled by Button." << std::endl;
    }

    app.quit();
    return 0;
}
