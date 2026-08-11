/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

# UXCpp User Guide & Documentation

Welcome to the official documentation for **UXCpp**. This guide provides a comprehensive overview of how to integrate and use the framework in your C++ projects.

---

## 1. Getting Started

### Installation
Ensure you have CMake 3.22+ and a C++20 compiler installed. You will also need the GLFW and OpenGL development libraries on your system.

### Basic Application Lifecycle
Every UXCpp application starts with the `Application` singleton.

```cpp
#include <UXCpp/Core/Application.h>

int main() {
    auto& app = uxcpp::core::Application::getInstance();
    if (app.init()) {
        app.run();
    }
    return 0;
}
```

---

## 2. The Widget System

### Hierarchy & Management
UXCpp uses a **Retained Mode** UI tree. Every element is a `Widget`.
- **Adding Children**: Use `widget->addChild(child)`.
- **Geometry**: Set positions and sizes using `widget->setBounds(x, y, w, h)`.

### Event Handling (Signals & Slots)
UXCpp uses a type-safe signal system. No MOC is required.

```cpp
auto btn = std::make_shared<uxcpp::ui::Button>("MyBtn", "Click Me");
btn->onClicked.connect([]() {
    std::cout << "Button was clicked!" << std::endl;
});
```

---

## 3. Layout Management

### Box Layouts
The `Box` widget allows for linear arrangements of children.
- **Orientation**: `Vertical` or `Horizontal`.
- **Stretch Factors**: Set `layoutParams().stretch = 1.0f` to make a widget expand to fill available space.
- **Margins & Padding**: Use `layoutParams().margin` and `layoutParams().padding` for spacing.

### Grid Layouts
The `GridLayout` allows for precise cell-based positioning:
```cpp
auto grid = std::make_shared<uxcpp::ui::GridLayout>("MainGrid", 2, 2); // 2x2 grid
grid->addWidget(myWidget, 0, 0); // Row 0, Col 0
```

---

## 4. Advanced Features

### Reactive Property Binding
Use `core::Property<T>` to link data sources directly to UI elements.
```cpp
core::Property<float> health(100.0f);
auto bar = std::make_shared<uxcpp::ui::ProgressBar>("HealthBar", &health);

// Changing the property automatically updates the ProgressBar
health.set(80.0f); 
```

### Modal Dialogs
Create popups that block interaction with the background:
```cpp
auto dialog = std::make_shared<uxcpp::ui::Dialog>("MyDialog", "Warning");
dialog->setContent(myContentWidget);
dialog->show(); // Adds to global overlay layer and blocks input
```

### Declarative UI Loading
Define your UI in a string format for rapid prototyping:
```cpp
std::string uiDef = "[Box, \"Root\", \"Vertical\"] { [Button, \"Btn1\", \"Submit\"] }";
auto root = uxcpp::core::UILoader::loadFromString(uiDef);
```

---

## 5. Styling & Theming
UXCpp uses a global `Theme` manager. You can define styles for different classes of widgets:

```cpp
uxcpp::ui::WidgetStyle btnStyle;
btnStyle.backgroundColor = uxcpp::graphics::Color::Blue();
btnStyle.foregroundColor = uxcpp::graphics::Color::White();

uxcpp::ui::Theme::getInstance().setStyle("button", btnStyle);
```

---

## 6. Troubleshooting & FAQ

**Q: Why is my window not appearing?**
A: Ensure you have called `app.init()` and that your OpenGL drivers are up to date.

**Q: How do I handle custom events?**
A: Override the `onPointerDown` or `onKeyDown` methods in a custom class inheriting from `Widget`.

---
*This document is designed to be exported as a PDF via Markdown tools.*
