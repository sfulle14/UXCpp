/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI/Widget.h>
#include <UXCpp/Core/InputManager.h>
#include <UXCpp/Core/IMEBridge.h>
#include <UXCpp/Graphics/TextCursor.h>
#include <memory>

namespace uxcpp::ui {

class TextInput : public Widget {
public:
    TextInput(std::string name, std::string placeholder = "") 
        : Widget(std::move(name), "textinput"), m_placeholder(std::move(placeholder)) {
        m_cursor = std::make_unique<graphics::TextCursor>(0);
        setupIME();
    }

    void setPlaceholder(const std::string& placeholder) { m_placeholder = placeholder; }

    void onDraw(graphics::Renderer& renderer) override {
        // Draw background
        renderer.drawRect(m_bounds, getStyle().backgroundColor);
        
        // Handle IME composition overlay
        std::string compText = core::IMEBridge::getInstance().getCurrentComposition();
        
        // Simplified: draw text + composition
        std::string display = m_text.empty() && compText.empty() ? m_placeholder : (m_text + compText);
        graphics::Color color = (m_text.empty() && compText.empty()) 
                                ? graphics::Color{0.7f, 0.7f, 0.7f, 1.0f} 
                                : getStyle().foregroundColor;
        
        renderer.drawText({m_bounds.x + 5, m_bounds.y + 5}, display, color);

        // Draw Caret if focused
        if (isFocused()) {
            float caretX = m_bounds.x + 5 + (m_text.length() + compText.length()) * 8.0f; // Rough estimate
            renderer.drawRect({caretX, m_bounds.y + 5, 2, 15}, graphics::Color::Black());
        }
    }

    bool onPointerDown(graphics::Point p) override {
        if (m_bounds.contains(p)) {
            core::InputManager::getInstance().setFocus(shared_from_this());
            return true;
        }
        return false;
    }

    bool onKeyDown(int key) override {
        if (key >= 32 && key <= 126) {
            m_text += static_cast<char>(key);
            m_cursor->setPosition(m_text.length());
            return true;
        } else if (key == 8 && !m_text.empty()) { // Backspace
            m_text.pop_back();
            m_cursor->setPosition(m_text.length());
            return true;
        } else if (key == 13) { // Enter
            m_text += '\n';
            m_cursor->setPosition(m_text.length());
            return true;
        }
        return false;
    }

    void copyText() {
        if (m_text.empty()) return;
        core::Application::getInstance().setClipboardText(m_text);
    }

    void pasteText() {
        std::string text = core::Application::getInstance().getClipboardText();
        if (!text.empty()) {
            m_text += text;
            m_cursor->setPosition(m_text.length());
        }
    }

    const std::string& getText() const { return m_text; }

private:
    void setupIME() {
        core::IMEBridge::getInstance().setCommitCallback([this](const std::string& text) {
            m_text += text;
            m_cursor->setPosition(m_text.length());
        });
    }

    std::string m_text;
    std::string m_placeholder;
    std::unique_ptr<graphics::TextCursor> m_cursor;
};

} // namespace uxcpp::ui
