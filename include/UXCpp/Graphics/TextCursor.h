/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <algorithm>

namespace uxcpp::graphics {

/**
 * @brief Manages text selection and caret position.
 */
class TextCursor {
public:
    TextCursor(int length = 0) : m_selectionStart(0), m_selectionEnd(0), m_textLength(length) {}

    void setPosition(int pos) {
        m_selectionStart = std::clamp(pos, 0, m_textLength);
        m_selectionEnd = m_selectionStart;
    }

    void selectRange(int start, int end) {
        m_selectionStart = std::clamp(start, 0, m_textLength);
        m_selectionEnd = std::clamp(end, 0, m_textLength);
    }

    int getCaretPosition() const {
        return (m_selectionStart == m_selectionEnd) ? m_selectionStart : m_selectionEnd;
    }

    bool hasSelection() const {
        return m_selectionStart != m_selectionEnd;
    }

    int getSelectionStart() const { return std::min(m_selectionStart, m_selectionEnd); }
    int getSelectionEnd() const { return std::max(m_selectionStart, m_selectionEnd); }

    void setLength(int length) { m_textLength = length; }

private:
    int m_selectionStart;
    int m_selectionEnd;
    int m_textLength;
};

} // namespace uxcpp::graphics
