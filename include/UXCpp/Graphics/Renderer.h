/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <cstdint>
#include <string>

namespace uxcpp::graphics {

struct Color {
    float r, g, b, a;

    static Color White() { return {1.0f, 1.0f, 1.0f, 1.0f}; }
    static Color Black() { return {0.0f, 0.0f, 0.0f, 1.0f}; }
    static Color Red()   { return {1.0f, 0.0f, 0.0f, 1.0f}; }
    static Color Green() { return {0.0f, 1.0f, 0.0f, 1.0f}; }
    static Color Blue()  { return {0.0f, 0.0f, 1.0f, 1.0f}; }
};

struct Point {
    float x, y;
};

struct Rect {
    float x, y, width, height;

    bool contains(Point p) const {
        return p.x >= x && p.x <= x + width && p.y >= y && p.y <= y + height;
    }
};

/**
 * @brief Abstract Rendering Context for 2D operations.
 */
class Renderer {
public:
    virtual ~Renderer() = default;

    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;

    virtual void drawRect(const Rect& rect, Color color) = 0;
    virtual void drawLine(Point a, Point b, Color color) = 0;
    virtual void drawCircle(Point center, float radius, Color color) = 0;
    virtual void drawFocusRing(const Rect& rect, Color color) = 0;
    virtual void drawText(Point pos, const std::string& text, Color color) = 0;
    virtual void setClipRect(const Rect& rect) = 0;
    virtual void clear(Color color) = 0;
};

} // namespace uxcpp::graphics
