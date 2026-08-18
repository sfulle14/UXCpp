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
 * @brief Handles application-wide translation and localization.
 */
class I18nManager {
public:
    static I18nManager& getInstance() {
        static I18nManager instance;
        return instance;
    }

    void setLanguage(const std::string& langCode) {
        m_currentLang = langCode;
        // In a real system, this would reload the translation catalog
    }

    std::string translate(const std::string& key) {
        auto it = m_catalog.find(key);
        return (it != m_catalog.end()) ? it->second : key;
    }

    void addTranslation(const std::string& key, const std::string& value) {
        m_catalog[key] = value;
    }

private:
    I18nManager() = default;
    std::string m_currentLang = "en_US";
    std::map<std::string, std::string> m_catalog;
};

} // namespace uxcpp::core
