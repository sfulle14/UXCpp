# Section 3: Advanced Data & Graphics

UXCpp provides high-level abstractions for handling large datasets, complex vector graphics, and real-time data visualization.

## Model/View Architecture

To prevent UI freezing when dealing with thousands of items, UXCpp uses a decoupled Model/View architecture. The View only requests the data it needs to render currently visible cells.

### Table Models (`uxcpp::core::TableModel`)
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

### TableView & Virtualization
The `uxcpp::ui::TableView` widget renders the model. It supports virtualization, meaning it only draws the rows currently visible on screen.

```cpp
#include <UXCpp/UI/TableView.h>

auto table = std::make_shared<uxcpp::ui::TableView>("DataTable");
auto model = std::make_shared<MyDataModel>();
table->setModel(model);
```

### Proxy Models for Sorting & Filtering
The `uxcpp::core::TableProxyModel` wraps an existing model to provide sorting and filtering without modifying the original data source.

```cpp
#include <UXCpp/Core/TableProxyModel.h>

auto baseModel = std::make_shared<MyDataModel>();
auto proxyModel = std::make_shared<uxcpp::core::TableProxyModel>(baseModel);

proxyModel->setFilter("Search Term"); // Only rows containing "Search Term" are shown
proxyModel->setSortColumn(1, true);    // Sort by column 1 ascending

table->setModel(proxyModel);
```

## Vector Graphics & Assets

UXCpp uses a modern rendering pipeline that supports high-fidelity vector graphics and dynamic canvas drawing.

### The Renderer API
The `uxcpp::graphics::Renderer` provides the primitives needed to draw custom widgets:
- `drawRect(rect, color)`: Fills a rectangle.
- `drawLine(p1, p2, color)`: Draws a line between two points.
- `drawText(point, text, color)`: Renders text using the current font manager.
- `drawFocusRing(rect, color)`: Draws a stylized border for focused elements.

### SVG Rendering
UXCpp supports SVG assets via the `uxcpp::graphics::SVGAsset` class, allowing for resolution-independent icons and illustrations.

## Charting & Plotting

For scientific and financial data visualization, UXCpp includes the `PlotWidget`.

### Using the PlotWidget
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

## Code Editor & Rich Text

For applications requiring text manipulation, UXCpp provides a professional-grade `TextEditor`.

### The TextEditor Widget
The `uxcpp::ui::TextEditor` supports:
- **Caret Navigation**: Precise control over the insertion point.
- **Clipboard Integration**: Native copy/paste via `Application::getClipboardText()`.
- **Selection Handling**: Ability to highlight and manipulate text blocks.

```cpp
#include <UXCpp/UI/TextEditor.h>

auto editor = std::make_shared<uxcpp::ui::TextEditor>("CodeEditor");
editor->setText("// Welcome to UXCpp\nint main() { return 0; }");

// Handle text changes via signals or polling getText()
```
