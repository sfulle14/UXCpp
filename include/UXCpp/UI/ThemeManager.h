/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace uxcpp::ui {

/**
 * @brief A high-contrast theme palette for accessibility.
 */
struct HighContrastPalette {
    uint32_t background = 0x000000FF; // Black
    uint32_t foreground = 0xFFFFFFFF; // White
    uint32_t accent = 0x00FFFFFF;    // Yellow
};

class ThemeManager {
public:
    static ThemeManager& getInstance() {
        static ThemeManager instance;
        return instance;
    }

    void setHighContrastMode(bool enabled) {
        m_highContrastEnabled = enabled;
        if (enabled) applyHighContrast(); else applyDefault();
    }

    bool isHighContrastEnabled() const { return m_highContrastEnabled; }

private:
    void applyHighContrast() {
        // In a real system, this would update the global Theme singleton's colors
    }

    void applyDefault() {
        // Restore default theme
    }

    bool m_highContrastEnabled = false;
};

} // namespace uxcpp::ui
