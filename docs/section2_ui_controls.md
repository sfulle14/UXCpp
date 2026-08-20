# Section 2: UI Layouts & Control Catalog

UXCpp provides a comprehensive set of layout managers and standard controls to build professional interfaces quickly. All controls are built upon the `uxcpp::ui::Widget` base class.

## Layout Engine

The layout engine automates the positioning and sizing of widgets, removing the need for manual coordinate calculation.

### Box Layout (`uxcpp::ui::Box`)
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

// The Box will call performLayout() to position children based on their LayoutParams.
```

### Understanding `LayoutParams`
Every widget has a `LayoutParams` object that tells the parent layout how to treat it:
- **Stretch**: A value $> 0$ indicates the widget should expand to fill available space proportionally.
- **Preferred Width/Height**: Fixed dimensions for the widget.
- **Margin & Padding**: Spacing around and inside the widget.

```cpp
auto searchBar = std::make_shared<uxcpp::ui::TextInput>("Search", "");
searchBar->layoutParams().stretch = 1.0f; // Expand to fill horizontal space
searchBar->layoutParams().margin = {5, 5, 5, 5}; // Add 5px margin on all sides
```

## Standard Controls Catalog

### Basic Input & Display
| Control | Class | Description | Key Methods |
| :--- | :--- | :--- | :--- |
| **Label** | `uxcpp::ui::Label` | Static or dynamic text display. | `setText(string)` |
| **Button** | `uxcpp::ui::Button` | Clickable trigger for actions. | `onClicked` (Signal) |
| **TextInput** | `uxcpp::ui::TextEditor` | Single/Multi-line editable text. | `getText()`, `setText()` |
| **CheckBox** | `uxcpp::ui::CheckBox` | Binary toggle state. | `setChecked(bool)`, `isChecked()` |
| **ComboBox** | `uxcpp::ui::ComboBox` | Dropdown selection list. | `selectItem(int)` |
| **ProgressBar** | `uxcpp::ui::ProgressBar` | Visual progress indicator. | `setProgress(float)` |

### Example: Creating a Simple Form
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

## Menus & Navigation

UXCpp provides a native-feeling menu system that handles overlays and popups automatically via the `Application` singleton.

### MenuBar & MenuPopup
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

### Toolbars & Status Bars
- **`uxcpp::ui::Toolbar`**: A horizontal bar for quick-access icons. Use `addButton(iconId, action)` to populate it.
- **`uxcpp::ui::StatusBar`**: A thin strip at the bottom of the window for status messages. Use `setMessage(string)` to update text.

## Dialogs & Overlays

### Modal Dialogs (`uxcpp::ui::Dialog`)
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

### Native System Dialogs
For common OS tasks, use the `Application` singleton directly:
- `app.openFileDialog(title, filters)`: Opens a native file picker.
- `app.saveFileDialog(title, filters)`: Opens a native save dialog.
- `app.showMessageBox(title, message, type)`: Displays a system alert.
