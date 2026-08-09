/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <functional>
#include <vector>
#include <algorithm>

namespace uxcpp::core {

/**
 * @brief A reactive property that notifies observers when its value changes.
 * 
 * Similar to Qt's QProperty or observable patterns.
 */
template <typename T>
class Property {
public:
    explicit Property(T initialValue) : m_value(std::move(initialValue)) {}

    /**
     * @brief Sets the value and notifies all observers if the value changed.
     */
    void set(const T& newValue) {
        if (m_value != newValue) {
            m_value = newValue;
            notify();
        }
    }

    const T& get() const { return m_value; }

    /**
     * @brief Connects a callback that is triggered whenever the property changes.
     */
    void observe(std::function<void(const T&)> observer) {
        m_observers.push_back(observer);
    }

private:
    void notify() {
        for (auto& obs : m_observers) {
            obs(m_value);
        }
    }

    T m_value;
    std::vector<std::function<void(const T&)>> m_observers;
};

} // namespace uxcpp::core
