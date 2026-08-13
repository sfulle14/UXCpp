/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <functional>
#include <memory>

namespace uxcpp::core {

/**
 * @brief Manages a system tray icon and its associated behavior.
 */
class SystemTrayIcon {
public:
    struct Config {
        std::string tooltip;
        // In a real implementation, we'd pass an HICON or image data here.
        // For now, we use a default system icon on Windows.
    };

    explicit SystemTrayIcon(const Config& config) : m_config(config) {}
    ~SystemTrayIcon();

    /**
     * @brief Sets the callback for when the tray icon is clicked.
     */
    void setOnClick(std::function<void()> action) { m_onClick = std::move(action); }

    /**
     * @brief Updates the tooltip text of the tray icon.
     */
    void setTooltip(const std::string& tooltip) {
        m_config.tooltip = tooltip;
        update();
    }

    /**
     * @brief Activates the tray icon in the system notification area.
     */
    void activate();

    /**
     * @brief Removes the tray icon from the system notification area.
     */
    void deactivate();

private:
    void update();

    Config m_config;
    std::function<void()> m_onClick;
    bool m_isActive = false;
};

} // namespace uxcpp::core
