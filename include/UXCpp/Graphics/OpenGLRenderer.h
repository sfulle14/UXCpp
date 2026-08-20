/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/Renderer.h>
#include <GL/gl.h>
#include <memory>
#include <iostream>
#include <UXCpp/Graphics/Text.h>
#include <cmath>
#include <vector>

namespace uxcpp::graphics {

enum class PathCmd { MoveTo, LineTo, QuadCurveTo, CubicCurveTo };
struct PathSegment {
    PathCmd cmd;
    std::vector<float> params;
};

class OpenGLRenderer : public Renderer {
public:
    explicit OpenGLRenderer(std::unique_ptr<TextRenderer> textRenderer = nullptr)
        : m_textRenderer(std::move(textRenderer)) {}

    void beginFrame() override {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_BLEND);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void endFrame() override {
        // Flush or other post-frame operations
    }

    void clear(Color color) override {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void drawRect(const Rect& rect, Color color) override {
        glColor4f(color.r, color.g, color.b, color.a);
        glBegin(GL_QUADS);
            glVertex2f(rect.x, rect.y);
            glVertex2f(rect.x + rect.width, rect.y);
            glVertex2f(rect.x + rect.width, rect.y + rect.height);
            glVertex2f(rect.x, rect.y + rect.height);
        glEnd();
    }

    void drawLine(Point a, Point b, Color color) override {
        glColor4f(color.r, color.g, color.b, color.a);
        glBegin(GL_LINES);
            glVertex2f(a.x, a.y);
            glVertex2f(b.x, b.y);
        glEnd();
    }

    void drawCircle(Point center, float radius, Color color) override {
        glColor4f(color.r, color.g, color.b, color.a);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(center.x, center.y);
            for (int i = 0; i <= 360; i += 10) {
                float rad = i * 3.14159f / 180.0f;
                glVertex2f(center.x + cos(rad) * radius, center.y + sin(rad) * radius);
            }
        glEnd();
    }

    /**
     * @brief Draws a focus ring around the specified rectangle.
     */
    void drawFocusRing(const Rect& rect, Color color) override {
        glColor4f(color.r, color.g, color.b, color.a);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(rect.x - 2, rect.y - 2);
            glVertex2f(rect.x + rect.width + 2, rect.y - 2);
            glVertex2f(rect.x + rect.width + 2, rect.y + rect.height + 2);
            glVertex2f(rect.x - 2, rect.y + rect.height + 2);
        glEnd();
    }

    void drawText(Point pos, const std::string& text, Color color) override {
        if (m_textRenderer) {
            m_textRenderer->renderText(text, pos.x, pos.y, 16.0f, color);
        }
    }

    void drawRoundedRect(const Rect& rect, float radius, Color color, bool fill = true) override {
        if (radius <= 0) {
            drawRect(rect, color);
            return;
        }
        
        beginPath();
        // Bottom-left corner
        moveTo(rect.x + radius, rect.y);
        lineTo(rect.x + rect.width - radius, rect.y);
        // Bottom-right corner
        quadraticCurveTo(rect.x + rect.width, rect.y, rect.x + rect.width, rect.y + radius);
        lineTo(rect.x + rect.width, rect.y + rect.height - radius);
        // Top-right corner
        quadraticCurveTo(rect.x + rect.width, rect.y + rect.height, rect.x + rect.width - radius, rect.y + rect.height);
        lineTo(rect.x + radius, rect.y + rect.height);
        // Top-left corner
        quadraticCurveTo(rect.x, rect.y + rect.height, rect.x, rect.y + rect.height - radius);
        lineTo(rect.x, rect.y + radius);
        // Final corner
        quadraticCurveTo(rect.x, rect.y, rect.x + radius, rect.y);
        closePath();

        if (fill) fillPath(color); else strokePath(color, 1.0f);
    }

    void drawGradientRect(const Rect& rect, Color startColor, Color endColor, bool vertical = true) override {
        // OpenGL immediate mode doesn't support gradients easily without shaders.
        // We simulate it with a quad and vertex colors.
        glBegin(GL_QUADS);
            glColor4f(startColor.r, startColor.g, startColor.b, startColor.a);
            glVertex2f(rect.x, rect.y);
            glVertex2f(rect.x + rect.width, rect.y);

            glColor4f(endColor.r, endColor.g, endColor.b, endColor.a);
            glVertex2f(rect.x + rect.width, rect.y + rect.height);
            glVertex2f(rect.x, rect.y + rect.height);
        glEnd();
    }

    void drawShadow(const Rect& rect, float blur, float offset, Color color) override {
        // Simple drop shadow: render a slightly offset black rect with lower alpha
        glColor4f(color.r, color.g, color.b, color.a * 0.5f);
        drawRect({rect.x + offset, rect.y + offset, rect.width, rect.height}, color);
    }

    // Path Rendering Implementation
    void beginPath() override {
        m_currentPath.clear();
        m_isPathClosed = false;
    }

    void moveTo(float x, float y) override {
        m_currentPath.push_back({PathCmd::MoveTo, {x, y}});
    }

    void lineTo(float x, float y) override {
        m_currentPath.push_back({PathCmd::LineTo, {x, y}});
    }

    void quadraticCurveTo(float cp1x, float cp1y, float x, float y) override {
        m_currentPath.push_back({PathCmd::QuadCurveTo, {cp1x, cp1y, x, y}});
    }

    void cubicCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) override {
        m_currentPath.push_back({PathCmd::CubicCurveTo, {cp1x, cp1y, cp2x, cp2y, x, y}});
    }

    void closePath() override {
        m_isPathClosed = true;
    }

    void fillPath(Color color) override {
        glColor4f(color.r, color.g, color.b, color.a);
        glBegin(GL_POLYGON);
        renderPathInternal();
        glEnd();
    }

    void strokePath(Color color, float thickness) override {
        glColor4f(color.r, color.g, color.b, color.a);
        glLineWidth(thickness);
        glBegin(GL_LINE_STRIP);
        renderPathInternal();
        glEnd();
    }

    void setClipRect(const Rect& rect) override {
        glEnable(GL_SCISSOR_TEST);
        glScissor((GLint)rect.x, (GLint)rect.y, (GLint)rect.width, (GLint)rect.height);
    }

    void pushClip(const Rect& rect) override {
        setClipRect(rect); 
    }

    void popClip() override {
        glDisable(GL_SCISSOR_TEST);
    }

private:
    std::unique_ptr<TextRenderer> m_textRenderer;
    std::vector<PathSegment> m_currentPath;
    bool m_isPathClosed = false;

    void renderPathInternal() {
        float lastX = 0, lastY = 0;
        for (const auto& seg : m_currentPath) {
            switch (seg.cmd) {
                case PathCmd::MoveTo:
                    glVertex2f(seg.params[0], seg.params[1]);
                    lastX = seg.params[0]; lastY = seg.params[1];
                    break;
                case PathCmd::LineTo:
                    glVertex2f(seg.params[0], seg.params[1]);
                    lastX = seg.params[0]; lastY = seg.params[1];
                    break;
                case PathCmd::QuadCurveTo: {
                    float cp1x = seg.params[0], cp1y = seg.params[1];
                    float x = seg.params[2], y = seg.params[3];
                    for (int i = 0; i <= 20; ++i) {
                        float t = i / 20.0f;
                        float px = (1-t)*(1-t)*lastX + 2*(1-t)*t*cp1x + t*t*x;
                        float py = (1-t)*(1-t)*lastY + 2*(1-t)*t*cp1y + t*t*y;
                        glVertex2f(px, py);
                    }
                    lastX = x; lastY = y;
                    break;
                }
                case PathCmd::CubicCurveTo: {
                    float cp1x = seg.params[0], cp1y = seg.params[1];
                    float cp2x = seg.params[2], cp2y = seg.params[3];
                    float x = seg.params[4], y = seg.params[5];
                    for (int i = 0; i <= 20; ++i) {
                        float t = i / 20.0f;
                        float px = (1-t)*(1-t)*(1-t)*lastX + 3*(1-t)*(1-t)*t*cp1x + 3*(1-t)*t*t*cp2x + t*t*t*x;
                        float py = (1-t)*(1-t)*(1-t)*lastY + 3*(1-t)*(1-t)*t*cp1y + 3*(1-t)*t*t*cp2y + t*t*t*y;
                        glVertex2f(px, py);
                    }
                    lastX = x; lastY = y;
                    break;
                }
            }
        }
        if (m_isPathClosed) {
            // Close path by returning to first point if it exists
            if (!m_currentPath.empty() && m_currentPath[0].cmd == PathCmd::MoveTo) {
                glVertex2f(m_currentPath[0].params[0], m_currentPath[0].params[1]);
            }
        }
    }
};

} // namespace uxcpp::graphics
