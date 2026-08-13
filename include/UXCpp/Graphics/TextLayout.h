/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/Renderer.h>
#include <cctype>
#include <string>
#include <vector>

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
    float getTotalHeight() const { return m_lines.empty() ? 0.0f : m_lines.size() * m_lineHeight; }
    float getLineHeight() const { return m_lineHeight; }

private:
    void wrapText(const std::string& text, float maxWidth, float fontSize) {
        m_lines.clear();
        m_lineHeight = std::max(12.0f, fontSize * 1.25f);

        std::string currentLine;
        float currentWidth = 0.0f;

        auto flushLine = [&]() {
            m_lines.push_back({currentLine, currentWidth});
            currentLine.clear();
            currentWidth = 0.0f;
        };

        auto measureChar = [fontSize](char c) {
            if (c == ' ' || c == '\t') return fontSize * 0.45f;
            if (std::ispunct(static_cast<unsigned char>(c))) return fontSize * 0.5f;
            return fontSize * 0.6f;
        };

        auto appendWord = [&](const std::string& word) {
            if (word.empty()) return;

            float wordWidth = 0.0f;
            for (char c : word) {
                wordWidth += measureChar(c);
            }

            if (!currentLine.empty() && currentWidth + measureChar(' ') + wordWidth > maxWidth) {
                flushLine();
            }

            if (!currentLine.empty()) {
                currentLine += ' ';
                currentWidth += measureChar(' ');
            }

            currentLine += word;
            currentWidth += wordWidth;

            if (currentWidth >= maxWidth) {
                flushLine();
            }
        };

        std::string word;
        for (char c : text) {
            if (c == '\n') {
                appendWord(word);
                word.clear();
                flushLine();
                continue;
            }

            if (std::isspace(static_cast<unsigned char>(c))) {
                appendWord(word);
                word.clear();
                continue;
            }

            word += c;
        }

        appendWord(word);
        if (!currentLine.empty() || m_lines.empty()) {
            flushLine();
        }
    }

    std::vector<TextLine> m_lines;
    float m_lineHeight = 18.0f;
};

} // namespace uxcpp::graphics
