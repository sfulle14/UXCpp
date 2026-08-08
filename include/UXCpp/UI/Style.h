/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/Renderer.h>
#include <unordered_map>
#include <string>

namespace uxcpp::ui {

/**
 * @brief Defines the visual properties of a widget.
 */
struct WidgetStyle {
    graphics::Color backgroundColor = graphics::Color::White();
    graphics::Color foregroundColor = graphics::Color::Black();
    graphics::Color borderColor = graphics::Color::Black();
    float borderWidth = 1.0f;
    float borderRadius = 0.0f;
};

/**
 * @brief A simple theme manager to handle global styles.
 */
class Theme {
public:
    static Theme& getInstance() {
        static Theme instance;
        return instance;
    }

    void setStyle(const std::string& className, const WidgetStyle& style) {
        m_styles[className] = style;
    }

    WidgetStyle getStyle(const std::string& className) const {
        auto it = m_styles.find(className);
        if (it != m_styles.end()) return it->second;
        return m_defaultStyle;
    }

private:
    Theme() = default;
    std::unordered_map<std::string, WidgetStyle> m_styles;
    WidgetStyle m_defaultStyle;
};

} // namespace uxcpp::ui
