/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/Renderer.h>
#include <string>

namespace uxcpp::graphics {

/**
 * @brief Represents a GPU texture resource.
 */
class Texture {
public:
    virtual ~Texture() = default;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
};

/**
 * @brief Interface for loading images from disk into textures.
 */
class ImageLoader {
public:
    virtual ~ImageLoader() = default;
    virtual std::unique_ptr<Texture> loadFromFile(const std::string& path) = 0;
};

} // namespace uxcpp::graphics
