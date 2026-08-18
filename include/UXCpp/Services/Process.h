/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <future>
#include <memory>

namespace uxcpp::services {

struct ProcessResult {
    int exitCode;
    std::string stdOut;
    std::string stdErr;
};

class Process {
public:
    virtual ~Process() = default;

    /**
     * @brief Launches a child process.
     */
    virtual bool launch(const std::string& command, const std::vector<std::string>& args) = 0;

    /**
     * @brief Waits for the process to exit and returns the result.
     */
    virtual std::future<ProcessResult> waitForExit() = 0;

    /**
     * @brief Terminates the process immediately.
     */
    virtual void terminate() = 0;
};

class ProcessImpl : public Process {
public:
    bool launch(const std::string& command, const std::vector<std::string>& args) override {
        m_launched = true;
        return true;
    }

    std::future<ProcessResult> waitForExit() override {
        return std::async(std::launch::async, [this]() {
            // Mock implementation.
            return ProcessResult{0, "Mock output", ""};
        });
    }

    void terminate() override {
        m_launched = false;
    }

private:
    bool m_launched = false;
};

} // namespace uxcpp::services