/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Services/FileSystemWatcher.h>
#include <thread>
#include <chrono>

namespace uxcpp::services {

bool FileSystemWatcherImpl::watchDirectory(const std::string& path, EventCallback callback) {
    m_running = true;
    m_worker = std::thread([this, path, callback]() {
        while (m_running) {
            // Mock polling for changes.
            std::this_thread::sleep_for(std::chrono::seconds(2));
            callback({FileEvent::Modified, path + "/mock_file.txt"});
        }
    });
    return true;
}

void FileSystemWatcherImpl::stop() {
    m_running = false;
    if (m_worker.joinable()) m_worker.join();
}

} // namespace uxcpp::services