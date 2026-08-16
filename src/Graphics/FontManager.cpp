/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Graphics/FontManager.h>
#include <iostream>
#include <map>

namespace uxcpp::graphics {

bool FontManager::loadFont(const std::string& family, const std::string& path) {
    m_loadedFonts[family] = path;
    std::cout << "[FontManager] Loaded font " << family << " from " << path << std::endl;
    return true;
}

void FontManager::addFallbackFont(const std::string& family, const std::string& path) {
    m_fallbackChain.push_back({family, path});
    std::cout << "[FontManager] Added fallback font " << family << std::endl;
}

float FontManager::measureString(const std::string& text, const FontFace& face) {
    // In a real implementation, we'd iterate through the fallback chain 
    // if glyphs are missing in the primary face.
    return static_cast<float>(text.length()) * (face.size * 0.6f);
}

} // namespace uxcpp::graphics
