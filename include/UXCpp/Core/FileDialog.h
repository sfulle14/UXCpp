/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>

namespace uxcpp::core {

class FileDialog {
public:
    static std::string openFile(const std::string& title, const std::vector<std::string>& filters = {});
    static std::string saveFile(const std::string& title, const std::vector<std::string>& filters = {});
};

} // namespace uxcpp::core
