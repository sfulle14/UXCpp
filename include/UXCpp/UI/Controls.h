/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI\Widget.h>

namespace uxcpp::ui {

class Label : public Widget {
public:
    Label(std::string name, std::string text) 
        : Widget(std::move(name), "label"), m_text(std::move(text)) {}

    void setText(std::string text) { m_text = std::move(text); }

    void onDraw(graphics::Renderer& renderer) override {
        renderer.drawText({m_bounds.x, m_bounds.y}, m_text, getStyle().foregroundColor);
    }

private:
    std::string m_text;
};

class Button : public Widget {
public:
    Button(std::string name, std::string label) 
        : Widget(std::move(name), "button"), m_label(std::move(label)) {}

    void onDraw(graphics::Renderer& renderer) override {
        const auto& style = getStyle();
        renderer.drawRect(m_bounds, style.backgroundColor); 
        renderer.drawText({m_bounds.x + 5, m_bounds.y + 5}, m_label, style.foregroundColor);
    }

    bool onPointerDown(graphics::Point p) override {
        if (m_bounds.contains(p)) {
            onClicked.emit();
            return true;
        }
        return false;
    }

private:
    std::string m_label;
};

class CheckBox : public Widget {
public:
    CheckBox(std::string name, std::string label) 
        : Widget(std::move(name), "checkbox"), m_label(std::move(label)), m_checked(false) {}

    void setChecked(bool checked) { m_checked = checked; }
    bool isChecked() const { return m_checked; }

    void onDraw(graphics::Renderer& renderer) override {
        renderer.drawRect({m_bounds.x, m_bounds.y, 20, 20}, m_checked ? graphics::Color::Green() : graphics::Color::White());
        renderer.drawText({m_bounds.x + 25, m_bounds.y + 2}, m_label, getStyle().foregroundColor);
    }

    bool onPointerDown(graphics::Point p) override {
        if (m_bounds.contains(p)) {
            m_checked = !m_checked;
            onClicked.emit();
            return true;
        }
        return false;
    }

private:
    std::string m_label;
    bool m_checked;
};

class ProgressBar : public Widget {
public:
    ProgressBar(std::string name) 
        : Widget(std::move(name), "progressbar"), m_progress(0.0f) {}

    void setProgress(float value) { m_progress = std::clamp(value, 0.0f, 1.0f); }

    void onDraw(graphics::Renderer& renderer) override {
        renderer.drawRect(m_bounds, graphics::Color::White());
        graphics::Rect fill = {m_bounds.x, m_bounds.y, m_bounds.width * m_progress, m_bounds.height};
        renderer.drawRect(fill, graphics::Color::Blue());
    }

private:
    float m_progress;
};

} // namespace uxcpp::ui
