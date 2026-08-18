/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>

namespace uxcpp::plugins {

/**
 * @brief Versioning for the Plugin ABI to ensure compatibility.
 */
struct PluginApiVersion {
    uint32_t major = 1;
    uint32_t minor = 0;
    uint32_t patch = 0;

    bool isCompatible(const PluginApiVersion& other) const {
        return major == other.major; // Simple major-version compatibility check
    }
};

/**
 * @brief Base interface for all UXCpp plugins.
 */
class IPlugin {
public:
    virtual ~IPlugin() = default;

    /**
     * @brief Returns the unique identifier of the plugin.
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Returns the version of the plugin.
     */
    virtual std::string getVersion() const = 0;

    /**
     * @brief Called by the host when the plugin is loaded.
     * Use this to register widgets, services, or other extensions.
     */
    virtual void onInitialize() = 0;

    /**
     * @brief Called by the host before the plugin is unloaded.
     */
    virtual void onShutdown() = 0;
};

/**
 * @brief Entry point signature for plugin DLLs.
 */
typedef IPlugin* (*CreatePluginFunc)();

} // namespace uxcpp::plugins