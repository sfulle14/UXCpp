/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <functional>

namespace uxcpp::graphics {

/**
 * @brief Represents a single styled segment of text.
 */
struct TextSpan {
    std::string text;
    float fontSize = 16.0f;
    bool bold = false;
    bool italic = false;
    uint32_t color = 0xFFFFFFFF; // RGBA
    bool isLink = false;
};

/**
 * @brief A rich text document consisting of multiple styled spans.
 */
class RichTextDocument {
public:
    void append(const std::string& text, float size = 16.0f, bool bold = false, uint32_t color = 0xFFFFFFFF) {
        m_spans.push_back({text, size, bold, false, color, false});
    }

    void addSpan(TextSpan span) {
        m_spans.push_back(std::move(span));
    }

    const std::vector<TextSpan>& getSpans() const { return m_spans; }
    void clear() { m_spans.clear(); }

private:
    std::vector<TextSpan> m_spans;
};

/**
 * @brief Result of a shaping operation for a single line.
 */
struct ShapedGlyph {
    uint32_t codepoint;
    float xOffset;
    float yOffset;
    float advance;
};

struct ShapedLine {
    std::vector<ShapedGlyph> glyphs;
    float width;
    float height;
};

/**
 * @brief Interface for text shaping and BiDi layout.
 */
class ITextShaper {
public:
    virtual ~ITextShaper() = default;

    /**
     * @brief Shapes a string into glyphs, handling BiDi and ligatures.
     */
    virtual ShapedLine shapeText(const std::string& text, float fontSize, bool bold, bool italic) = 0;
};

} // namespace uxcpp::graphics
