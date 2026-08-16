/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>

namespace uxcpp::ui {

/**
 * @brief A specialized text input that supports full selection, caret navigation, and clipboard hooks.
 */
class TextEditor : public Widget {
public:
    explicit TextEditor(std::string name) 
        : Widget(std::move(name), "texteditor"), m_cursorPos(0) {}

    void setText(const std::string& text) {
        m_text = text;
        m_cursorPos = (int)m_text.length();
    }

    std::string getText() const { return m_text; }

    // Caret & Selection
    void setCaretPosition(int pos) {
        m_cursorPos = std::clamp(pos, 0, (int)m_text.length());
    }

    int getCaretPosition() const { return m_cursorPos; }

    void insertText(const std::string& text) {
        m_text.insert(m_cursorPos, text);
        m_cursorPos += (int)text.length();
    }

    void deleteBack() {
        if (m_cursorPos > 0) {
            m_text.erase(m_cursorPos - 1, 1);
            m_cursorPos--;
        }
    }

    // Clipboard Hooks
    void copy() {
        core::Application::getInstance().setClipboardText(m_text); // Simplified: copies all
    }

    void paste() {
        std::string text = core::Application::getInstance().getClipboardText();
        insertText(text);
    }

    void onDraw(graphics::Renderer& renderer) override {
        // Draw background
        renderer.drawRect(m_bounds, getStyle().backgroundColor);
        
        // Render text (simplified: single line for now)
        renderer.drawText({m_bounds.x + 5, m_bounds.y + 5}, m_text, getStyle().foregroundColor);

        // Draw Caret
        if (isFocused()) {
            float caretX = m_bounds.x + 5 + (m_cursorPos * 8.0f); // Rough estimate
            renderer.drawRect({caretX, m_bounds.y + 5, 2, 15}, graphics::Color::Black());
        }
    }

    bool onKeyDown(int key) override {
        if (key >= 32 && key <= 126) {
            insertText(std::string(1, static_cast<char>(key)));
            return true;
        } else if (key == 8) { // Backspace
            deleteBack();
            return true;
        } else if (key == 264 || key == 261) { // Left/Up
            setCaretPosition(m_cursorPos - 1);
            return true;
        } else if (key == 265 || key == 262) { // Right/Down
            setCaretPosition(m_cursorPos + 1);
            return true;
        }
        return false;
    }

private:
    std::string m_text;
    int m_cursorPos;
};

} // namespace uxcpp::ui
