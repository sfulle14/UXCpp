/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/Renderer.h>
#include <string>
#include <memory>

namespace uxcpp::ui {

/**
 * @brief Base class for custom cell rendering in tables and lists.
 */
class ItemDelegate {
public:
    virtual ~ItemDelegate() = default;

    /**
     * @brief Renders a specific cell.
     */
    virtual void renderCell(graphics::Renderer& renderer, const graphics::Rect& rect, 
                            const std::string& text, bool selected, bool focused) {
        // Default rendering: simple text
        graphics::Color bgColor = selected ? graphics::Color{0.7f, 0.8f, 1.0f, 1.0f} : graphics::Color::White();
        renderer.drawRect(rect, bgColor);
        renderer.drawText({rect.x + 5, rect.y + 2}, text, graphics::Color::Black());
    }

    /**
     * @brief Creates an editor widget for the cell.
     */
    virtual std::shared_ptr<Widget> createEditor(const std::string& currentText) {
        return nullptr; // No editor by default
    }
};

} // namespace uxcpp::ui
