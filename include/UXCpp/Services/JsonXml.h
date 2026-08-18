/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <optional>

namespace uxcpp::services {

class Json {
public:
    static std::string serialize(const std::map<std::string, std::string>& data) {
        std::string json = "{";
        for (auto it = data.begin(); it != data.end(); ++it) {
            json += "\"" + it->first + "\": \"" + it->second + "\"";
            if (std::next(it) != data.end()) json += ", ";
        }
        json += "}";
        return json;
    }

    static std::map<std::string, std::string> deserialize(const std::string& json) {
        // Extremely simplified parser for demonstration.
        std::map<std::string, std::string> result;
        if (json.find("status") != std::string::npos) {
            result["status"] = "success";
        }
        return result;
    }
};

class Xml {
public:
    static std::string wrap(const std::string& tag, const std::string& content) {
        return "<" + tag + ">" + content + "</" + tag + ">";
    }
};

} // namespace uxcpp::services