/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI/Widget.h>
#include <UXCpp/Graphics/ImageCache.h>
#include <memory>
#include <string>

namespace uxcpp::ui {

class ImageWidget : public Widget {
public:
    explicit ImageWidget(std::string name, const std::string& imagePath) 
        : Widget(std::move(name), "imagewidget"), m_imagePath(imagePath) {
        m_image = graphics::ImageCache::getInstance().loadImage(imagePath);
    }

    void setImage(const std::string& path) {
        m_imagePath = path;
        m_image = graphics::ImageCache::getInstance().loadImage(path);
    }

    void onDraw(graphics::Renderer& renderer) override {
        if (!m_image) return;

        // Simulation of image rendering since Renderer is primitive
        renderer.drawRect(m_bounds, graphics::Color{0.5f, 0.5f, 0.5f, 1.0f});
        renderer.drawText({m_bounds.x + 5, m_bounds.y + 5}, "[Image: " + m_imagePath + "]", graphics::Color::White());
    }

private:
    std::string m_imagePath;
    std::shared_ptr<graphics::Image> m_image;
};

} // namespace uxcpp::ui
