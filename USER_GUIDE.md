# UXCpp User Guide

UXCpp is a clean-room, MIT-licensed GUI framework for modern C++ applications. This guide explains how to use the framework from the ground up, including window setup, widgets, layout, binding, charts, dialogs, and custom controls.

## 1. Overview

UXCpp is built around a retained-mode widget tree. Instead of drawing everything every frame by hand, you create widgets, connect them together, and let the framework manage layout, input propagation, and rendering.

### Core concepts
- **Application** manages the main loop and global overlays.
- **Windowing** is abstracted through a backend such as GLFW.
- **Renderer** provides drawing primitives like rectangles, lines, circles, and text.
- **Widget** is the base class for all visual controls.
- **Layouts** arrange widgets automatically.
- **Signals** provide event callbacks without a meta-object compiler.
- **Properties** provide lightweight reactive binding.

## 2. Getting Started

### Minimal application
```cpp
#include <UXCpp/Core/Application.h>

int main() {
    auto& app = uxcpp::core::Application::getInstance();
    if (!app.init()) {
        return -1;
    }

    app.run();
    return 0;
}
```

### What `init()` and `run()` do
- `init()` creates the backend window and renderer.
- `run()` enters the main event loop.
- `quit()` stops the loop.

## 3. Building Widgets

Every widget inherits from `uxcpp::ui::Widget`. A widget has:
- a name,
- a style class,
- bounds,
- layout parameters,
- children,
- input handlers,
- and draw/update hooks.

### Common operations
```cpp
auto button = std::make_shared<uxcpp::ui::Button>("OkButton", "OK");
button->setBounds(20, 20, 120, 40);
button->setStyleClass("primary");
```

### Adding children
```cpp
auto root = std::make_shared<uxcpp::ui::Box>("Root", uxcpp::ui::LayoutOrientation::Vertical);
root->addChild(button);
```

## 4. Signals and Event Handling

UXCpp uses a type-safe signal system. You connect lambdas or functions directly to widget signals.

### Button clicks
```cpp
button->onClicked.connect([]() {
    std::cout << "Button pressed" << std::endl;
});
```

### Typical event overrides
When creating a custom widget, override:
- `onDraw()` for rendering,
- `onPointerDown()` for mouse input,
- `onKeyDown()` for keyboard input,
- `onFocusGained()` and `onFocusLost()` for focus changes.

## 5. Layout Management

Layout widgets compute positions and sizes automatically.

### Box layout
Use `Box` for vertical or horizontal stacking.
```cpp
auto root = std::make_shared<uxcpp::ui::Box>("Main", uxcpp::ui::LayoutOrientation::Vertical);
root->setBounds(0, 0, 600, 400);

auto title = std::make_shared<uxcpp::ui::Label>("Title", "Dashboard");
title->layoutParams().preferredHeight = 40.0f;

auto body = std::make_shared<uxcpp::ui::Widget>("Body");
body->layoutParams().stretch = 1.0f;

root->addChild(title);
root->addChild(body);
root->performLayout();
```

### Grid layout
Use `GridLayout` for dashboards, forms, and control panels.
```cpp
auto grid = std::make_shared<uxcpp::ui::GridLayout>("Grid", 2, 2);
grid->setBounds(0, 0, 500, 300);
grid->addWidget(buttonA, 0, 0);
grid->addWidget(buttonB, 0, 1);
```

### Layout parameters
Each widget exposes `layoutParams()`.
- `margin` controls external spacing.
- `padding` controls inner spacing.
- `preferredWidth` and `preferredHeight` set fixed sizes.
- `stretch` makes a widget expand into available space.

## 6. Reactive Data Binding

The `Property<T>` class lets widgets observe data without manual refresh logic.

### Example: progress bar binding
```cpp
uxcpp::core::Property<float> downloadProgress(0.0f);
auto bar = std::make_shared<uxcpp::ui::ProgressBar>("Download", &downloadProgress);

// Updating the property automatically updates the widget
downloadProgress.set(0.75f);
```

### When to use it
Use properties for:
- progress values,
- settings panels,
- forms,
- data dashboards,
- and any state shared across multiple widgets.

## 7. Advanced Components

### Modal dialogs
Dialogs live in the overlay layer and block the background UI.
```cpp
auto dialog = std::make_shared<uxcpp::ui::Dialog>("Confirm", "Delete item?");
dialog->setContent(contentWidget);
dialog->show();
```

### Tab widgets
Use `TabWidget` to present multiple pages in one area.
```cpp
auto tabs = std::make_shared<uxcpp::ui::TabWidget>("SettingsTabs");
tabs->addTab("General", generalPage);
tabs->addTab("Advanced", advancedPage);
```

### Tree views
Use `TreeView` with a `TreeModel` for hierarchical data.
```cpp
auto rootNode = std::make_shared<uxcpp::core::TreeNode>("Project");
rootNode->children.push_back(std::make_shared<uxcpp::core::TreeNode>("src"));
rootNode->children.push_back(std::make_shared<uxcpp::core::TreeNode>("include"));

auto treeModel = std::make_shared<uxcpp::core::SimpleTreeModel>(rootNode);
auto treeView = std::make_shared<uxcpp::ui::TreeView>("ProjectTree");
treeView->setModel(treeModel);
```

### Table views
Use `TableView` with a `TableModel` for tabular data.
```cpp
auto model = std::make_shared<uxcpp::core::SimpleTableModel>(
    std::vector<std::string>{"Name", "Status"},
    std::vector<std::vector<std::string>>{
        {"Renderer", "Ready"},
        {"Input", "Ready"}
    }
);
auto table = std::make_shared<uxcpp::ui::TableView>("StatusTable");
table->setModel(model);
```

### Combo boxes
Use `ComboBox` for dropdown selection.
```cpp
auto combo = std::make_shared<uxcpp::ui::ComboBox>("Mode", std::vector<std::string>{"Fast", "Balanced", "Quality"});
```

## 8. Plotting and Data Visualization

UXCpp now includes a lightweight `PlotWidget` for line charts and trend graphs.

### What it supports
- multiple line series,
- automatic axis scaling,
- grid lines,
- titles,
- and legends.

### Basic example
```cpp
auto plot = std::make_shared<uxcpp::ui::PlotWidget>("Latency");
plot->setBounds(20, 20, 500, 280);
plot->setTitle("Service Latency");

auto& requests = plot->addSeries("Requests", uxcpp::graphics::Color::Blue());
requests.addSample(0.0f, 12.0f);
requests.addSample(1.0f, 20.0f);
requests.addSample(2.0f, 18.0f);
requests.addSample(3.0f, 22.0f);
```

### Plot widget behavior
- The chart auto-scales to the data you add.
- Series are drawn as connected line segments.
- The legend shows each visible series and its color.
- Grid lines are enabled by default.

### Best practices for plots
- Add samples in increasing X order when possible.
- Use one series per metric.
- Keep plot bounds large enough for labels and legends.
- For live charts, update series samples and redraw in your main loop.

### What is not implemented yet
- zoom and pan,
- scatter plots,
- bar charts,
- histogram views,
- candlesticks,
- antialiased shader-based plot rendering.

## 9. Rich Text and Typography

UXCpp now has the beginning of a richer text stack:
- `TextLayout` performs word-aware wrapping and line-height calculation.
- `TextDocument` lets you build styled spans for future rich text rendering.
- `RichTextLabel` can display a simple styled document with wrapping.

### Rich text example
```cpp
uxcpp::graphics::TextDocument doc;

uxcpp::graphics::TextStyle heading;
heading.size = 20.0f;
heading.bold = true;
heading.color = uxcpp::graphics::Color::Black();

uxcpp::graphics::TextStyle body;
body.size = 16.0f;
body.color = uxcpp::graphics::Color::Blue();

doc.append("UXCpp Guide", heading);
doc.append("This is a simple rich text document.", body);

auto rich = std::make_shared<uxcpp::ui::RichTextLabel>("IntroText");
rich->setDocument(doc);
rich->setBounds(20, 20, 500, 120);
```

### Current limitations
- Rich text is span-based rather than full HTML.
- Text shaping still uses a lightweight layout strategy instead of a full font engine.
- Complex scripts and bidirectional layout are still roadmap items.

## 10. Clipboard and File Dialogs

UXCpp now exposes basic platform services through `uxcpp::core::Application`.

### Clipboard
```cpp
auto& app = uxcpp::core::Application::getInstance();
app.setClipboardText("Hello from UXCpp");

std::string copied = app.getClipboardText();
```

### File dialogs
```cpp
std::string fileToOpen = app.openFileDialog("Open a file", {"Text Files"});
std::string fileToSave = app.saveFileDialog("Save file", {"Text Files"});
```

### Notes
- Clipboard support uses the active GLFW window.
- File dialogs are native on Windows and fall back to a no-op stub on unsupported platforms for now.
- This is enough for basic editor and utility workflows, but drag-and-drop and native menu integration are still planned.

### Message boxes
```cpp
uxcpp::core::Application::getInstance().showMessageBox(
    "Saved",
    "Your changes were written successfully.",
    uxcpp::core::MessageBoxType::Information
);
```

### Notes
- Message boxes are native on Windows and fall back to console output elsewhere.
- Use them for lightweight alerts, confirmations, and user-visible status messages.

### System Tray
```cpp
auto trayIcon = app.createSystemTrayIcon("My App is running in the background");
trayIcon->setOnClick([]() {
    // Restore window or show menu
});
trayIcon->activate();
```

### Notes
- System tray icons are currently supported on Windows via Shell_NotifyIcon.
- This allows applications to run as background services with a visible presence in the notification area.

## 11. Keyboard Shortcuts and Drag and Drop

UXCpp now includes the beginnings of a more desktop-like interaction layer.

### Keyboard shortcuts
```cpp
uxcpp::core::ShortcutManager::getInstance().registerShortcut(
    { 'S', true, false, false },
    []() {
        // Save the current document
    },
    "Save"
);
```

### Drag and drop
Widgets can participate in drag-and-drop by overriding the new hooks on `Widget`.
```cpp
class FileDropZone : public uxcpp::ui::Widget {
public:
    using Widget::Widget;

    void onDragEnter(const uxcpp::core::DragDropPayload&) override {
        // Highlight the drop zone
    }

    void onDragLeave() override {
        // Remove the highlight
    }

    bool onDrop(const uxcpp::core::DragDropPayload& payload) override {
        if (payload.type == uxcpp::core::DragDropPayload::Type::Text) {
            auto text = payload.getValue<std::string>();
            // Handle dropped text
            return true;
        }
        return false;
    }
};
```

### Notes
- Keyboard shortcuts are routed through `InputManager` before the focused widget receives input.
- Tab navigation uses widget tab indices to move focus across the UI.
- Drag-and-drop is currently framework-level and is intended to grow into native file and text payload support.

## 12. Menus and Context Menus

UXCpp now includes a lightweight menu bar and popup menu model that uses overlays for display.

### Menu bar example
```cpp
auto menuBar = std::make_shared<uxcpp::ui::MenuBar>("MainMenu");
menuBar->setBounds(0, 0, 800, 28);

auto fileMenu = menuBar->addMenu("File");
fileMenu->addItem({"Open", []() {
    // Open a file
}});
fileMenu->addItem({"Save", []() {
    // Save the current document
}});
```

### Context menu example
```cpp
auto contextMenu = std::make_shared<uxcpp::ui::MenuPopup>("EditorContextMenu");
contextMenu->addItem({"Copy", []() { /* copy selection */ }});
contextMenu->addItem({"Paste", []() { /* paste selection */ }});
contextMenu->showAt(100.0f, 100.0f, 160.0f);
```

### Notes
- Menus are rendered as overlay widgets and can be triggered from a menu bar or opened directly as a context menu.
- Each menu item stores a callback, so actions stay in application code rather than the UI framework.
- This is a framework menu system, not yet a full native menu bar integration.

## 13. Styling and Theming

UXCpp uses a global `Theme` manager. Style classes let you separate appearance from behavior.

### Defining a style
```cpp
uxcpp::ui::WidgetStyle warningStyle;
warningStyle.backgroundColor = uxcpp::graphics::Color::Red();
warningStyle.foregroundColor = uxcpp::graphics::Color::White();

uxcpp::ui::Theme::getInstance().setStyle("warning", warningStyle);
```

### Applying a style
```cpp
auto warningButton = std::make_shared<uxcpp::ui::Button>("Delete", "Delete");
warningButton->setStyleClass("warning");
```

## 14. Declarative UI Loading

The `UILoader` can build widget trees from a simple string format.

### Example
```cpp
std::string ui =
    "[Box, \"Main\", \"Vertical\"] {"
    "  [Label, \"Title\", \"Welcome to UXCpp\"]"
    "  [Button, \"Submit\", \"Run\"]"
    "}";

auto root = uxcpp::core::UILoader::loadFromString(ui);
```

### When to use it
Use declarative loading when you want:
- fast prototyping,
- UI definitions separate from code,
- easier future tooling,
- and template-like screen layouts.

## 15. Creating Custom Widgets

To create a new widget, inherit from `Widget` and override the behavior you need.

### Example: custom gauge
```cpp
class Gauge : public uxcpp::ui::Widget {
public:
    explicit Gauge(std::string name) : Widget(std::move(name), "gauge") {}

    void onDraw(uxcpp::graphics::Renderer& renderer) override {
        renderer.drawRect(m_bounds, getStyle().backgroundColor);
        renderer.drawLine(
            {m_bounds.x, m_bounds.y + m_bounds.height},
            {m_bounds.x + m_bounds.width, m_bounds.y},
            getStyle().foregroundColor
        );
    }
};
```

### Custom widget checklist
- set a style class,
- define drawing,
- handle pointer and keyboard events as needed,
- use child widgets if the control is composite,
- expose signals or properties when state changes.

## 16. Common Usage Patterns

### Putting it all together
```cpp
auto root = std::make_shared<uxcpp::ui::Box>("Root", uxcpp::ui::LayoutOrientation::Vertical);
root->setBounds(0, 0, 800, 600);

auto title = std::make_shared<uxcpp::ui::Label>("Title", "Dashboard");
auto plot = std::make_shared<uxcpp::ui::PlotWidget>("Traffic");
auto button = std::make_shared<uxcpp::ui::Button>("Refresh", "Refresh");

auto& series = plot->addSeries("Requests", uxcpp::graphics::Color::Blue());
series.addSample(0.0f, 3.0f);
series.addSample(1.0f, 8.0f);
series.addSample(2.0f, 6.0f);

button->onClicked.connect([plot]() {
    // Update plot samples here
});

root->addChild(title);
root->addChild(plot);
root->addChild(button);
root->performLayout();
```

## 17. Troubleshooting

### My widget is not visible
- Check that it has bounds.
- Make sure it was added to a parent.
- Call `performLayout()` on the root container.
- Confirm the widget is within the visible window area.

### My plot is empty
- Make sure you added at least two points to a visible series.
- Confirm the plot bounds are large enough.
- Check that `PlotWidget` is included and drawn each frame.

### My dialog is behind other content
- Use `Dialog::show()` so it enters the overlay layer.

### My control is not getting keyboard input
- Ensure it has focus.
- Use Tab navigation or call `InputManager::setFocus()` through a pointer click.

## 18. Current Feature Status

UXCpp currently supports:
- application lifecycle management,
- retained-mode widget trees,
- signals and reactive properties,
- box and grid layouts,
- styling and theming,
- dialogs and overlays,
- keyboard focus traversal,
- accessibility metadata,
- keyboard shortcut registration,
- drag-and-drop hooks,
- menus and context menus,
- table views and tree views,
- combo boxes and tab widgets,
- and lightweight line plotting.

Still planned for later:
- zoomable plotting,
- advanced text shaping,
- richer chart types,
- and external designer tooling.

---
UXCpp is designed for performance, clarity, and proprietary use.
