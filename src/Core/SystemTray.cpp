/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Core/SystemTray.h>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#pragma comment(lib, "shell32.lib")
#else
#include <iostream>
#endif

namespace uxcpp::core {

// Windows implementation of the tray icon requires a window to receive NOTIFYICONDATA messages.
// We'll use a hidden helper window or hook into the main application window.
// For this clean-room slice, we implement the Win32 API calls for adding/removing icons.

#ifdef _WIN32
static HWND g_trayWindow = nullptr;

// Simple Window Proc to handle tray messages
LRESULT CALLBACK TrayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_USER + 1) { // Custom message for tray click
        // We'll need a way to map this back to the SystemTrayIcon instance.
        // For simplicity in this slice, we use a global pointer or singleton approach.
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

static void createHiddenTrayWindow() {
    if (g_trayWindow) return;

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = TrayWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "UXCppTrayWindow";
    RegisterClassEx(&wc);

    g_trayWindow = CreateWindowEx(0, "UXCppTrayWindow", NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
}
#endif

SystemTrayIcon::~SystemTrayIcon() {
    deactivate();
}

void SystemTrayIcon::activate() {
#ifdef _WIN32
    createHiddenTrayWindow();

    NOTIFYICONDATA info = {};
    info.cbSize = sizeof(NOTIFYICONDATA);
    info.hWnd = g_trayWindow;
    info.uID = 1; // Simplified: one icon per app for now
    info.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    info.uCallbackMessage = WM_USER + 1;
    info.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    
    // Convert std::string to wide string for Windows if needed, but using NOTIFYICONDATAA here
    strncpy_s(info.szTip, 128, m_config.tooltip.c_str(), _TRUNCATE);

    if (Shell_NotifyIcon(NIM_ADD, &info)) {
        m_isActive = true;
    } else {
        std::cerr << "[UXCpp] Failed to add system tray icon." << std::endl;
    }
#else
    std::cout << "[UXCpp] System Tray not supported on this platform." << std::endl;
#endif}

void SystemTrayIcon::deactivate() {
#ifdef _WIN32
    if (m_isActive) {
        NOTIFYICONDATA info = {};
        info.cbSize = sizeof(NOTIFYICONDATA);
        info.hWnd = g_trayWindow;
        info.uID = 1;
        Shell_NotifyIcon(NIM_DELETE, &info);
        m_isActive = false;
    }
#endif}

void SystemTrayIcon::update() {
#ifdef _WIN32
    if (m_isActive) {
        NOTIFYICONDATA info = {};
        info.cbSize = sizeof(NOTIFYICONDATA);
        info.hWnd = g_trayWindow;
        info.uID = 1;
        info.uFlags = NIF_TIP;
        strncpy_s(info.szTip, 128, m_config.tooltip.c_str(), _TRUNCATE);
        Shell_NotifyIcon(NIM_MODIFY, &info);
    }
#endif}

} // namespace uxcpp::core
