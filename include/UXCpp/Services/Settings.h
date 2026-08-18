/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <map>
#include <memory>
#include <optional>

namespace uxcpp::services {

class Settings {
public:
    virtual ~Settings() = default;

    /**
     * @brief Sets a configuration value.
     */
    virtual void set(const std::string& key, const std::string& value) = 0;

    /**
     * @brief Gets a configuration value.
     */
    virtual std::optional<std::string> get(const std::string& key) = 0;

    /**
     * @brief Saves current settings to disk.
     */
    virtual bool save() = 0;

    /**
     * @brief Loads settings from disk.
     */
    virtual bool load() = 0;
};

class SettingsImpl : public Settings {
public:
    void set(const std::string& key, const std::string& value) override {
        m_data[key] = value;
    }

    std::optional<std::string> get(const std::string& key) override {
        if (m_data.count(key)) return m_data[key];
        return std::nullopt;
    }

    bool save() override { return true; }
    bool load() override { return true; }

private:
    std::map<std::string, std::string> m_data;
};

} // namespace uxcpp::services