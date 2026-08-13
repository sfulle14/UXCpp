/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/Renderer.h>
#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace uxcpp::graphics {

enum class TextAlign {
    Left,
    Center,
    Right,
    Justify
};

struct TextStyle {
    Color color = Color::Black();
    float size = 16.0f;
    bool bold = false;
    bool italic = false;
    bool underline = false;
};

struct TextSpan {
    std::string text;
    TextStyle style;
};

class TextDocument {
public:
    void append(std::string text, TextStyle style = {}) {
        m_spans.push_back(TextSpan{std::move(text), style});
    }

    const std::vector<TextSpan>& spans() const { return m_spans; }

    bool empty() const { return m_spans.empty(); }

private:
    std::vector<TextSpan> m_spans;
};

/**
 * @brief Represents a loaded font face.
 */
class Font {
public:
    virtual ~Font() = default;
    virtual float getCharWidth(char32_t codepoint, float size) const = 0;
    virtual float getHeight(float size) const = 0;
};

/**
 * @brief Handles the rasterization and caching of glyphs.
 */
class TextRenderer {
public:
    virtual ~TextRenderer() = default;
    virtual void renderText(const std::string& text, float x, float y, float size, Color color) = 0;
    virtual float measureText(const std::string& text, float size) = 0;
};

} // namespace uxcpp::graphics
