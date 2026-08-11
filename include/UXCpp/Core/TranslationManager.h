/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace uxcpp::core {

/**
 * @brief Simple Internationalization manager for translating UI strings.
 */
class TranslationManager {
public:
    static TranslationManager& getInstance() {
        static TranslationManager instance;
        return instance;
    }

    void loadLanguage(const std::string& langCode, const std::unordered_map<std::string, std::string>& translations) {
        m_languages[langCode] = translations;
        if (m_currentLang.empty()) m_currentLang = langCode;
    }

    void setLanguage(const std::string& langCode) {
        m_currentLang = langCode;
    }

    /**
     * @brief Translates a key into the current language.
     */
    std::string translate(const std::string& key) const {
        auto itLang = m_languages.find(m_currentLang);
        if (itLang != m_languages.end()) {
            auto itKey = itLang->second.find(key);
            if (itKey != itLang->second.end()) {
                return itKey->second;
            }
        }
        return key; // Fallback to the key itself
    }

private:
    TranslationManager() = default;
    std::string m_currentLang;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_languages;
};

} // namespace uxcpp::core
