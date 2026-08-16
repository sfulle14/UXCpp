/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace uxcpp::core {

struct NativeMenuItem {
    std::string title;
    std::function<void()> action;
};

/**
 * @brief Provides access to native OS context menus.
 */
class NativeMenuService {
public:
    static NativeMenuService& getInstance() {
        static NativeMenuService instance;
        return instance;
    }

    /**
     * @brief Displays a native context menu at the specified screen coordinates.
     */
    void showContextMenu(int x, int y, const std::vector<NativeMenuItem>& items);

private:
    NativeMenuService() = default;
};

} // namespace uxcpp::core
