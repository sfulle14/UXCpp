/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <mutex>

namespace uxcpp::core {

/**
 * @brief A type-safe signal implementation that allows multiple slots to be connected.
 * 
 * This is a clean-room implementation of a Signal/Slot mechanism, avoiding any MOC dependency.
 */
template <typename... Args>
class Signal {
public:
    using Slot = std::function<void(Args...)>;

    /**
     * @brief Connects a slot to the signal.
     * @param slot The callable to be invoked when the signal is emitted.
     * @return A connection ID that can be used to disconnect the slot.
     */
    size_t connect(Slot slot) {
        std::lock_guard<std::mutex> lock(m_mutex);
        size_t id = m_nextId++;
        m_slots.emplace_back(Connection{id, std::move(slot)});
        return id;
    }

    /**
     * @brief Disconnects a slot using its connection ID.
     * @param id The connection ID returned by connect().
     */
    void disconnect(size_t id) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_slots.erase(
            std::remove_if(m_slots.begin(), m_slots.end(), 
                [id](const Connection& conn) { return conn.id == id; }),
            m_slots.end()
        );
    }

    /**
     * @brief Emits the signal, calling all connected slots with the provided arguments.
     */
    void emit(Args... args) {
        std::vector<Slot> slotsCopy;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            slotsCopy.reserve(m_slots.size());
            for (const auto& conn : m_slots) {
                slotsCopy.push_back(conn.slot);
            }
        }

        for (const auto& slot : slotsCopy) {
            slot(args...);
        }
    }

    /**
     * @brief Disconnects all slots from the signal.
     */
    void disconnectAll() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_slots.clear();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_slots.size();
    }

private:
    struct Connection {
        size_t id;
        Slot slot;
    };

    mutable std::mutex m_mutex;
    std::vector<Connection> m_slots;
    size_t m_nextId = 0;
};

} // namespace uxcpp::core
