/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>

namespace uxcpp::core {

/**
 * @brief Easing functions for smooth animations.
 */
enum class Easing {
    Linear,
    EaseInQuad,
    EaseOutQuad,
    EaseInOutQuad,
    EaseInCubic,
    EaseOutCubic,
    EaseInOutCubic
};

class EasingFunctions {
public:
    static float apply(Easing easing, float t) {
        switch (easing) {
            case Easing::Linear: return t;
            case Easing::EaseInQuad: return t * t;
            case Easing::EaseOutQuad: return t * (2 - t);
            case Easing::EaseInOutQuad: return t < 0.5f ? 2 * t * t : -1 + (4 - 2 * t) * t;
            case Easing::EaseInCubic: return t * t * t;
            case Easing::EaseOutCubic: return (--t) * t * t + 1;
            case Easing::EaseInOutCubic: return t < 0.5f ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1;
            default: return t;
        }
    }
};

/**
 * @brief Animates a value over time.
 */
template <typename T>
class PropertyAnimation {
public:
    PropertyAnimation(T start, T end, float durationSeconds, Easing easing = Easing::Linear)
        : m_start(start), m_end(end), m_duration(durationSeconds), m_easing(easing), m_elapsed(0.0f), m_finished(false) {}

    void update(float deltaTime) {
        if (m_finished) return;

        m_elapsed += deltaTime;
        float t = std::clamp(m_elapsed / m_duration, 0.0f, 1.0f);
        float easedT = EasingFunctions::apply(m_easing, t);
        
        // Linear interpolation: start + (end - start) * t
        m_currentValue = m_start + (m_end - m_start) * easedT;

        if (t >= 1.0f) m_finished = true;
    }

    T getValue() const { return m_currentValue; }
    bool isFinished() const { return m_finished; }

private:
    T m_start, m_end, m_currentValue;
    float m_duration, m_elapsed;
    Easing m_easing;
    bool m_finished;
};

} // namespace uxcpp::core
