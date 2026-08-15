/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/Renderer.h>
#include <UXCpp/Graphics/FontManager.h>
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
        
        FontFace face{"Sans", static_cast<int>(fontSize)};
        m_lineHeight = FontManager::getInstance().getLineHeight(face);

        std::string currentLine;
        float currentWidth = 0.0f;

        auto flushLine = [&]() {
            m_lines.push_back({currentLine, currentWidth});
            currentLine.clear();
            currentWidth = 0.0f;
        };

        for (char c : text) {
            if (c == '\n') {
                flushLine();
                continue;
            }

            std::string charStr(1, c);
            float charW = FontManager::getInstance().measureString(charStr, face);

            if (currentWidth + charW > maxWidth && !currentLine.empty()) {
                // Word wrap: find last space
                size_t lastSpace = currentLine.find_last_of(" \t");
                if (lastSpace != std::string::npos) {
                    std::string word = currentLine.substr(lastSpace + 1);
                    currentLine = currentLine.substr(0, lastSpace);
                    
                    // Recalculate width for the truncated line
                    currentWidth = FontManager::getInstance().measureString(currentLine, face);
                    flushLine();
                    
                    currentLine = word;
                    currentWidth = FontManager::getInstance().measureString(word, face);
                } else {
                    // Force break if no space found
                    flushLine();
                    currentLine = charStr;
                    currentWidth = charW;
                }
            } else {
                currentLine += c;
                currentWidth += charW;
            }
        }

        if (!currentLine.empty()) {
            flushLine();
        }
    }
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
