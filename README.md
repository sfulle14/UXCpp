# UXCpp 🚀
**A high-performance, lightweight, cross-platform GUI framework for modern C++.**

[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-20/23-blue.svg)](https://en.cppreference.com/w/cpp/20)

UXCpp is a clean-room implementation of a professional GUI framework designed specifically for developers who need a powerful, permissive alternative to Qt. It provides the essential tools to build complex, hardware-accelerated user interfaces without the licensing constraints of LGPL/GPL.

## ✨ Key Features

### 🛠️ Core Architecture
- **MOC-less Reactive System**: Type-safe Signal/Slot mechanism using C++20 templates—no custom meta-object compiler required.
- **Hardware Accelerated**: Native OpenGL backend with a pluggable architecture designed for Vulkan, Metal, and Direct3D 12.
- **Retained Mode UI**: A robust widget tree hierarchy with efficient event propagation and lifecycle management.

### 🎨 Layout & Styling
- **Flexible Layout Engine**: Advanced `Box` and `Grid` layouts supporting proportional stretching, margins, and padding.
- **Decoupled Theming**: Global theme manager allowing for application-wide skinning via style classes.
- **Declarative UI**: Load complex interface structures from strings or external files using the `UILoader`.

### 🧩 Advanced Components
- **Composite Widgets**: Professional implementations of `ComboBox`, `TabWidget`, and `TreeView`.
- **Data Binding**: Reactive `Property<T>` system for automatic UI updates when underlying data changes.
- **Modal Systems**: Full support for overlay layers, popups, and blocking modal dialogs.

### 📈 Plotting
- **Line Charts**: A lightweight `PlotWidget` for trend graphs and metrics dashboards.
- **Auto-Scaling**: Axes adapt to the samples you add.
- **Legends and Grid Lines**: Included for readability out of the box.

### 📈 Plotting
- **Line Charts**: A lightweight `PlotWidget` for time series and trend graphs.
- **Auto-Scaling**: Axes adjust to the data points you add.
- **Legends and Grid Lines**: Built in for dashboard-style visualizations.

### 📈 Plotting
- **Line Charts**: A lightweight `PlotWidget` for series-based line plots.
- **Auto-Scaling**: Axes adjust to the data you add.
- **Legends and Grid Lines**: Built-in chart readability helpers for dashboards and metrics views.

## 🚀 Quick Start

```cpp
#include <UXCpp/Core/Application.h>
#include <UXCpp/UI/Controls.h>
#include <UXCpp/UI/Layout.h>

int main() {
    auto& app = uxcpp::core::Application::getInstance();
    app.init();

    // Create a vertical layout
    auto root = std::make_shared<uxcpp::ui::Box>("Root", uxcpp::ui::LayoutOrientation::Vertical);
    root->setBounds(0, 0, 400, 300);

    // Add a styled button
    auto btn = std::make_shared<uxcpp::ui::Button>("MyBtn", "Click Me!");
    btn->onClicked.connect([]() { 
        std::cout << "Hello from UXCpp!" << std::endl; 
    });

    root->addChild(btn);
    root->performLayout();

    app.run();
    return 0;
}
```

## 🏗️ Build & Installation

### Prerequisites
- **CMake** 3.22+
- **C++20** compatible compiler (MSVC 2019+, GCC 10+, Clang 10+)
- **GLFW** & **OpenGL** development libraries

### Building from Source
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## ⚖️ Licensing
UXCpp is released under the **MIT License**. It is designed for use in both open-source and proprietary commercial software.

---
*Designed for performance. Built for freedom.*
