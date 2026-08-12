
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 

# 📘 UXCpp Comprehensive Developer's Guide

Welcome to the exhaustive documentation for **UXCpp**. This guide is designed to take you from a basic "Hello World" to building complex, professional-grade user interfaces.

---

## 📑 Table of Contents
1. [Introduction & Architecture](#1-introduction--architecture)
2. [The Application Lifecycle](#2-the-application-lifecycle)
3. [The Widget System](#3-the-widget-system)
4. [Event Handling: Signals & Slots](#4-event-handling-signals--slots)
5. [Layout Management](#5-layout-management)
6. [Reactive Data Binding (Properties)](#6-reactive-data-binding-properties)
7. [Advanced UI Components](#7-advanced-ui-components)
8. [Styling & Theming](#8-styling--theming)
9. [Declarative UI Loading](#9-declarative-ui-loading)
10. [Creating Custom Widgets](#10-creating-custom-widgets)

---

## 1. Introduction & Architecture

### What is UXCpp?
UXCpp is a **Retained Mode** GUI framework. Unlike Immediate Mode GUIs (like Dear ImGui), UXCpp maintains a persistent tree of widget objects. This allows for:
- **State Persistence**: Widgets remember their state (e.g., text in an input box) between frames.
- **Efficient Updates**: Only "dirty" regions can be redrawn (though the current implementation redraws the frame).
- **Complex Layouts**: The framework can calculate positions once and reuse them until a resize occurs.

### Core Pipeline
`OS Events` $\rightarrow$ `GLFW/Window` $\rightarrow$ `InputManager` $\rightarrow$ `Widget Tree` $\rightarrow$ `Renderer (OpenGL)` $\rightarrow$ `GPU`.

---

## 2. The Application Lifecycle

The `Application` class is a singleton that manages the global state and the main loop.

### Basic Setup
```cpp
#include <UXCpp/Core/Application.h>

int main() {
    auto& app = uxcpp::core::Application::getInstance();
    
    // Initialize backends (GLFW, OpenGL)
    if (!app.init()) return -1;

    // Setup your UI here...

    // Start the blocking event loop
    app.run(); 
    return 0;
}
```

---

## 3. The Widget System

### The Widget Tree
Every element in UXCpp inherits from `Widget`. Widgets are managed via `std::shared_ptr` to ensure safe lifetime management across the tree.

**Key Concepts:**
- **Parent/Child**: Use `addChild()` to build your hierarchy.
- **Bounds**: Every widget has a `Rect` (x, y, width, height).
- **Z-Order**: Children added later are rendered on top of earlier children.

### Geometry Management
You can set bounds manually:
```cpp
myWidget->setBounds(10, 10, 200, 50);
```
However, it is highly recommended to use **Layout Managers** (see Section 5) to handle positioning automatically.

---

## 4. Event Handling: Signals & Slots

UXCpp replaces the need for a Meta-Object Compiler (MOC) with C++20 variadic templates.

### Connecting to a Signal
Signals are members of widgets. You can connect any callable (lambda, function pointer, or `std::function`) to them.

```cpp
auto btn = std::make_shared<uxcpp::ui::Button>("SubmitBtn", "Send");

// Connect a lambda
btn->onClicked.connect([]() {
    std::cout << "Submitted!" << std::endl;
});
```

### Signal Parameters
Signals can pass data. For example, a `Slider` might have a signal like `Signal<float> onValueChanged`.
```cpp
slider->onValueChanged.connect([](float val) {
    std::cout << "Value is now: " << val << std::endl;
});
```

---

## 5. Layout Management

Layouts automate the positioning of widgets based on constraints rather than hardcoded pixels.

### Box Layout (`Box`)
The `Box` widget arranges children linearly.
- **Vertical**: Children are stacked top-to-bottom.
- **Horizontal**: Children are placed left-to-right.

#### The Stretch Factor
The `stretch` property in `LayoutParams` determines how a widget grows to fill available space:
- `stretch = 0`: Fixed size (uses `preferredWidth`/`preferredHeight`).
- `stretch > 0`: Proportional growth. If Widget A has stretch 1 and Widget B has stretch 2, Widget B will take twice as much of the remaining space as Widget A.

```cpp
auto root = std::make_shared<uxcpp::ui::Box>("Root", uxcpp::ui::LayoutOrientation::Vertical);

auto header = std::make_shared<uxcpp::ui::Label>("Hdr", "Header");
header->layoutParams().preferredHeight = 50.0f; // Fixed height

auto body = std::make_shared<uxcpp::ui::Widget>("Body");
body->layoutParams().stretch = 1.0f; // Fills remaining space

root->addChild(header);
root->addChild(body);
root->performLayout();
```

### Grid Layout (`GridLayout`)
Used for tabular or dashboard-style interfaces.
```cpp
auto grid = std::make_shared<uxcpp::ui::GridLayout>("Grid", 2, 2); // 2 rows, 2 cols
grid->addWidget(widgetA, 0, 0); // Row 0, Col 0
grid->addWidget(widgetB, 0, 1); // Row 0, Col 1
```

---

## 6. Reactive Data Binding (Properties)

The `Property<T>` class allows you to decouple your data from your UI.

### How it Works
Instead of manually updating a label when a variable changes, the label "observes" the property.

```cpp
// 1. Define a reactive property
uxcpp::core::Property<int> score(0);

// 2. Create a widget that observes this property
auto progress = std::make_shared<uxcpp::ui::ProgressBar>("ScoreBar", &score);

// 3. Update the value anywhere in your code
score.set(50); // The ProgressBar updates automatically!
```

---

## 7. Advanced UI Components

### Modal Dialogs
Dialogs are special widgets that live in the **Overlay Layer**. When shown, they block all input to the rest of the application.

```cpp
auto dlg = std::make_shared<uxcpp::ui::Dialog>("ConfirmDlg", "Are you sure?");
dlg->setContent(myContent);
dlg->show(); // Blocks background input
```

### TableViews & TreeViews
These widgets use a **Model-View** architecture. You provide a `TableModel` or `TreeModel`, and the widget handles the rendering of thousands of items efficiently.

---

## 8. Styling & Theming

UXCpp uses a global theme system to avoid hardcoding colors in every widget.

### Defining a Style
```cpp
uxcpp::ui::WidgetStyle dangerStyle;
dangerStyle.backgroundColor = uxcpp::graphics::Color::Red();
dangerStyle.foregroundColor = uxcpp::graphics::Color::White();

uxcpp::ui::Theme::getInstance().setStyle("danger", dangerStyle);
```

### Applying a Style
Widgets are assigned a style class upon creation:
```cpp
auto btn = std::make_shared<uxcpp::ui::Button>("DeleteBtn", "Delete");
btn->setStyleClass("danger"); // Now uses the red/white theme
```

---

## 9. Declarative UI Loading

You can define your UI using a simple string format, which is ideal for loading layouts from files.

**Format:** `[Type, "Name", Args...] { children }`

```cpp
std::string myUI = 
    "[Box, \"Main\", \"Vertical\"] {"
    "  [Label, \"Title\", \"Welcome to UXCpp\"]"
    "  [Button, \"Btn1\", \"Click Here\"]"
    "}";

auto root = uxcpp::core::UILoader::loadFromString(myUI);
```

---

## 10. Creating Custom Widgets

To create your own widget, inherit from `Widget` and override the necessary methods.

### Example: A Simple Color Picker
```cpp
class ColorPicker : public uxcpp::ui::Widget {
public:
    ColorPicker(std::string name) : Widget(name, "colorpicker") {}

    void onDraw(uxcpp::graphics::Renderer& renderer) override {
        // Draw a custom color square
        renderer->drawRect(m_bounds, m_currentColor);
    }

    bool onPointerDown(uxcpp::graphics::Point p) override {
        // Logic to change color based on click position...
        return true;
    }
private:
    uxcpp::graphics::Color m_currentColor = uxcpp::graphics::Color::White();
};
```

---

## 🛠️ Troubleshooting & FAQ

**Q: My widget is not appearing!**
A: Check if you added it to a parent and called `performLayout()` on the root container. Also, ensure its bounds are within the window size.

**Q: How do I handle window resizing?**
A: In your main loop, check if the window size changed, then call `root->setBounds(...)` and `root->performLayout()`.

---
*UXCpp - Built for performance, designed for freedom.*
