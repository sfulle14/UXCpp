# Section 4: Application Services

UXCpp provides a suite of cross-platform services for OS integration, networking, and data persistence. All services are designed as interfaces to allow for easy mocking during testing.

## Async Networking (`HttpClient`)

The `uxcpp::services::HttpClient` allows applications to perform non-blocking REST requests using C++20 futures.

### Performing a GET Request
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

## Database Layer (`SqlDatabase`)

UXCpp provides a unified interface for SQL databases, supporting connection pooling and asynchronous query execution.

### Basic Database Operations
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

## File System & OS Integration

### File System Watcher (`FileSystemWatcher`)
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

### Process Management (`Process`)
Launch and manage external child processes.

```cpp
#include <UXCpp/Services/Process.h>

auto proc = std::make_shared<uxcpp::services::ProcessImpl>();
proc->launch("git", {"status"});
auto result = proc->waitForExit().get();
std::cout << "Git output: " << result.stdOut << std::endl;
```

### Clipboard & Drag-and-Drop
System clipboard access is handled via the `Application` singleton:
- `app.setClipboardText(string)`
- `app.getClipboardText()`

Drag-and-drop is implemented by overriding hooks in your custom widgets:
- `onDragEnter(payload)`
- `onDragLeave()`
- `onDrop(payload)`

## Storage & Configuration (`Settings`)

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

## Hardware Communication (`SerialPort`)

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

## Internationalization (`I18n`)

UXCpp supports runtime language switching and translation catalogs via the `TranslationManager`. This allows you to ship a single binary that adapts to the user's locale.
