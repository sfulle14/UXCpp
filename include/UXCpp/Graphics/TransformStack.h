/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/Renderer.h>
#include <cmath>

namespace uxcpp::graphics {

/**
 * @brief A transform stack for scale, rotate, and translate operations.
 */
class TransformStack {
public:
    struct State {
        float tx = 0, ty = 0;
        float sx = 1, sy = 1;
        float rotation = 0; // Radians
    };

    void push() { m_stack.push_back(m_current); }
    void pop() { if (m_stack.size() > 0) { m_current = m_stack.back(); m_stack.pop_back(); } }

    void translate(float dx, float dy) { m_current.tx += dx; m_current.ty += dy; }
    void scale(float sx, float sy) { m_current.sx *= sx; m_current.sy *= sy; }
    void rotate(float radians) { m_current.rotation += radians; }

    State getCurrent() const { return m_current; }

    /**
     * @brief Applies the current transform to a point.
     */
    Point apply(Point p) const {
        // Rotation
        float cosR = std::cos(m_current.rotation);
        float sinR = std::sin(m_current.rotation);
        float rx = p.x * cosR - p.y * sinR;
        float ry = p.x * sinR + p.y * cosR;

        // Scale and Translate
        return { rx * m_current.sx + m_current.tx, ry * m_current.sy + m_current.ty };
    }

private:
    State m_current;
    std::vector<State> m_stack;
};

} // namespace uxcpp::graphics
