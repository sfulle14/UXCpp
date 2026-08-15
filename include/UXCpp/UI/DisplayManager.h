/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <memory>
#include <vector>

namespace uxcpp::ui {

/**
 * @brief Handles high-DPI scaling and multi-monitor display settings.
 */
class DisplayManager {
public:
    static DisplayManager& getInstance() {
        static DisplayManager instance;
        return instance;
    }

    float getScaleFactor(int monitorIndex = 0) const {
        return m_scaleFactors[monitorIndex];
    }

    void setScaleFactor(int monitorIndex, float scale) {
        m_scaleFactors[monitorIndex] = scale;
    }

    /**
     * @brief Returns the logical dimensions of a given monitor.
     */
    struct MonitorInfo {
        int width;
        int height;
        float scale;
    };

    std::vector<MonitorInfo> getMonitors() const {
        return m_monitors;
    }

private:
    DisplayManager() {
        // Default to 1.0x scaling
        m_scaleFactors[0] = 1.0f;
        m_monitors.push_back({1920, 1080, 1.0f});
    }

    std::map<int, float> m_scaleFactors;
    std::vector<MonitorInfo> m_monitors;
};

} // namespace uxcpp::ui
