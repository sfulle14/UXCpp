# UXCpp User Guide

Welcome to the official user guide for **UXCpp**, a professional, clean-room C++20 UI framework designed as a permissive alternative to traditional GUI libraries. 

UXCpp provides a retained-mode widget hierarchy, high-performance OpenGL rendering, and a comprehensive suite of application services—all without requiring a meta-object compiler (MOC) or introducing copyleft dependencies.

---

## Table of Contents
1. [Architecture & Core Concepts](01_architecture_and_core.md)
2. [UI Layouts & Control Catalog](02_layouts_controls_and_dialogs.md)
3. [Advanced Data & Graphics](03_data_graphics_and_editors.md)
4. [Application Services](04_application_services.md)
5. [Developer Tools & Extensibility](05_developer_tools_and_plugins.md)
6. [Tutorial: Building a Production App](06_complete_tutorial.md)

---

For the most up-to-date index and quick start guide, please refer to [README.md](README.md).
# UXCpp Master User Guide & Developer Manual

Welcome to the official comprehensive manual for **UXCpp**, a professional, clean-room C++20 UI framework designed as a permissive alternative to traditional GUI libraries. 

UXCpp provides a retained-mode widget hierarchy, high-performance OpenGL rendering, and a comprehensive suite of application services—all without requiring a meta-object compiler (MOC) or introducing copyleft dependencies.

---

## 1. Architecture & Core Concepts

### Design Philosophy
UXCpp follows a **clean-room, permissive design philosophy**. Unlike many traditional C++ frameworks, UXCpp:
- **Requires no MOC (Meta-Object Compiler):** All reactivity and signal-slot mechanisms are implemented using standard C++20 templates and functional primitives.
- **Retained-Mode UI:** The framework maintains a persistent widget tree, allowing for efficient updates and complex layout calculations.
- **C++20 Native:** Leverages modern language features (concepts, `std::future`, `std::shared_ptr`) to provide a type-safe and ergonomic API.

### Application Lifecycle
The entry point of every UXCpp application is the `uxcpp::core::Application` singleton. It manages the global event loop, windowing backends, and system-level services.

#### Basic Startup Sequence
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

#### Key Lifecycle Methods
- `init()`: Initializes the graphics backend (OpenGL) and system services.
- `run()`: Starts the main event loop, processing input events and triggering redraws.
- `quit()`: Signals the event loop to terminate gracefully.

### Widget Hierarchy & Component Lifecycle
UXCpp uses a **Retained-Mode Widget Hierarchy**. Every UI element inherits from `uxcpp::ui::Widget`.

#### The Widget Tree
Widgets are organized in a parent-child relationship. A root widget (usually a Window or a Main Container) holds children, which may hold their own children, forming a tree.

```cpp
auto root = std::make_shared<uxcpp::ui::Box>("Root", uxcpp::ui::LayoutOrientation::Vertical);
auto btn = std::make_shared<uxcpp::ui::Button>("SubmitBtn", "Click Me");

root->addChild(btn); 
```

#### Component Lifecycle Hooks
Developers can override several virtual methods to customize widget behavior:
- `onInit()`: Called once when the widget is first created and added to the tree.
- `onUpdate(float deltaTime)`: Called every frame; ideal for animations or polling state.
- `onDraw(graphics::Renderer& renderer)`: The primary rendering hook where you define how the widget looks using the `Renderer` API.

### Reactive Programming Model
UXCpp implements a powerful reactive model to decouple components, avoiding tight coupling between UI elements and business logic.

#### Signals and Slots
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

#### Reactive Properties
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

### Theme & Style System
Visual consistency is managed through a CSS-like styling system. Instead of hardcoding colors, widgets reference **Style Classes**.

#### Widget Styles
A `uxcpp::ui::WidgetStyle` defines properties like background color, foreground color, and border radius.

#### The Theme Manager
The `uxcpp::ui::Theme` singleton stores the mapping between class names (e.g., `"button"`, `"label"`) and their corresponding styles.

```cpp
#include <UXCpp/UI/Style.h>

uxcpp::ui::WidgetStyle btnStyle;
btnStyle.backgroundColor = uxcpp::graphics::Color::Blue();
btnStyle.foregroundColor = uxcpp::graphics::Color::White();

uxcpp::ui::Theme::getInstance().setStyle("primary-button", btnStyle);
```

#### Applying Styles to Widgets
Widgets are assigned a style class upon construction or via `setStyleClass()`. The widget then queries the theme during its `onDraw` call:

```cpp
auto myBtn = std::make_shared<uxcpp::ui::Button>("MyBtn", "Submit");
myBtn->setStyleClass("primary-button"); 
// Inside onDraw, getStyle() will return the 'primary-button' style.
```
$content = @'
---

## 2. UI Layouts, Control Catalog & Dialogs

UXCpp provides a comprehensive set of layout managers and standard controls to build professional interfaces quickly. All controls are built upon the `uxcpp::ui::Widget` base class.

### Layout Engine
The layout engine automates the positioning and sizing of widgets, removing the need for manual coordinate calculation.

#### Box Layout (`uxcpp::ui::Box`)
The `Box` widget is the primary tool for linear arrangements. It supports both vertical and horizontal orientations.

```cpp
#include <UXCpp/UI/Layout.h>
#include <UXCpp/UI/Controls.h>

// Create a vertical container
auto layout = std::make_shared<uxcpp::ui::Box>("MainLayout", uxcpp::ui::LayoutOrientation::Vertical);

auto label = std::make_shared<uxcpp::ui::Label>("Title", "Welcome to UXCpp");
auto btn = std::make_shared<uxcpp::ui::Button>("OkBtn", "OK");

layout->addChild(label);
layout->addChild(btn);
```

#### Understanding `LayoutParams`
Every widget has a `LayoutParams` object that tells the parent layout how to treat it:
- **Stretch**: A value $> 0$ indicates the widget should expand to fill available space proportionally.
- **Preferred Width/Height**: Fixed dimensions for the widget.
- **Margin & Padding**: Spacing around and inside the widget.

```cpp
auto searchBar = std::make_shared<uxcpp::ui::TextInput>("Search", "");
searchBar->layoutParams().stretch = 1.0f; // Expand to fill horizontal space
searchBar->layoutParams().margin = {5, 5, 5, 5}; // Add 5px margin on all sides
```

### Standard Controls Catalog
| Control | Class | Description | Key Methods |
| :--- | :--- | :--- | :--- |
| **Label** | `uxcpp::ui::Label` | Static or dynamic text display. | `setText(string)` |
| **Button** | `uxcpp::ui::Button` | Clickable trigger for actions. | `onClicked` (Signal) |
| **TextInput** | `uxcpp::ui::TextEditor` | Single/Multi-line editable text. | `getText()`, `setText()` |
| **CheckBox** | `uxcpp::ui::CheckBox` | Binary toggle state. | `setChecked(bool)`, `isChecked()` |
| **ComboBox** | `uxcpp::ui::ComboBox` | Dropdown selection list. | `selectItem(int)` |
| **ProgressBar** | `uxcpp::ui::ProgressBar` | Visual progress indicator. | `setProgress(float)` |

#### Example: Creating a Simple Form
```cpp
#include <UXCpp/UI/Layout.h>
#include <UXCpp/UI/Controls.h>

auto form = std::make_shared<uxcpp::ui::Box>("Form", uxcpp::ui::LayoutOrientation::Vertical);

auto nameLabel = std::make_shared<uxcpp::ui::Label>("NameLbl", "Enter Name:");
auto nameInput = std::make_shared<uxcpp::ui::TextEditor>("NameInp");

auto submitBtn = std::make_shared<uxcpp::ui::Button>("SubmitBtn", "Save");
submitBtn->onClicked.connect([]() {
    // Handle submission
});

form->addChild(nameLabel);
form->addChild(nameInput);
form->addChild(submitBtn);
```

### Menus & Navigation
UXCpp provides a native-feeling menu system that handles overlays and popups automatically via the `Application` singleton.

#### MenuBar & MenuPopup
The `MenuBar` sits at the top of the window, while `MenuPopup` contains the actual items.

```cpp
#include <UXCpp/UI/Menu.h>

auto menuBar = std::make_shared<uxcpp::ui::MenuBar>("MainMenuBar");

// Create a "File" menu
auto fileMenu = menuBar->addMenu("File");
fileMenu->addItem({"New", []() { /* New File Logic */ }});
fileMenu->addItem({"Open", []() { /* Open File Logic */ }});
fileMenu->addItem({"Exit", []() { uxcpp::core::Application::getInstance().quit(); }});

// Create an "Edit" menu
auto editMenu = menuBar->addMenu("Edit");
editMenu->addItem({"Undo", []() { /* Undo Logic */ }});
```

#### Toolbars & Status Bars
- **`uxcpp::ui::Toolbar`**: A horizontal bar for quick-access icons. Use `addButton(iconId, action)` to populate it.
- **`uxcpp::ui::StatusBar`**: A thin strip at the bottom of the window for status messages. Use `setMessage(string)` to update text.

### Dialogs & Overlays
#### Modal Dialogs (`uxcpp::ui::Dialog`)
Dialogs are overlay widgets that dim the background and focus user attention on a specific task.

```cpp
#include <UXCpp/UI/Dialog.h>
#include <UXCpp/UI/Controls.h>

auto dialog = std::make_shared<uxcpp::ui::Dialog>("ConfirmDlg", "Are you sure?");
auto content = std::make_shared<uxcpp::ui::Box>("DlgContent", uxcpp::ui::LayoutOrientation::Vertical);

auto okBtn = std::make_shared<uxcpp::ui::Button>("OkBtn", "Yes");
okBtn->onClicked.connect([dialog]() { dialog->close(); });

content->addChild(okBtn);
dialog->setContent(content);

dialog->show(); // Adds to Application overlay list
```

#### Native System Dialogs
For common OS tasks, use the `Application` singleton directly:
- `app.openFileDialog(title, filters)`: Opens a native file picker.
- `app.saveFileDialog(title, filters)`: Opens a native save dialog.
- `app.showMessageBox(title, message, type)`: Displays a system alert.
---

## 3. Advanced Data, Graphics & Editor Widgets

UXCpp provides high-level abstractions for handling large datasets, complex vector graphics, and real-time data visualization.

### Model/View Architecture
To prevent UI freezing when dealing with thousands of items, UXCpp uses a decoupled Model/View architecture. The View only requests the data it needs to render currently visible cells.

#### Table Models (`uxcpp::core::TableModel`)
The `TableModel` is an abstract interface that defines how data is accessed.

```cpp
#include <UXCpp/Core/TableModel.h>

class MyDataModel : public uxcpp::core::TableModel {
public:
    int rowCount() const override { return 1000; }
    int columnCount() const override { return 3; }
    std::string data(int row, int col) const override {
        return "Cell " + std::to_string(row) + ":" + std::to_string(col);
    }
    std::string headerData(int section, bool isRow) const override {
        return (isRow) ? "" : "Column " + std::to_string(section);
    }
};
```

#### TableView & Virtualization
The `uxcpp::ui::TableView` widget renders the model. It supports virtualization, meaning it only draws the rows currently visible on screen.

```cpp
#include <UXCpp/UI/TableView.h>

auto table = std::make_shared<uxcpp::ui::TableView>("DataTable");
auto model = std::make_shared<MyDataModel>();
table->setModel(model);
```

#### Proxy Models for Sorting & Filtering
The `uxcpp::core::TableProxyModel` wraps an existing model to provide sorting and filtering without modifying the original data source.

```cpp
#include <UXCpp/Core/TableProxyModel.h>

auto baseModel = std::make_shared<MyDataModel>();
auto proxyModel = std::make_shared<uxcpp::core::TableProxyModel>(baseModel);

proxyModel->setFilter("Search Term"); // Only rows containing "Search Term" are shown
proxyModel->setSortColumn(1, true);    // Sort by column 1 ascending

table->setModel(proxyModel);
```

### Vector Graphics & Assets
UXCpp uses a modern rendering pipeline that supports high-fidelity vector graphics and dynamic canvas drawing.

#### The Renderer API
The `uxcpp::graphics::Renderer` provides the primitives needed to draw custom widgets:
- `drawRect(rect, color)`: Fills a rectangle.
- `drawLine(p1, p2, color)`: Draws a line between two points.
- `drawText(point, text, color)`: Renders text using the current font manager.
- `drawFocusRing(rect, color)`: Draws a stylized border for focused elements.

#### SVG Rendering
UXCpp supports SVG assets via the `uxcpp::graphics::SVGAsset` class, allowing for resolution-independent icons and illustrations.

### Charting & Plotting
For scientific and financial data visualization, UXCpp includes the `PlotWidget`.

#### Using the PlotWidget
The `PlotWidget` supports multiple series, auto-scaling, and customizable axes.

```cpp
#include <UXCpp/UI/PlotWidget.h>

auto plot = std::make_shared<uxcpp::ui::PlotWidget>("CPU Usage");
plot->setTitle("System CPU Load (%)");
plot->setShowGrid(true);

// Add a data series
auto& series = plot->addSeries("Core 0", uxcpp::graphics::Color::Blue());
series.addSample(0.0f, 12.5f);
series.addSample(1.0f, 45.0f);
series.addSample(2.0f, 30.2f);

// Add a second series for comparison
auto& series2 = plot->addSeries("Core 1", uxcpp::graphics::Color::Red());
series2.addSample(0.0f, 5.0f);
series2.addSample(1.0f, 20.0f);
series2.addSample(2.0f, 15.8f);
```

### Code Editor & Rich Text
For applications requiring text manipulation, UXCpp provides a professional-grade `TextEditor`.

#### The TextEditor Widget
The `uxcpp::ui::TextEditor` supports:
- **Caret Navigation**: Precise control over the insertion point.
- **Clipboard Integration**: Native copy/paste via `Application::getClipboardText()`.
- **Selection Handling**: Ability to highlight and manipulate text blocks.

```cpp
#include <UXCpp/UI/TextEditor.h>

auto editor = std::make_shared<uxcpp::ui::TextEditor>("CodeEditor");
editor->setText("// Welcome to UXCpp\nint main() { return 0; }");
```
---

## 4. Application Services & OS Integration

UXCpp provides a suite of cross-platform services for OS integration, networking, and data persistence. All services are designed as interfaces to allow for easy mocking during testing.

### Async Networking (`HttpClient`)
The `uxcpp::services::HttpClient` allows applications to perform non-blocking REST requests using C++20 futures.

#### Performing a GET Request
```cpp
#include <UXCpp/Services/HttpClient.h>
#include <iostream>

auto client = std::make_shared<uxcpp::services::HttpClientImpl>();
std::future<uxcpp::services::HttpResponse> responseFuture = client->get("https://api.example.com/data");

// In a real app, you would check the future in onUpdate() or use a callback
auto response = responseFuture.get(); 
if (response.statusCode == 200) {
    std::cout << "Data received: " << response.body << std::endl;
}
```

### Database Access Layer (`SqlDatabase`)
UXCpp provides a unified interface for SQL databases, supporting connection pooling and asynchronous query execution.

#### Basic Database Operations
```cpp
#include <UXCpp/Services/SqlDatabase.h>

auto db = std::make_shared<uxcpp::services::SqlDatabaseImpl>();
if (db->open("Server=myServer;Database=myDB;")) {
    // Execute a SELECT query asynchronously
    auto resultFuture = db->executeQuery("SELECT * FROM Users WHERE Active = 1");
    auto result = resultFuture.get();

    if (result.success) {
        for (const auto& row : result.rows) {
            // Process row data
        }
    }
}
```

### System Integration
#### File System Watcher (`FileSystemWatcher`)
Monitor directories for changes in real-time. This is ideal for "Live Reload" features or asset synchronization.

```cpp
#include <UXCpp/Services/FileSystemWatcher.h>

auto watcher = std::make_shared<uxcpp::services::FileSystemWatcherImpl>();
watcher->watchDirectory("./assets", [](const uxcpp::services::FileSystemEvent& event) {
    if (event.type == uxcpp::services::FileEvent::Modified) {
        std::cout << "File changed: " << event.path << std::endl;
    }
});
```

#### Process Management (`Process`)
Launch and manage external child processes.

```cpp
#include <UXCpp/Services/Process.h>

auto proc = std::make_shared<uxcpp::services::ProcessImpl>();
proc->launch("git", {"status"});
auto result = proc->waitForExit().get();
std::cout << "Git output: " << result.stdOut << std::endl;
```

#### Clipboard & Drag-and-Drop
System clipboard access is handled via the `Application` singleton:
- `app.setClipboardText(string)`
- `app.getClipboardText()`

Drag-and-drop is implemented by overriding hooks in your custom widgets:
- `onDragEnter(payload)`
- `onDragLeave()`
- `onDrop(payload)`

### Storage & Hardware
#### Configuration (`Settings`)
The `uxcpp::services::Settings` API provides a cross-platform way to store user preferences (INI on Windows/Linux, plist on macOS).

```cpp
#include <UXCpp/Services/Settings.h>

auto settings = std::make_shared<uxcpp::services::SettingsImpl>();
settings->load();

// Get a value with a fallback
std::string theme = settings->get("ui.theme").value_or("light");

// Set and save a value
settings->set("ui.theme", "dark");
settings->save();
```

#### Hardware Communication (`SerialPort`)
For industrial or embedded applications, UXCpp includes a robust `SerialPort` service.

```cpp
#include <UXCpp/Services/SerialPort.h>

auto serial = std::make_shared<uxcpp::services::SerialPortImpl>();
if (serial->open("COM3", 9600)) {
    serial->write("GET_STATUS");
    auto response = serial->read();
    if (response.success) {
        std::cout << "Device says: " << response.data << std::endl;
    }
}
```

### Internationalization (`I18n`)
UXCpp supports runtime language switching and translation catalogs via the `TranslationManager`. This allows you to ship a single binary that adapts to the user's locale.
---

## 5. Developer Experience, Tooling & Plugins

UXCpp is designed to be extended both at runtime via plugins and during development using built-in diagnostic tools.

### Runtime Debug Overlay
The `uxcpp::ui::DebugOverlay` provides a real-time visual inspector for the widget tree, allowing developers to debug layout issues without leaving the application.

#### Enabling the Overlay
```cpp
#include <UXCpp/UI/DebugOverlay.h>

// Enable the overlay globally
uxcpp::ui::DebugOverlay::getInstance().setEnabled(true);
```

#### Features of the Debug Overlay
- **Bounds Inspector**: Draws green bounding boxes around every widget in the hierarchy.
- **Widget Metadata**: Displays the Widget ID and its current dimensions (e.g., `MainPanel [800x600]`) above each box.
- **FPS Counter**: A real-time frame timing display in the top-left corner to monitor rendering performance.
- **Highlighting**: Use `setHighlightedWidget(widget)` to mark a specific component in red for easier tracking.

### Plugin Engine
UXCpp supports dynamic extensibility through a stable Plugin ABI. This allows third parties to add new widgets or services without access to the core source code.

#### Writing a Custom Plugin
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

#### Loading Plugins at Runtime
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

### Live Reloading
UXCpp includes a `LiveReloadEngine` that can monitor UI definition files (such as JSON or XML) and automatically rebuild the widget tree when changes are detected on disk, without restarting the application. This significantly accelerates the UI iteration loop.
---

## 6. Full End-to-End Tutorial Application

In this tutorial, we will build a production-ready application that monitors system metrics and displays data from an SQL database. This project combines almost every core feature of UXCpp.

### Project Goals
1. **Navigation**: A sidebar for switching between "System" and "Database" views.
2. **Real-time Metrics**: A line chart showing CPU usage over time.
3. **Data Grid**: A virtualized table displaying SQL records with sorting/filtering.
4. **Async Integration**: Fetching data from a remote API and local DB without blocking the UI.

### Complete Implementation

```cpp
#include <UXCpp/Core/Application.h>
#include <UXCpp/Core/Property.h>
#include <UXCpp/UI/Layout.h>
#include <UXCpp/UI/Controls.h>
#include <UXCpp/UI/PlotWidget.h>
#include <UXCpp/UI/TableView.h>
#include <UXCpp/Core/TableProxyModel.h>
#include <UXCpp/Services/SqlDatabase.h>
#include <UXCpp/Services/HttpClient.h>
#include <UXCpp/Services/Settings.h>
#include <memory>
#include <vector>
#include <string>

using namespace uxcpp;

class DashboardApp {
public:
    DashboardApp() {
        // 1. Setup Application Services
        m_db = std::make_shared<services::SqlDatabaseImpl>();
        m_http = std::make_shared<services::HttpClientImpl>();
        m_settings = std::make_shared<services::SettingsImpl>();
        
        m_settings->load();

        // 2. Build UI Hierarchy
        m_root = std::make_shared<ui::Box>("Root", ui::LayoutOrientation::Horizontal);
        
        // Sidebar
        auto sidebar = std::make_shared<ui::Box>("Sidebar", ui::LayoutOrientation::Vertical);
        sidebar->layoutParams().preferredWidth = 200.0f;

        auto btnSys = std::make_shared<ui::Button>("BtnSys", "System Monitor");
        auto btnDb = std::make_shared<ui::Button>("BtnDb", "SQL Dashboard");
        
        sidebar->addChild(btnSys);
        sidebar->addChild(btnDb);

        // Main Content Area
        m_contentArea = std::make_shared<ui::Box>("Content", ui::LayoutOrientation::Vertical);
        m_contentArea->layoutParams().stretch = 1.0f;

        m_root->addChild(sidebar);
        m_root->addChild(m_contentArea);

        // Setup Views
        m_sysView = createSystemView();
        m_dbView = createDatabaseView();

        // Navigation Logic
        btnSys->onClicked.connect([this]() { switchView(m_sysView); });
        btnDb->onClicked.connect([this]() { switchView(m_dbView); });

        // Default View
        switchView(m_sysView);
    }

    void run() {
        core::Application::getInstance().addOverlayWidget(m_root);
        core::Application::getInstance().run();
    }

private:
    std::shared_ptr<ui::Box> createSystemView() {
        auto view = std::make_shared<ui::Box>("SysView", ui::LayoutOrientation::Vertical);
        
        auto title = std::make_shared<ui::Label>("Title", "Real-time CPU Load");
        
        auto cpuPlot = std::make_shared<ui::PlotWidget>("CPUPlot");
        cpuPlot->setTitle("CPU Usage (%)");
        cpuPlot->setShowGrid(true);

        // Simulate data updates via a property
        auto cpuLoad = std::make_shared<core::Property<float>>(0.0f);
        cpuLoad->observe([cpuPlot](const float& val) {
            static float time = 0;
            cpuPlot->addSeries("Core 0").addSample(time++, val);
        });

        view->addChild(title);
        view->addChild(cpuPlot);
        
        // Store property to update it in a real app loop
        m_cpuLoad = cpuLoad;
        return view;
    }

    std::shared_ptr<ui::Box> createDatabaseView() {
        auto view = std::make_shared<ui::Box>("DbView", ui::LayoutOrientation::Vertical);
        
        auto searchBar = std::make_shared<ui::TextInput>("Search", "Filter records...");
        
        auto table = std::make_shared<ui::TableView>("SqlTable");
        auto baseModel = std::make_shared<core::SimpleTableModel>(
            std::vector<std::string>{"ID", "Name", "Status"},
            std::vector<std::vector<std::string>>{
                {"1", "Server-01", "Online"},
                {"2", "Server-02", "Offline"},
                {"3", "Database-01", "Online"}
            }
        );
        
        auto proxy = std::make_shared<core::TableProxyModel>(baseModel);
        table->setModel(proxy);

        // Link search bar to proxy filter
        searchBar->onClicked.connect([proxy, searchBar]() {
            proxy->setFilter(searchBar->getText());
        });

        view->addChild(searchBar);
        view->addChild(table);
        return view;
    }

    void switchView(std::shared_ptr<ui::Box> newView) {
        m_contentArea->getChildren().clear(); // Simplified: remove all children
        m_contentArea->addChild(newView);
    }

    std::shared_ptr<ui::Box> m_root;
    std::shared_ptr<ui::Box> m_contentArea;
    std::shared_ptr<ui::Box> m_sysView;
    std::shared_ptr<ui::Box> m_dbView;
    
    std::shared_ptr<core::Property<float>> m_cpuLoad;
    std::shared_ptr<services::SqlDatabase> m_db;
    std::shared_ptr<services::HttpClient> m_http;
    std::shared_ptr<services::Settings> m_settings;
};

int main() {
    auto& app = core::Application::getInstance();
    if (!app.init()) return -1;

    DashboardApp myApp;
    myApp.run();

    return 0;
}
```

### Key Technical Takeaways
- **Decoupling**: The `DashboardApp` class separates service initialization from UI construction.
- **Reactivity**: Using `core::Property` allows the CPU plot to update automatically whenever the underlying value changes.
- **Efficiency**: The la TableView and TableProxyModel ensure that filtering thousands of SQL records remains performant by only rendering visible rows.
- **Async Flow**: By using `std::future` in services like `HttpClient`, the UI thread remains responsive while waiting for network I/O.
