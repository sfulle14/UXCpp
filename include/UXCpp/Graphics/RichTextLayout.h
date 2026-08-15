/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/RichText.h>
#include <UXCpp/Graphics/FontManager.h>
#include <string>
#include <vector>
#include <memory>

namespace uxcpp::graphics {

/**
 * @brief A layout result for a rich text document, containing lines and glyph positions.
 */
struct LayoutLine {
    std::vector<ShapedGlyph> glyphs;
    float width;
    float height;
    float baselineY;
};

class RichTextLayout {
public:
    RichTextLayout(const RichTextDocument& doc, float maxWidth, ITextShaper& shaper) 
        : m_doc(doc), m_maxWidth(maxWidth), m_shaper(shaper) {
        layout();
    }

    const std::vector<LayoutLine>& getLines() const { return m_lines; }
    float getTotalHeight() const {
        float total = 0;
        for (const auto& line : m_lines) total += line.height;
        return total;
    }

private:
    void layout() {
        m_lines.clear();
        
        // Simplified wrapping logic for rich text spans
        std::string currentBuffer;
        float currentLineWidth = 0;
        float currentLineHeight = 0;

        for (const auto& span : m_doc.getSpans()) {
            // In a real implementation, we'd shape the span and break it across lines
            // For this slice, we treat each span as a block or wrap it simply
            ShapedLine shaped = m_shaper.shapeText(span.text, span.fontSize, span.bold, span.italic);
            
            if (currentLineWidth + shaped.width > m_maxWidth) {
                // Flush current line
                flushLine(currentBuffer, currentLineWidth, currentLineHeight);
                currentBuffer = "";
                currentLineWidth = 0;
                currentLineHeight = 0;
            }

            currentBuffer += span.text;
            currentLineWidth += shaped.width;
            currentLineHeight = std::max(currentLineHeight, shaped.height);
        }

        if (!currentBuffer.empty()) {
            flushLine(currentBuffer, currentLineWidth, currentLineHeight);
        }
    }

    void flushLine(const std::string& text, float width, float height) {
        // Re-shape the final line to get exact glyphs
        // This is a simplification; real layout happens during wrapping
        ShapedLine shaped = m_shaper.shapeText(text, 16.0f, false, false);
        m_lines.push_back({shaped.glyphs, width, height, height * 0.8f});
    }

    const RichTextDocument& m_doc;
    float m_maxWidth;
    ITextShaper& m_shaper;
    std::vector<LayoutLine> m_lines;
};

} // namespace uxcpp::graphics
