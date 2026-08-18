/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace uxcpp::graphics {

/**
 * @brief Represents a loaded image resource.
 */
struct Image {
    int width;
    int height;
    std::vector<uint8_t> data; // RGBA pixels
    std::string path;

    Image(int w, int h, const std::string& p) : width(w), height(h), path(p) {
        data.resize(w * h * 4);
    }
};

/**
 * @brief Manages loading and caching of images to avoid redundant disk I/O.
 */
class ImageCache {
public:
    static ImageCache& getInstance() {
        static ImageCache instance;
        return instance;
    }

    /**
     * @brief Loads an image from path or returns a cached version.
     */
    std::shared_ptr<Image> loadImage(const std::string& path) {
        if (m_cache.count(path)) {
            return m_cache[path];
        }

        // In a real implementation, this would use stb_image or similar.
        // For now, we simulate loading a dummy image of varying size based on path length.
        int w = 64 + (path.length() % 128);
        int h = 64 + (path.length() % 128);
        auto img = std::make_shared<Image>(w, h, path);
        
        m_cache[path] = img;
        return img;
    }

    void clearCache() {
        m_cache.clear();
    }

private:
    ImageCache() = default;
    std::map<std::string, std::shared_ptr<Image>> m_cache;
};

} // namespace uxcpp::graphics
