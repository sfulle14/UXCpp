/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <algorithm>
#include <string>
#include <memory>
#include <functional>
#include <vector>

#include <UXCpp/Core/MessageBox.h>

struct GLFWwindow;

namespace uxcpp::ui {
class Widget;
}

namespace uxcpp::core {

/**
 * @brief The Application class manages the global lifecycle of the GUI framework.
 * 
 * It handles the main event loop and provides a central point for application-wide settings.
 */
class Application {
public:
    static Application& getInstance() {
        static Application instance;
        return instance;
    }

    void addOverlayWidget(std::shared_ptr<ui::Widget> widget) {
        m_overlays.push_back(widget);
    }

    void removeOverlayWidget(std::shared_ptr<ui::Widget> widget) {
        m_overlays.erase(
            std::remove(m_overlays.begin(), m_overlays.end(), widget),
            m_overlays.end()
        );
    }

    const std::vector<std::shared_ptr<ui::Widget>>& getOverlays() const {
        return m_overlays;
    }

    void setClipboardText(const std::string& text);
    std::string getClipboardText() const;

    std::string openFileDialog(const std::string& title, const std::vector<std::string>& filters = {});
    std::string saveFileDialog(const std::string& title, const std::vector<std::string>& filters = {});
    std::shared_ptr<class SystemTrayIcon> createSystemTrayIcon(const std::string& tooltip);
    void showNativeContextMenu(int x, int y, const std::vector<std::pair<std::string, std::function<void()>>>& items);
    void showMessageBox(const std::string& title, const std::string& message, MessageBoxType type = MessageBoxType::Information);

    // Delete copy/move to ensure singleton integrity
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    /**
     * @brief Initializes the application and its underlying backends.
     */
    bool init();

    /**
     * @brief Starts the main event loop. Blocks until quit() is called.
     */
    void run();

    /**
     * @brief Signals the application to terminate the event loop.
     */
    void quit();

    /**
     * @brief Returns true if the application is currently running.
     */
    bool isRunning() const { return m_running; }

private:
    Application() : m_running(false) {}
    ~Application() = default;

    bool m_running;
    std::vector<std::shared_ptr<ui::Widget>> m_overlays;
};

} // namespace uxcpp::core
