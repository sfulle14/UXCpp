/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <thread>
#include <atomic>

namespace uxcpp::services {

enum class FileEvent { Created, Modified, Deleted, Renamed };

struct FileSystemEvent {
    FileEvent type;
    std::string path;
};

class FileSystemWatcher {
public:
    using EventCallback = std::function<void(const FileSystemEvent&)>;

    virtual ~FileSystemWatcher() = default;

    /**
     * @brief Starts watching a directory for changes.
     */
    virtual bool watchDirectory(const std::string& path, EventCallback callback) = 0;

    /**
     * @brief Stops watching all directories.
     */
    virtual void stop() = 0;
};

class FileSystemWatcherImpl : public FileSystemWatcher {
public:
    bool watchDirectory(const std::string& path, EventCallback callback) override {
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

    void stop() override {
        m_running = false;
        if (m_worker.joinable()) m_worker.join();
    }

    ~FileSystemWatcherImpl() override {
        stop();
    }

private:
    std::atomic<bool> m_running{false};
    std::thread m_worker;
};

} // namespace uxcpp::services