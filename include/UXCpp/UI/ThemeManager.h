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
struct HighContrastTheme {
    static constexpr uint32_t Background = 0x000000FF; // Black
    static constexpr uint32_t Foreground = 0xFFFFFFFF; // White
    static constexpr uint32_t Accent = 0x00FFFF00;    // Yellow
};

class ThemeManager {
public:
    static ThemeManager& getInstance() {
        static ThemeManager instance;
        return instance;
    }

    void setHighContrastMode(bool enabled) {
        m_highContrastEnabled = enabled;
    }

    bool isHighContrastEnabled() const { return m_highContrastEnabled; }

    uint32_t getColor(const std::string& key) {
        if (!m_highContrastEnabled) {
            return 0xFFFFFFFF; // Default white
        }
        if (key == "bg") return HighContrastTheme::Background;
        if (key == "fg") return HighContrastTheme::Foreground;
        return HighContrastTheme::Accent;
    }

private:
    ThemeManager() = default;
    bool m_highContrastEnabled = false;
};

} // namespace uxcpp::ui
