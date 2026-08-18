/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <functional>
#include <memory>
#include <UXCpp/UI/Widget.h>

namespace uxcpp::ui {

class LiveReloadEngine {
public:
    using UIUpdateCallback = std::function<void()>;

    /**
     * @brief Registers a file to watch for changes to trigger a UI reload.
     */
    void watchUIFile(const std::string& filePath, UIUpdateCallback callback) {
        m_callbacks[filePath] = callback;
        // In real implementation, this would integrate with services::FileSystemWatcher
    }

    /**
     * @brief Manually triggers a reload of the UI.
     */
    void triggerReload(const std::string& filePath) {
        if (m_callbacks.count(filePath)) {
            m_callbacks[filePath]();
        }
    }

private:
    std::map<std::string, UIUpdateCallback> m_callbacks;
};

} // namespace uxcpp::ui