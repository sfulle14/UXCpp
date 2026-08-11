/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <UXCpp/Graphics/Renderer.h>

namespace uxcpp::graphics {

struct TextLine {
    std::string text;
    float width;
};

/**
 * @brief Handles multi-line text wrapping and layout.
 */
class TextLayout {
public:
    TextLayout(const std::string& text, float maxWidth, float fontSize = 16.0f) {
        wrapText(text, maxWidth, fontSize);
    }

    const std::vector<TextLine>& getLines() const { return m_lines; }
    float getTotalHeight() const { return m_lines.size() * (18.0f); } // Approx line height

private:
    void wrapText(const std::string& text, float maxWidth, float fontSize) {
        std::string currentLine;
        float currentWidth = 0;

        for (char c : text) {
            // Simplified width calculation (assuming monospaced for now)
            float charWidth = 8.0f; 
            if (currentWidth + charWidth > maxWidth) {
                m_lines.push_back({currentLine, currentWidth});
                currentLine = "";
                currentWidth = 0;
            }
            currentLine += c;
            currentWidth += charWidth;
        }
        if (!currentLine.empty()) {
            m_lines.push_back({currentLine, currentWidth});
        }
    }

    std::vector<TextLine> m_lines;
};

} // namespace uxcpp::graphics
