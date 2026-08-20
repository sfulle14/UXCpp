/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/Renderer.h>
#include <iostream>
#include <vector>
#include <string>

namespace uxcpp::graphics {

/**
 * @brief A non-OpenGL implementation of the Renderer for unit testing and headless environments.
 */
class MockRenderer : public Renderer {
public:
    struct DrawCall {
        std::string op;
        float x, y, w, h;
        Color color;
    };

    void beginFrame() override { m_calls.clear(); }
    void endFrame() override {}

    void drawRect(const Rect& rect, Color color) override {
        m_calls.push_back({"drawRect", rect.x, rect.y, rect.width, rect.height, color});
    }

    void drawLine(Point a, Point b, Color color) override {
        m_calls.push_back({"drawLine", a.x, a.y, b.x, b.y, color});
    }

    void drawCircle(Point center, float radius, Color color) override {
        m_calls.push_back({"drawCircle", center.x, center.y, radius, 0, color});
    }

    void drawFocusRing(const Rect& rect, Color color) override {
        m_calls.push_back({"drawFocusRing", rect.x, rect.y, rect.width, rect.height, color});
    }

    void drawText(Point pos, const std::string& text, Color color) override {
        m_calls.push_back({"drawText", pos.x, pos.y, 0, 0, color});
    }

    void pushTransform() override {}
    void popTransform() override {}
    void translate(float dx, float dy) override {}
    void scale(float sx, float sy) override {}
    void rotate(float radians) override {}

    void setClipRect(const Rect& rect) override {
        m_calls.push_back({"setClip", rect.x, rect.y, rect.width, rect.height, {}});
    }

    void clear(Color color) override {
        m_calls.push_back({"clear", 0, 0, 0, 0, color});
    }

    // Path API implementation for Mock
    void beginPath() override { m_currentOp = "beginPath"; }
    void moveTo(float x, float y) override { m_calls.push_back({"moveTo", x, y, 0, 0, {}}); }
    void lineTo(float x, float y) override { m_calls.push_back({"lineTo", x, y, 0, 0, {}}); }
    void quadraticCurveTo(float cp1x, float cp1y, float x, float y) override {
        m_calls.push_back({"quadCurve", cp1x, cp1y, x, y, {}});
    }
    void cubicCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) override {
        m_calls.push_back({"cubicCurve", cp1x, cp1y, cp2x, cp2y, {}});
    }
    void closePath() override { m_calls.push_back({"closePath", 0, 0, 0, 0, {}}); }
    void fillPath(Color color) override { m_calls.push_back({"fillPath", 0, 0, 0, 0, color}); }
    void strokePath(Color color, float thickness) override {
        m_calls.push_back({"strokePath", 0, 0, thickness, 0, color});
    }

    // Render Polish implementation for Mock
    void drawRoundedRect(const Rect& rect, float radius, Color color, bool fill) override {
        m_calls.push_back({"drawRoundedRect", rect.x, rect.y, radius, 0, color});
    }
    void drawGradientRect(const Rect& rect, Color startColor, Color endColor, bool vertical) override {
        m_calls.push_back({"drawGradientRect", rect.x, rect.y, 0, 0, startColor});
    }
    void drawShadow(const Rect& rect, float blur, float offset, Color color) override {
        m_calls.push_back({"drawShadow", rect.x, rect.y, blur, offset, color});
    }

    const std::vector<DrawCall>& getCalls() const { return m_calls; }

private:
    std::vector<DrawCall> m_calls;
    std::string m_currentOp;
};

} // namespace uxcpp::graphics