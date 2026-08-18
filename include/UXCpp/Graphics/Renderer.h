/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <cstdint>
#include <string>
#include <UXCpp/Graphics/TransformStack.h>

namespace uxcpp::graphics {

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

    // Render Polish API
    virtual void drawRoundedRect(const Rect& rect, float radius, Color color, bool fill = true) = 0;
    virtual void drawGradientRect(const Rect& rect, Color startColor, Color endColor, bool vertical = true) = 0;
    virtual void drawShadow(const Rect& rect, float blur, float offset, Color color) = 0;

    // Path Rendering API (for SVGs and complex shapes)
    virtual void beginPath() = 0;
    virtual void moveTo(float x, float y) = 0;
    virtual void lineTo(float x, float y) = 0;
    virtual void quadraticCurveTo(float cp1x, float cp1y, float x, float y) = 0;
    virtual void cubicCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) = 0;
    virtual void closePath() = 0;
    virtual void fillPath(Color color) = 0;
    virtual void strokePath(Color color, float thickness) = 0;

    // Transform Stack API
    virtual void pushTransform() = 0;
    virtual void popTransform() = 0;
    virtual void translate(float dx, float dy) = 0;
    virtual void scale(float sx, float sy) = 0;
    virtual void rotate(float radians) = 0;

    virtual void setClipRect(const Rect& rect) = 0;
    virtual void clear(Color color) = 0;
};

} // namespace uxcpp::graphics
