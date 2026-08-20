/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Services/Settings.h>

namespace uxcpp::services {

void SettingsImpl::set(const std::string& key, const std::string& value) {
    m_data[key] = value;
}

std::optional<std::string> SettingsImpl::get(const std::string& key) {
    if (m_data.count(key)) return m_data[key];
    return std::nullopt;
}

bool SettingsImpl::save() {
    // Mock implementation of saving to disk
    return true;
}

bool SettingsImpl::load() {
    // Mock implementation of loading from disk
    return true;
}

} // namespace uxcpp::services