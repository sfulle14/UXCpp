/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Core/NativeMenuService.h>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <iostream>
#endif

namespace uxcpp::core {

void NativeMenuService::showContextMenu(int x, int y, const std::vector<NativeMenuItem>& items) {
#ifdef _WIN32
    HMENU hMenu = CreatePopupMenu();
    for (size_t i = 0; i < items.size(); ++i) {
        // In a real implementation, we'd store the actions in a map indexed by ID
        CreateMenuItemA(hMenu, items[i].title.c_str(), MI_STRING);
    }

    // Note: To actually execute the action, we would need to handle WM_COMMAND 
    // in the main window proc. For this slice, we implement the native call.
    TRACKMENU_INFO tmi = { sizeof(TRACKMENU_INFO) };
    tmi. hwnd = GetActiveWindow();
    tmi. uPosition = TPM_LEFTALIGN;
    tmi. pt = { x, y };
    tmi. fDefault = 0;

    TrackPopupMenu(&tmi, 0, 0, 0, 0, hInstance); // Simplified hInstance
#else
    std::cout << "[NativeMenuService] Context menu requested at (" << x << "," << y << ") with " << items.size() << " items." << std::endl;
#endif}

// Helper for Win32 (simplified)
#ifdef _WIN32
void CreateMenuItemA(HMENU hMenu, const char* text, UINT flags) {
    AppendMenuA(hMenu, flags, 0, text);
}
#endif

} // namespace uxcpp::core
