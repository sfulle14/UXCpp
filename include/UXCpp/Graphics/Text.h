/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>

namespace uxcpp::graphics {

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
