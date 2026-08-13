/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Core/Signal.h>
#include <functional>
#include <string>
#include <vector>
#include <map>

namespace uxcpp::core {

/**
 * @brief Represents a keyboard shortcut combination.
 */
struct Shortcut {
    int key;               // The primary key (e.g., ASCII or GLFW key)
    bool ctrl = false;     // Ctrl modifier
    bool shift = false;    // Shift modifier
    bool alt = false;      // Alt modifier

    bool operator<(const Shortcut& other) const {
        if (key != other.key) return key < other.key;
        if (ctrl != other.ctrl) return ctrl < other.ctrl;
        if (shift != other.shift) return shift < other.shift;
        return alt < other.alt;
    }

    bool operator==(const Shortcut& other) const {
        return key == other.key && ctrl == other.ctrl && shift == other.shift && alt == other.alt;
    }
};

/**
 * @brief Manages global and context-specific keyboard shortcuts.
 */
class ShortcutManager {
private:
    struct ShortcutInfo {
        std::function<void()> action;
        std::string description;
    };

public:
    static ShortcutManager& getInstance() {
        static ShortcutManager instance;
        return instance;
    }

    /**
     * @brief Registers a shortcut with a callback.
     * @param shortcut The key combination to trigger the action.
     * @param action The function to execute when the shortcut is triggered.
     * @param description A human-readable description of the shortcut (for tooltips/menus).
     */
    void registerShortcut(const Shortcut& shortcut, std::function<void()> action, std::string description = "") {
        m_shortcuts[shortcut] = {action, std::move(description)};
    }

    /**
     * @brief Unregisters a shortcut.
     */
    void unregisterShortcut(const Shortcut& shortcut) {
        m_shortcuts.erase(shortcut);
    }

    /**
     * @brief Checks if a key combination matches any registered shortcut and executes it.
     * @return true if a shortcut was found and executed.
     */
    bool tryTriggerShortcut(int key, bool ctrl, bool shift, bool alt) {
        Shortcut s{key, ctrl, shift, alt};
        auto it = m_shortcuts.find(s);
        if (it != m_shortcuts.end()) {
            it->second.action();
            return true;
        }
        return false;
    }

    const std::map<Shortcut, ShortcutInfo>& getRegisteredShortcuts() const {
        return m_shortcuts;
    }

    ShortcutManager() = default;
    std::map<Shortcut, ShortcutInfo> m_shortcuts;
};

} // namespace uxcpp::core
