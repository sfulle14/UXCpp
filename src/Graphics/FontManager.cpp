/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Graphics/FontManager.h>
#include <iostream>
#include <map>

namespace uxcpp::graphics {

bool FontManager::loadFont(const std::string& family, const std::string& path) {
    std::cout << "[UXCpp] Loading font " << family << " from " << path << std::endl;
    // Real implementation would use FreeType or OS APIs here.
    return true;
}

float FontManager::measureString(const std::string& text, const FontFace& face) {
    // Simplified: assume average char width is 0.6 * size
    return static_cast<float>(text.length()) * (face.size * 0.6f);
}

} // namespace uxcpp::graphics
