/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Services/Process.h>
#include <future>

namespace uxcpp::services {

bool ProcessImpl::launch(const std::string& command, const std::vector<std::string>& args) {
    m_launched = true;
    return true;
}

std::future<ProcessResult> ProcessImpl::waitForExit() {
    return std::async(std::launch::async, [this]() {
        return ProcessResult{0, "Mock output", ""};
    });
}

void ProcessImpl::terminate() {
    m_launched = false;
}

} // namespace uxcpp::services