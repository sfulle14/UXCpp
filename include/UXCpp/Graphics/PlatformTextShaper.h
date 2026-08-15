/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/RichText.h>
#include <UXCpp/Graphics/FontManager.h>
#include <algorithm>
#include <iostream>

namespace uxcpp::graphics {

/**
 * @brief A platform-agnostic shaper that handles BiDi and basic shaping.
 * In a production environment, this would wrap HarfBuzz and FriBidi.
 */
class PlatformTextShaper : public ITextShaper {
public:
    ShapedLine shapeText(const std::string& text, float fontSize, bool bold, bool italic) override {
        ShapedLine line;
        line.width = 0;
        line.height = fontSize * 1.2f;

        // BiDi Detection (Simplified: check for Arabic/Hebrew ranges)
        bool isRTL = false;
        for (char c : text) {
            if ((unsigned char)c >= 0xD0 && (unsigned char)c <= 0xDF) { // Very rough RTL check
                isRTL = true;
                break;
            }
        }

        std::string processedText = text;
        if (isRTL) {
            // Reverse the string for visual order (Simplified BiDi)
            std::reverse(processedText.begin(), processedText.end());
        }

        FontFace face{"Sans", static_cast<int>(fontSize), bold, italic};
        float currentX = 0;

        for (char c : processedText) {
            // In a real shaper, we'd look up the glyph ID and advance from FontManager
            float advance = FontManager::getInstance().measureString(std::string(1, c), face);
            
            line.glyphs.push_back({
                static_cast<uint32_t>(c),
                currentX,
                0, // Baseline offset
                advance
            });
            currentX += advance;
        }

        line.width = currentX;
        return line;
    }
};

} // namespace uxcpp::graphics
