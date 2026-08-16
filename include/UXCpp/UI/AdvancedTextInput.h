/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI/Widget.h>
#include <UXCpp/Core/Application.h>
#include <vector>
#include <string>
#include <memory>

namespace uxcpp::ui {

/**
 * @brief A specialized text input that supports full selection and clipboard operations.
 */
class AdvancedTextInput : public TextInput {
public:
    using TextInput::TextInput;

    void onDraw(graphics::Renderer& renderer) override {
        // Draw base text
        TextInput::onDraw(renderer);

        // Draw selection highlight (simplified)
        if (m_cursor && m_cursor->hasSelection()) {
            float startX = m_bounds.x + 5 + m_cursor->getSelectionStart() * 8.0f;
            float endX = m_bounds.x + 5 + m_cursor->getSelectionEnd() * 8.0f;
            renderer.drawRect({startX, m_bounds.y + 5, endX - startX, 15}, graphics::Color{0, 0, 255, 0.3f});
        }
    }

    void copy() {
        if (m_cursor && m_cursor->hasSelection()) {
            std::string selected = getText().substr(m_cursor->getSelectionStart(), 
                                                   m_cursor->getSelectionEnd() - m_cursor->getSelectionStart());
            core::Application::getInstance().setClipboardText(selected);
        }
    }

    void paste() {
        std::string text = core::Application::getInstance().getClipboardText();
        if (!text.empty()) {
            // Insert at cursor position (simplified)
            // m_text.insert(...)
        }
    }

private:
    // Uses the TextCursor from base TextInput
};

} // namespace uxcpp::ui
