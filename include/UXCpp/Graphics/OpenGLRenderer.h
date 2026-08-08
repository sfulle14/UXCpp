/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Graphics/Renderer.h>
#include <GL/gl.h>
#include <iostream>
#include <UXCpp/Graphics/Text.h>

namespace uxcpp::graphics {

class OpenGLRenderer : public Renderer {
public:
    explicit OpenGLRenderer(std::unique_ptr<TextRenderer> textRenderer) 
        : m_textRenderer(std::move(textRenderer)) {}

    void beginFrame() override {
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

    void drawText(Point pos, const std::string& text, Color color) override {
        if (m_textRenderer) {
            m_textRenderer->renderText(text, pos.x, pos.y, 16.0f, color);
        }
    }

    void setClipRect(const Rect& rect) override {
        glEnable(GL_SCISSOR_TEST);
        glScissor((GLint)rect.x, (GLint)rect.y, (GLint)rect.width, (GLint)rect.height);
    }

private:
    std::unique_ptr<TextRenderer> m_textRenderer;
};

} // namespace uxcpp::graphics
