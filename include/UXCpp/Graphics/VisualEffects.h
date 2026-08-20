/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <UXCpp/Graphics/Renderer.h>
#include <UXCpp/UI/Widget.h>

namespace uxcpp::graphics {

/**
 * @brief High-level utility for creating complex visual effects.
 */
class VisualEffects {
public:
    static void applyGlow(Renderer& renderer, const Rect& rect, Color color, float intensity) {
        // Simulate glow by drawing multiple concentric rectangles with decreasing alpha
        for (int i = 1; i <= 5; ++i) {
            float offset = i * 2.0f * intensity;
            Color glowColor = color;
            glowColor.a *= (1.0f / i);
            renderer.drawRect({rect.x - offset, rect.y - offset, rect.width + 2*offset, rect.height + 2*offset}, glowColor);
        }
    }

    static void applyInnerShadow(Renderer& renderer, const Rect& rect, Color color) {
        // Simulate inner shadow by drawing a thin border inside the rect
        renderer.drawRect({rect.x, rect.y, rect.width, 2.0f}, color);
        renderer.drawRect({rect.x, rect.y, 2.0f, rect.height}, color);
    }
};

} // namespace uxcpp::graphics