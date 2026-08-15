/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>

namespace uxcpp::core {

/**
 * @brief Manages application resources such as images, fonts, and configuration files.
 */
class ResourceManager {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    /**
     * @brief Loads a resource from the filesystem into the cache.
     */
    bool loadResource(const std::string& id, const std::string& path) {
        m_resourcePaths[id] = path;
        std::cout << "[ResourceManager] Registered resource: " << id << " -> " << path << std::endl;
        return true;
    }

    /**
     * @brief Retrieves the path of a registered resource.
     */
    std::string getResourcePath(const std::string& id) const {
        auto it = m_resourcePaths.find(id);
        return (it != m_resourcePaths.end()) ? it->second : "";
    }

    /**
     * @brief Checks if a resource is registered.
     */
    bool hasResource(const std::string& id) const {
        return m_resourcePaths.find(id) != m_resourcePaths.end();
    }

private:
    ResourceManager() = default;
    std::map<std::string, std::string> m_resourcePaths;
};

} // namespace uxcpp::core
