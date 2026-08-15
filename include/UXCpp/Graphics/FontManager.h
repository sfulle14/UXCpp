/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace uxcpp::graphics {

/**
 * @brief Represents a font face and its properties.
 */
struct FontFace {
    std::string family;
    int size;
    bool bold = false;
    bool italic = false;

    bool operator<(const FontFace& other) const {
        if (family != other.family) return family < other.family;
        if (size != other.size) return size < other.size;
        if (bold != other.bold) return bold < other.bold;
        return italic < other.italic;
    }

    bool operator==(const FontFace& other) const {
        return family == other.family && size == other.size && 
               bold == other.bold && italic == other.italic;
    }
};

/**
 * @brief Manages font loading, caching and glyph metrics.
 */
class FontManager {
public:
    static FontManager& getInstance() {
        static FontManager instance;
        return instance;
    }

    /**
     * @brief Loads a font from file or system.
     */
    bool loadFont(const std::string& family, const std::string& path);

    /**
     * @brief Returns the width of a string for a given face.
     */
    float measureString(const std::string& text, const FontFace& face) const;

    /**
     * @brief Gets the height/ascent/descent for a font face.
     */
    float getLineHeight(const FontFace& face) const {
        return static_cast<float>(face.size) * 1.2f; // Simplified fallback
    }

private:
    FontManager() = default;
    // In a real implementation, this would hold FreeType or OS font handles.
};

} // namespace uxcpp::graphics
