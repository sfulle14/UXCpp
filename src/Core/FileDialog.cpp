/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Core/FileDialog.h>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#include <algorithm>
#include <sstream>
#else
#include <iostream>
#endif

namespace uxcpp::core {

#ifdef _WIN32
static std::string buildFilterString(const std::vector<std::string>& filters) {
    if (filters.empty()) {
        return "All Files\0*.*\0\0";
    }

    std::string result;
    for (const auto& filter : filters) {
        result += filter;
        result += '\0';
        result += "*.*";
        result += '\0';
    }
    result += '\0';
    return result;
}

static std::string openDialog(bool save, const std::string& title, const std::vector<std::string>& filters) {
    char fileName[MAX_PATH] = {0};
    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = title.c_str();
    std::string filterString = buildFilterString(filters);
    ofn.lpstrFilter = filterString.c_str();
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    BOOL result = save ? GetSaveFileNameA(&ofn) : GetOpenFileNameA(&ofn);
    return result ? std::string(fileName) : std::string{};
}
#endif

std::string FileDialog::openFile(const std::string& title, const std::vector<std::string>& filters) {
#ifdef _WIN32
    return openDialog(false, title, filters);
#else
    (void)title;
    (void)filters;
    return {};
#endif
}

std::string FileDialog::saveFile(const std::string& title, const std::vector<std::string>& filters) {
#ifdef _WIN32
    return openDialog(true, title, filters);
#else
    (void)title;
    (void)filters;
    return {};
#endif
}

} // namespace uxcpp::core