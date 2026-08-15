/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI/Widget.h>
#include <UXCpp/Graphics/RichText.h>
#include <UXCpp/Graphics/RichTextLayout.h>
#include <UXCpp/Graphics/PlatformTextShaper.h>
#include <memory>

namespace uxcpp::ui {

class RichTextLabel : public Widget {
public:
    explicit RichTextLabel(std::string name) 
        : Widget(std::move(name), "rich_text_label"), m_shaper(std::make_unique<graphics::PlatformTextShaper>()) {}

    void setDocument(const graphics::RichTextDocument& doc) {
        m_doc = doc;
        updateLayout();
    }

    void onDraw(graphics::Renderer& renderer) override {
        if (!m_layout) return;

        float currentY = m_bounds.y;
        for (const auto& line : m_layout->getLines()) {
            float currentX = m_bounds.x;
            for (const auto& glyph : line.glyphs) {
                std::string s(1, static_cast<char>(glyph.codepoint));
                renderer.drawText({currentX + glyph.xOffset, currentY + glyph.yOffset}, s, graphics::Color::Black());
                currentX += glyph.advance;
            }
            currentY += line.height;
        }
    }

private:
    void updateLayout() {
        m_layout = std::make_unique<graphics::RichTextLayout>(m_doc, m_bounds.width, *m_shaper);
    }

    graphics::RichTextDocument m_doc;
    std::unique_ptr<graphics::RichTextLayout> m_layout;
    std::unique_ptr<graphics::ITextShaper> m_shaper;
};

} // namespace uxcpp::ui
