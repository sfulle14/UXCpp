/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Core/MessageBox.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <iostream>
#endif

namespace uxcpp::core {

void MessageBox::show(const std::string& title, const std::string& message, MessageBoxType type) {
#ifdef _WIN32
    UINT flags = MB_OK;
    switch (type) {
        case MessageBoxType::Warning:
            flags |= MB_ICONWARNING;
            break;
        case MessageBoxType::Error:
            flags |= MB_ICONERROR;
            break;
        case MessageBoxType::Information:
        default:
            flags |= MB_ICONINFORMATION;
            break;
    }
    MessageBoxA(nullptr, message.c_str(), title.c_str(), flags);
#else
    (void)type;
    std::cerr << '[' << title << "] " << message << std::endl;
#endif
}

} // namespace uxcpp::core