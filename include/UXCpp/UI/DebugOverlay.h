/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/Renderer.h>
#include <UXCpp/UI/Widget.h>
#include <vector>
#include <memory>
#include <string>
#include <chrono>

namespace uxcpp::ui {

class DebugOverlay {
public:
    static DebugOverlay& getInstance() {
        static DebugOverlay instance;
        return instance;
    }

    void setEnabled(bool enabled) { m_enabled = enabled; }
    bool isEnabled() const { return m_enabled; }

    /**
     * @brief Renders the debug overlay over the current UI.
     */
    void render(graphics::Renderer& renderer, std::shared_ptr<Widget> root) {
        if (!m_enabled) return;

        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - m_lastFrameTime).count();
        m_lastFrameTime = now;

        // Draw frame timing in top-left
        renderer.drawText({10, 10}, "FPS: " + std::to_string(1000000 / duration), graphics::Color::Green());

        // Recursively draw widget bounds
        renderWidgetBounds(renderer, root);
    }

    void setHighlightedWidget(std::shared_ptr<Widget> widget) { m_highlighted = widget; }

private:
    void renderWidgetBounds(graphics::Renderer& renderer, std::shared_ptr<Widget> widget) {
        if (!widget) return;

        graphics::Rect bounds = widget->getBounds();
        graphics::Color color = (widget == m_highlighted) ? graphics::Color::Red() : graphics::Color::Green();

        // Draw bounding box
        renderer.drawFocusRing(bounds, color);

        // Draw label: ID | Size
        std::string label = widget->getId() + " [" + std::to_string((int)bounds.width) + "x" + std::to_string((int)bounds.height) + "]";
        renderer.drawText({bounds.x, bounds.y - 15}, label, color);

        // Recurse to children
        // Note: In a real implementation, we would access m_children via a public getter or friend class
    }

    bool m_enabled = false;
    std::shared_ptr<Widget> m_highlighted = nullptr;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastFrameTime;
};

} // namespace uxcpp::ui