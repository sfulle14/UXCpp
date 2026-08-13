/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <variant>
#include <vector>
#include <memory>
#include <any>

namespace uxcpp::core {

/**
 * @brief Data payload for drag and drop operations.
 * Supports common types like text, files, or internal custom objects.
 */
struct DragDropPayload {
    enum class Type {
        Text,
        Files,
        Custom
    };

    Type type = Type::Text;
    std::any data; // Holds the actual content (e.g., std::string for Text)

    template<typename T>
    T getValue() const {
        return std::any_cast<T>(data);
    }

    template<typename T>
    void setValue(T value) {
        data = value;
    }
};

/**
 * @brief Manages the state and dispatching of drag and drop operations.
 */
class DragDropManager {
public:
    static DragDropManager& getInstance() {
        static DragDropManager instance;
        return instance;
    }

    /**
     * @brief Starts a drag operation.
     */
    void startDrag(std::shared_ptr<class ui::Widget> source, DragDropPayload payload) {
        m_isDragging = true;
        m_dragSource = source;
        m_payload = std::move(payload);
        m_lastTarget = nullptr;
    }

    /**
     * @brief Ends the current drag operation.
     */
    void endDrag() {
        if (m_lastTarget) {
            m_lastTarget->onDragLeave();
        }
        m_isDragging = false;
        m_dragSource = nullptr;
        m_lastTarget = nullptr;
    }

    bool isDragging() const { return m_isDragging; }
    const DragDropPayload& getPayload() const { return m_payload; }
    std::shared_ptr<class ui::Widget> getDragSource() const { return m_dragSource; }

    /**
     * @brief Updates the drag state based on current mouse position.
     * This should be called by the InputManager during mouse move events while dragging.
     */
    void updateDragPosition(std::shared_ptr<class ui::Widget> root, float x, float y);

private:
    DragDropManager() = default;

    bool m_isDragging = false;
    std::shared_ptr<class ui::Widget> m_dragSource = nullptr;
    std::shared_ptr<class ui::Widget> m_lastTarget = nullptr;
    DragDropPayload m_payload;
};

} // namespace uxcpp::core
