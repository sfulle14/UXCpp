# Section 5: Developer Tools & Extensibility

UXCpp is designed to be extended both at runtime via plugins and during development using built-in diagnostic tools.

## Runtime Debug Overlay

The `uxcpp::ui::DebugOverlay` provides a real-time visual inspector for the widget tree, allowing developers to debug layout issues without leaving the application.

### Enabling the Overlay
```cpp
#include <UXCpp/UI/DebugOverlay.h>

// Enable the overlay globally
uxcpp::ui::DebugOverlay::getInstance().setEnabled(true);
```

### Features of the Debug Overlay
- **Bounds Inspector**: Draws green bounding boxes around every widget in the hierarchy.
- **Widget Metadata**: Displays the Widget ID and its current dimensions (e.g., `MainPanel [800x600]`) above each box.
- **FPS Counter**: A real-time frame timing display in the top-left corner to monitor rendering performance.
- **Highlighting**: Use `setHighlightedWidget(widget)` to mark a specific component in red for easier tracking.

## Plugin Engine

UXCpp supports dynamic extensibility through a stable Plugin ABI. This allows third parties to add new widgets or services without access to the core source code.

### Writing a Custom Plugin
To create a plugin, implement the `uxcpp::plugins::IPlugin` interface:

```cpp
#include <UXCpp/Plugins/IPlugin.h>
#include <iostream>

class MyCustomPlugin : public uxcpp::plugins::IPlugin {
public:
    std::string getName() const override { return "AdvancedChartsPlugin"; }
    std::string getVersion() const override { return "1.0.0"; }

    void onInitialize() override {
        std::cout << "Custom Plugin Initialized!" << std::endl;
        // Register new widgets or services here
    }

    void onShutdown() override {
        std::cout << "Custom Plugin Shutting Down..." << std::endl;
    }
};

// Entry point for the DLL/Shared Library
extern "C" uxcpp::plugins::IPlugin* create_plugin() {
    return new MyCustomPlugin();
}
```

### Loading Plugins at Runtime
The `uxcpp::plugins::PluginManager` handles the loading and lifecycle of dynamic modules.

```cpp
#include <UXCpp/Plugins/PluginManager.h>

auto& pm = uxcpp::plugins::PluginManager::getInstance();

// Load a plugin from a file path
if (pm.loadPlugin("./plugins/advanced_charts.dll")) {
    std::cout << "Successfully loaded AdvancedChartsPlugin" << std::endl;
}

// Unload when no longer needed
pm.unloadPlugin("./plugins/advanced_charts.dll");
```

## Live Reloading

UXCpp includes a `LiveReloadEngine` that can monitor UI definition files (such as JSON or XML) and automatically rebuild the widget tree when changes are detected on disk, without restarting the application. This significantly accelerates the UI iteration loop.
