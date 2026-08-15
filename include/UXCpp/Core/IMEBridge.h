/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace uxcpp::core {

/**
 * @brief Handles Input Method Editor (IME) composition and candidate selection.
 */
class IMEBridge {
public:
    static IMEBridge& getInstance() {
        static IMEBridge instance;
        return instance;
    }

    /**
     * @brief Called by the OS when a composition string is updated.
     */
    void onCompositionUpdate(const std::string& compositionText, int caretPos) {
        m_currentComposition = compositionText;
        m_caretPosition = caretPos;
        if (m_onCompositionChanged) m_onCompositionChanged(compositionText, caretPos);
    }

    /**
     * @brief Called by the OS when a candidate is selected.
     */
    void onCandidateSelected(const std::string& finalText) {
        m_currentComposition = "";
        if (m_onCommit) m_onCommit(finalText);
    }

    /**
     * @brief Set callback for composition changes (e.g., to show a temporary underline).
     */
    void setCompositionCallback(std::function<void(const std::string&, int)> cb) {
        m_onCompositionChanged = std::move(cb);
    }

    /**
     * @brief Set callback for final text commitment.
     */
    void setCommitCallback(std::function<void(const std::string&)> cb) {
        m_onCommit = std::move(cb);
    }

    std::string getCurrentComposition() const { return m_currentComposition; }

private:
    IMEBridge() = default;
    std::string m_currentComposition;
    int m_caretPosition = 0;
    std::function<void(const std::string&, int)> m_onCompositionChanged;
    std::function<void(const std::string&)> m_onCommit;
};

} // namespace uxcpp::core
