# UXCpp User Guide

Welcome to the official user guide for **UXCpp**, a professional, clean-room C++20 UI framework designed as a permissive alternative to Qt. 

UXCpp provides a retained-mode widget hierarchy, high-performance OpenGL rendering, and a comprehensive suite of application services—all without requiring a meta-object compiler (MOC) or introducing copyleft dependencies.

---

## Table of Contents
1. [Architecture & Core Concepts](#architecture--core-concepts)
2. [UI Layouts & Control Catalog](#ui-layouts--control-catalog)
3. [Advanced Data & Graphics](#advanced-data--graphics)
4. [Application Services](#application-services)
5. [Developer Tools & Extensibility](#developer-tools--extensibility)
6. [Tutorial: Building a Production App](#tutorial-building-a-production-app)

---

## Architecture & Core Concepts

### Design Philosophy
UXCpp is built on the principle of **Zero-Boilerplate Modernity**. By leveraging C++20, it achieves reactive data binding and event handling through templates and functional primitives rather than code generation.

### Application Lifecycle
The `uxcpp::core::Application` singleton manages the global state and the main event loop.

```cpp
#include <UXCpp/Core/Application.h>
#include <UXCpp/UI/Widget.h>
#include <memory>

int main() {
    auto& app = uxcpp::core::Application::getInstance();
    
    if (!app.init()) {
        return -1;
    }

    // Application logic here...

    app.run(); // Blocks until quit() is called
    return 0;
}
```

### Retained-Mode Widget Hierarchy
UXCpp uses a retained-mode architecture where widgets are organized in a tree. Each `uxcpp::ui::Widget` manages its own geometry, style, and lifecycle.

**Component Lifecycle:**
1. `onInit()`: Called when the widget is added to the active tree.
2. `onUpdate(float deltaTime)`: Called every frame for logic/animations.
3. `onDraw(Renderer& renderer)`: Called during the render pass.

### Reactive Programming Model
UXCpp avoids MOC by using a type-safe **Signal** and **Property** system.

#### Signals & Slots
Signals allow objects to broadcast events to multiple observers.

```cpp
#include <UXCpp/Core/Signal.h>
#include <iostream>

struct MyWidget {
    uxcpp::core::Signal<int, std::string> onValueChanged;
};

// Usage
MyWidget widget;
widget.onValueChanged.connect([](int val, const std::string& text) {
    std::cout << "Value changed to " << val << ": " << text << std::endl;
});

widget.onValueChanged.emit(42, "Hello UXCpp");
```

#### Reactive Properties
`uxcpp::core::Property<T>` notifies observers automatically when its value changes.

```cpp
#include <UXCpp/Core/Property.h>
#include <iostream>

uxcpp::core::Property<float> opacity(1.0f);

opacity.observe([](const float& newVal) {
    std::cout << "Opacity updated to: " << newVal << std::endl;
});

opacity.set(0.5f); // Triggers observer
```

### Theme & Style System
Styling is decoupled from widget logic via the `Theme` singleton and `WidgetStyle` structures. Widgets are assigned a `styleClass` (e.g., "primary", "danger") which maps to a set of visual properties.

---

## UI Layouts & Control Catalog

### Layout Engine
UXCpp provides several layout managers that automatically calculate widget bounds:
- **BoxLayout**: Linear arrangement (Horizontal/Vertical).
- **GridLayout**: Tabular alignment based on rows and columns.
- **Splitter**: User-resizable panels.
- **ScrollArea**: Virtualized viewport for larger content.

### Standard Controls
| Control | Description | Key API |
| :--- | :--- | :--- |
| `Button` | Clickable trigger | `onClick()` signal |
| `Label` | Static or dynamic text | `setText(string)` |
| `TextInput` | Single-line text entry | `getText()`, `onTextChanged()` |
| `CheckBox` | Boolean toggle | `isChecked()`, `setChecked()` |
| `ComboBox` | Dropdown selection | `setItems(vector<string>)` |
| `Slider` | Range selector | `getValue()`, `setValue()` |

### Menus & Navigation
- **MenuBar**: Top-level application menu.
- **ContextMenu**: Right-click triggered popups via `Application::showNativeContextMenu`.
- **StatusBar**: Bottom-aligned status information.

### Dialogs & Overlays
UXCpp provides both framework-drawn overlays and native OS dialogs:
```cpp
auto& app = uxcpp::core::Application::getInstance();
app.showMessageBox("Alert", "Operation Successful!", uxcpp::core::MessageBoxType::Information);

std::string file = app.openFileDialog("Select Image", { "*.png", "*.jpg" });
```

---

## Advanced Data & Graphics

### Model/View Architecture
For large datasets, UXCpp uses a virtualized Model/View pattern to ensure $O(1)$ rendering complexity regardless of data size.

- **`AbstractItemModel`**: The base class for all data sources.
- **`VirtualTableView` / `VirtualListView`**: Only renders the visible rows/columns.
- **`ProxyModel`**: Allows non-destructive sorting and filtering of the underlying model.
- **`ItemDelegate`**: Customizes how individual cells are rendered (e.g., adding a button inside a table cell).

### Vector Graphics & Assets
UXCpp features a professional 2D graphics pipeline:
- **SVG Rendering**: Load and render `.svg` files via `SVGPipeline`.
- **Path API**: Draw complex shapes using Bézier curves (`moveTo`, `lineTo`, `quadraticCurveTo`).
- **Visual Polish**: Built-in support for rounded rectangles, linear gradients, and drop shadows.

### Charting & Plotting
The framework includes a high-performance charting subsystem:
- **Supported Types**: Line, Scatter, Bar, Area, Pie, and Candlestick charts.
- **Interactivity**: Built-in support for zooming and panning via `ChartWidget`.

### Code Editor & Rich Text
For text-heavy applications, UXCpp provides:
- **RichTextDocument**: A span-based model supporting inline styling and links.
- **CodeEditor**: A full-featured editor with caret management, selection bounds, and an undo/redo stack.

---

## Application Services

UXCpp includes a suite of asynchronous services to reduce dependence on third-party glue code.

### Async Networking
The `HttpClient` provides a non-blocking interface for REST APIs:
```cpp
#include <UXCpp/Services/HttpClient.h>

uxcpp::services::HttpClientImpl client;
auto future = client.get("https://api.example.com/status");
// ... later ...
auto response = future.get(); 
```

### Database Layer
`SqlDatabase` provides a thread-safe wrapper for SQL execution:
```cpp
uxcpp::services::SqlDatabaseImpl db;
db.open("connection_string");
auto result = db.executeQuery("SELECT * FROM users").get();
```

### OS Integration & Utilities
- **FileSystemWatcher**: Monitor directories for changes in real-time.
- **Process**: Launch and control child processes with pipe streams.
- **Settings**: Cross-platform configuration storage (INI/Registry).
- **SerialPort**: Low-level hardware communication.
- **I18n**: Runtime language switching via translation catalogs.

---

## Developer Tools & Extensibility

### Runtime Debug Overlay
Enable the debug overlay to inspect the UI hierarchy in real-time:
```cpp
#include <UXCpp/UI/DebugOverlay.h>
uxcpp::ui::DebugOverlay::getInstance().setEnabled(true);
```
This displays widget bounding boxes, unique IDs, and frame timing (FPS).

### Live Reload
The `LiveReloadEngine` allows developers to update declarative UI definitions without restarting the application.

### Plugin Engine
UXCpp supports dynamic extensibility via a stable ABI:
1. Implement the `IPlugin` interface.
2. Export a `CreatePlugin` function in your DLL/SO.
3. Load the module via `PluginManager::getInstance().loadPlugin("path/to/plugin.dll")`.

---

## Tutorial: Building a Production App

### Scenario: System Monitor & SQL Dashboard
This tutorial demonstrates how to combine multiple UXCpp modules into a single application.

```cpp
#include <UXCpp/Core/Application.h>
#include <UXCpp/UI/Widget.h>
#include <UXCpp/UI/BoxLayout.h>
#include <UXCpp/UI/ChartWidget.h>
#include <UXCpp/UI/VirtualTableView.h>
#include <UXCpp/Services/HttpClient.h>
#include <UXCpp/Services/SqlDatabase.h>
#include <UXCpp/Graphics/Charts.h>

class DashboardApp : public uxcpp::ui::Widget {
public:
    DashboardApp() : Widget("MainDashboard") {
        auto layout = std::make_shared<uxcpp::ui::BoxLayout>(uxcpp::ui::Orientation::Vertical);
        this->setLayout(layout);

        // 1. Add a Chart for CPU Usage
        auto cpuChart = std::make_unique<uxcpp::graphics::LineChart>();
        uxcpp::graphics::ChartSeries series;
        series.name = "CPU %";
        series.color = uxcpp::graphics::Color::Green();
        cpuChart->addSeries(series);

        auto chartWidget = std::make_shared<uxcpp::ui::ChartWidget>("CPUChart", std::move(cpuChart));
        layout->addChild(chartWidget);

        // 2. Add a Virtual Table for SQL Data
        auto table = std::make_shared<uxcpp::ui::VirtualTableView>("DataTable");
        // Assume we have a model that fetches from SqlDatabase
        // table->setModel(mySqlModel);
        layout->addChild(table);

        // 3. Async data fetch
        m_httpClient = std::make_unique<uxcpp::services::HttpClientImpl>();
        updateData();
    }

    void updateData() {
        auto fut = m_httpClient->get("https://api.monitor.local/stats");
        // In a real app, use a timer or callback to check the future
    }

private:
    std::unique_ptr<uxcpp::services::HttpClient> m_httpClient;
};

int main() {
    auto& app = uxcpp::core::Application::getInstance();
    app.init();

    auto dashboard = std::make_shared<DashboardApp>();
    app.addOverlayWidget(dashboard); // Simplified for demo

    app.run();
    return 0;
}
```