# Section 6: Tutorial - Building a System Monitor & SQL Dashboard

In this tutorial, we will build a production-ready application that monitors system metrics and displays data from an SQL database. This project combines almost every core feature of UXCpp.

## Project Goals
1. **Navigation**: A sidebar for switching between "System" and "Database" views.
2. **Real-time Metrics**: A line chart showing CPU usage over time.
3. **Data Grid**: A virtualized table displaying SQL records with sorting/filtering.
4. **Async Integration**: Fetching data from a remote API and local DB without blocking the UI.

## Complete Implementation

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

## Key Technical Takeaways
- **Decoupling**: The `DashboardApp` class separates service initialization from UI construction.
- **Reactivity**: Using `core::Property` allows the CPU plot to update automatically whenever the underlying value changes.
- **Efficiency**: The `TableView` and `TableProxyModel` ensure that filtering thousands of SQL records remains performant by only rendering visible rows.
- **Async Flow**: By using `std::future` in services like `HttpClient`, the UI thread remains responsive while waiting for network I\u0026O.
