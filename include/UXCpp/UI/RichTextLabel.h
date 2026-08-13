/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI/Widget.h>
#include <UXCpp/Graphics/Text.h>
#include <algorithm>
#include <memory>
#include <string>

namespace uxcpp::ui {

class RichTextLabel : public Widget {
public:
    explicit RichTextLabel(std::string name)
        : Widget(std::move(name), "richtextlabel") {}

    void setDocument(graphics::TextDocument document) {
        m_document = std::move(document);
    }

    void setWrapWidth(float width) {
        m_wrapWidth = std::max(0.0f, width);
    }

    void onDraw(graphics::Renderer& renderer) override {
        float cursorY = m_bounds.y;
        for (const auto& span : m_document.spans()) {
            graphics::TextLayout layout(span.text, m_wrapWidth > 0.0f ? m_wrapWidth : m_bounds.width, span.style.size);
            for (const auto& line : layout.getLines()) {
                renderer.drawText({m_bounds.x, cursorY}, line.text, span.style.color);
                cursorY += layout.getLineHeight();
            }
        }
    }

private:
    graphics::TextDocument m_document;
    float m_wrapWidth = 0.0f;
};

} // namespace uxcpp::ui
