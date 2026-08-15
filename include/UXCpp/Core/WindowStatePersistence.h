/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>

namespace uxcpp::core {

/**
 * @brief Abstract interface for native window state persistence.
 */
class WindowStatePersistence {
public:
    virtual ~WindowStatePersistence() = default;
    virtual void saveState(const std::string& windowId, int x, int y, int w, int h) = 0;
    virtual bool loadState(const std::string& windowId, int& x, int& y, int& w, int& h) = 0;
};

/**
 * @brief Windows-specific implementation of state persistence.
 */
class Win32WindowStatePersistence : public WindowStatePersistence {
public:
    void saveState(const std::string& windowId, int x, int y, int w, int h) override {
        // Real implementation would use Registry or .ini files
    }

    bool loadState(const std::string& windowId, int& x, int& y, int& w, int& h) override {
        return false; // Default to initial size
    }
};

} // namespace uxcpp::core
