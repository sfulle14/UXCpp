/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI\Widget.h>
#include <UXCpp/UI\DropdownList.h>
#include <UXCpp/UI\PlotWidget.h>
#include <UXCpp/Core/Property.h>
#include <UXCpp/Graphics/TextLayout.h>
#include <UXCpp/Core/Application.h>
#include <algorithm>
#include <vector>

namespace uxcpp::ui {

class Label : public Widget {
public:
    Label(std::string name, std::string text) 
        : Widget(std::move(name), "label"), m_text(std::move(text)) {}

    void setText(std::string text) { m_text = std::move(text); }

    void onDraw(graphics::Renderer& renderer) override {
        // Use TextLayout for multi-line support
        graphics::TextLayout layout(m_text, m_bounds.width - 10.0f);
        const auto& lines = layout.getLines();
        const float lineHeight = layout.getLineHeight();

        for (size_t i = 0; i < lines.size(); ++i) {
            renderer.drawText({m_bounds.x + 5, m_bounds.y + 5 + (i * lineHeight)}, lines[i].text, getStyle().foregroundColor);
        }
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

        if (isFocused()) {
            renderer.drawFocusRing(m_bounds, graphics::Color::Blue());
        }
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
    ProgressBar(std::string name, core::Property<float>* valueSource = nullptr) 
        : Widget(std::move(name), "progressbar"), m_valueSource(valueSource) {
        if (m_valueSource) {
            m_valueSource->observe([this](const float& val) {
                this->setProgress(val);
            });
        }
    }

    void setProgress(float value) { 
        m_progress = std::clamp(value, 0.0f, 1.0f); 
    }

    void onDraw(graphics::Renderer& renderer) override {
        renderer.drawRect(m_bounds, graphics::Color::White());
        graphics::Rect fill = {m_bounds.x, m_bounds.y, m_bounds.width * m_progress, m_bounds.height};
        renderer.drawRect(fill, graphics::Color::Blue());
    }

private:
    float m_progress = 0.0f;
    core::Property<float>* m_valueSource;
};

class ComboBox : public Widget {
public:
    ComboBox(std::string name, const std::vector<std::string>& items) 
        : Widget(std::move(name), "combobox"), m_items(items), m_selectedIndex(-1), m_isOpen(false) {
        m_list = std::make_shared<DropdownList>("ComboList", items);
    }

    void onDraw(graphics::Renderer& renderer) override {
        // Draw the main button part
        renderer.drawRect(m_bounds, getStyle().backgroundColor);
        std::string text = m_selectedIndex == -1 ? "Select..." : m_items[m_selectedIndex];
        renderer.drawText({m_bounds.x + 5, m_bounds.y + 5}, text, getStyle().foregroundColor);
        renderer.drawText({m_bounds.x + m_bounds.width - 15, m_bounds.y + 5}, "v", getStyle().foregroundColor);
    }

    bool onPointerDown(graphics::Point p) override {
        if (m_bounds.contains(p)) {
            toggleList();
            return true;
        }
        return false;
    }

    void toggleList() {
        m_isOpen = !m_isOpen;
        if (m_isOpen) {
            // Position the list below the combobox
            m_list->setBounds(m_bounds.x, m_bounds.y + m_bounds.height, m_bounds.width, m_items.size() * 25.0f);
            core::Application::getInstance().addOverlayWidget(m_list);
        } else {
            core::Application::getInstance().removeOverlayWidget(m_list);
        }
    }

    void selectItem(int index) {
        m_selectedIndex = index;
        m_isOpen = false;
        core::Application::getInstance().removeOverlayWidget(m_list);
        onClicked.emit(); // Notify that selection changed
    }

    std::shared_ptr<DropdownList> getList() const { return m_list; }

private:
    std::vector<std::string> m_items;
    int m_selectedIndex;
    bool m_isOpen;
    std::shared_ptr<DropdownList> m_list;
};

} // namespace uxcpp::ui
