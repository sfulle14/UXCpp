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
    bool watchDirectory(const std::string& path, EventCallback callback) override;
    void stop() override;

    ~FileSystemWatcherImpl() override {
        stop();
    }

private:
    std::atomic<bool> m_running{false};
    std::thread m_worker;
};

} // namespace uxcpp::services