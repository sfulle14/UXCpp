/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <UXCpp/Plugins/IPlugin.h>

namespace uxcpp::plugins {

class PluginManager {
public:
    static PluginManager& getInstance() {
        static PluginManager instance;
        return instance;
    }

    /**
     * @brief Loads a plugin from a shared library file.
     */
    bool loadPlugin(const std::string& path) {
        // In a real implementation, this would use LoadLibrary (Windows) or dlopen (Linux).
        // For the clean-room prototype, we simulate the loading process.
        m_loadedPlugins[path] = nullptr; // Simulate successful load
        return true;
    }

    /**
     * @brief Unloads a specific plugin.
     */
    void unloadPlugin(const std::string& path) {
        if (m_loadedPlugins.count(path)) {
            auto plugin = m_loadedPlugins[path];
            if (plugin) plugin->onShutdown();
            m_loadedPlugins.erase(path);
        }
    }

    /**
     * @brief Returns a list of all currently loaded plugins.
     */
    const std::map<std::string, std::unique_ptr<IPlugin>>& getLoadedPlugins() const {
        return m_loadedPlugins;
    }

    /**
     * @brief Registers a plugin manually (for static linking or testing).
     */
    void registerPlugin(const std::string& id, std::unique_ptr<IPlugin> plugin) {
        plugin->onInitialize();
        m_loadedPlugins[id] = std::move(plugin);
    }

private:
    PluginManager() = default;
    std::map<std::string, std::unique_ptr<IPlugin>> m_loadedPlugins;
};

} // namespace uxcpp::plugins