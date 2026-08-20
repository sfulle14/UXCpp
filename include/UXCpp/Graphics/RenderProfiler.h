/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <map>
#include <iostream>

namespace uxcpp::graphics {

/**
 * @brief Tracks timing for specific rendering stages to identify bottlenecks.
 */
class RenderProfiler {
public:
    static RenderProfiler& getInstance() {
        static RenderProfiler instance;
        return instance;
    }

    struct StageTiming {
    struct StageTiming {
        std::string name;
        double durationMs;
    };

    void startStage(const std::string& stageName) {
        m_stages[stageName] = std::chrono::high_resolution_clock::now();
    }

    void endStage(const std::string& stageName) {
        auto end = std::chrono::high_resolution_clock::now();
        if (m_stages.count(stageName)) {
            auto start = m_stages[stageName];
            double duration = std::chrono::duration<double, std::milli>(end - start).count();
            m_frameTimings[stageName] = duration;
        }
    }

    const std::map<std::string, double>& getFrameTimings() const {
        return m_frameTimings;
    }

    void clear() {
        m_frameTimings.clear();
    }

private:
    std::map<std::string, std::chrono::time_point<std::chrono::high_resolution_clock>> m_stages;
    std::map<std::string, double> m_frameTimings;
};

} // namespace uxcpp::graphics