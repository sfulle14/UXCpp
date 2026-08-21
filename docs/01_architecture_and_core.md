# Section 1: Architecture & Core Concepts

Welcome to the UXCpp Framework. UXCpp is a modern, high-performance C++20 GUI framework designed for developers who require absolute control over their application's visual and behavioral logic without the overhead of proprietary pre-compilers or complex build steps.

## Design Philosophy

UXCpp follows a **clean-room, permissive design philosophy**. Unlike many traditional C++ frameworks, UXCpp:
- **Requires no MOC (Meta-Object Compiler):** All reactivity and signal-slot mechanisms are implemented using standard C++20 templates and functional primitives.
- **Retained-Mode UI:** The framework maintains a persistent widget tree, allowing for efficient updates and complex layout calculations.
- **C++20 Native:** Leverages modern language features (concepts, `std::future`, `std::shared_ptr`) to provide a type-safe and ergonomic API.

## Application Lifecycle

The entry point of every UXCpp application is the `uxcpp::core::Application` singleton. It manages the global event loop, windowing backends, and system-level services.

### Basic Startup Sequence
To start an application, you must initialize the singleton and enter the main execution loop:

```cpp
#include <UXCpp/Core/Application.h>
#include <UXCpp/UI/Widget.h>
#include <iostream>

int main() {
    auto& app = uxcpp::core::Application::getInstance();

    if (!app.init()) {
        std::cerr << "Failed to initialize UXCpp Application" << std::endl;
        return -1;
    }

    // Setup your UI here...

    app.run(); // Blocks until app.quit() is called
    return 0;
}
```

### Key Lifecycle Methods
- `init()`: Initializes the graphics backend (OpenGL) and system services.
- `run()`: Starts the main event loop, processing input events and triggering redraws.
- `quit()`: Signals the event loop to terminate gracefully.

## Widget Hierarchy & Component Lifecycle

UXCpp uses a **Retained-Mode Widget Hierarchy**. Every UI element inherits from `uxcpp::ui::Widget`.

### The Widget Tree
Widgets are organized in a parent-child relationship. A root widget (usually a Window or a Main Container) holds children, which may hold their own children, forming a tree.

```cpp
auto root = std::make_shared<uxcpp::ui::Box>("Root", uxcpp::ui::LayoutOrientation::Vertical);
auto btn = std::make_shared<uxcpp::ui::Button>("SubmitBtn", "Click Me");

root->addChild(btn); 
```

### Component Lifecycle Hooks
Developers can override several virtual methods to customize widget behavior:
- `onInit()`: Called once when the widget is first created and added to the tree.
- `onUpdate(float deltaTime)`: Called every frame; ideal for animations or polling state.
- `onDraw(graphics::Renderer& renderer)`: The primary rendering hook where you define how the widget looks using the `Renderer` API.

## Reactive Programming Model

UXCpp implements a powerful reactive model to decouple components, avoiding tight coupling between UI elements and business logic.

### Signals and Slots
The `uxcpp::core::Signal<Args...>` class allows objects to emit events that multiple observers (slots) can listen to.

```cpp
#include <UXCpp/Core/Signal.h>
#include <iostream>

struct UserProfile {
    uxcpp::core::Signal<int> onAgeChanged;
};

void onAgeUpdate(int newAge) {
    std::cout << "Age updated to: " << newAge << std::endl;
}

// Usage
UserProfile profile;
profile.onAgeChanged.connect(onAgeUpdate);
profile.onAgeChanged.emit(25); // Triggers onAgeUpdate
```

### Reactive Properties
For state management, UXCpp provides `uxcpp::core::Property<T>`. A property notifies all registered observers whenever its value changes.

```cpp
#include <UXCpp/Core/Property.h>
#include <iostream>

uxcpp::core::Property<float> health(100.0f);

health.observe([](const float& val) {
    std::cout << "Health is now: " << val << std::endl;
});

health.set(85.0f); // Automatically triggers the observer
```

## Theme & Style System

Visual consistency is managed through a CSS-like styling system. Instead of hardcoding colors, widgets reference **Style Classes**.

### Widget Styles
A `uxcpp::ui::WidgetStyle` defines properties like background color, foreground color, and border radius.

### The Theme Manager
The `uxcpp::ui::Theme` singleton stores the mapping between class names (e.g., `"button"`, `"label"`) and their corresponding styles.

```cpp
#include <UXCpp/UI/Style.h>

uxcpp::ui::WidgetStyle btnStyle;
btnStyle.backgroundColor = uxcpp::graphics::Color::Blue();
btnStyle.foregroundColor = uxcpp::graphics::Color::White();

uxcpp::ui::Theme::getInstance().setStyle("primary-button", btnStyle);
```

### Applying Styles to Widgets
Widgets are assigned a style class upon construction or via `setStyleClass()`. The widget then queries the theme during its `onDraw` call:

```cpp
auto myBtn = std::make_shared<uxcpp::ui::Button>("MyBtn", "Submit");
myBtn->setStyleClass("primary-button"); 
// Inside onDraw, getStyle() will return the 'primary-button' style.
```
