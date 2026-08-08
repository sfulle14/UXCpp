/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI\Widget.h>
#include <UXCpp/Graphics/Texture.h>

namespace uxcpp::ui {

class ImageWidget : public Widget {
public:
    ImageWidget(std::string name, std::shared_ptr<graphics::Texture> texture) 
        : Widget(std::move(name), "image"), m_texture(std::move(texture)) {}

    void onDraw(graphics::Renderer& renderer) override {
        // We would need a drawTexture method in the Renderer interface
        // For now, we simulate it.
    }

private:
    std::shared_ptr<graphics::Texture> m_texture;
};

} // namespace uxcpp::ui
