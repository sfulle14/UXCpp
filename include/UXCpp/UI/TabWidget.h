/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI\Widget.h>
#include <UXCpp/UI\Controls.h>
#include <vector>
#include <string>
#include <memory>

namespace uxcpp::ui {

class Tab {
public:
    Tab(std::string title, std::shared_ptr<Widget> content) 
        : title(std::move(title)), content(std::move(content)) {}

    std::string title;
    std::shared_ptr<Widget> content;
};

class TabWidget : public Widget {
public:
    TabWidget(std::string name) 
        : Widget(std::move(name), "tabwidget"), m_selectedIndex(-1) {}

    void addTab(const std::string& title, std::shared_ptr<Widget> content) {
        m_tabs.emplace_back(title, content);
        if (m_selectedIndex == -1) {
            setSelectedIndex(0);
        }
    }

    void setSelectedIndex(int index) {
        if (index >= 0 && index < static_cast<int>(m_tabs.size())) {
            m_selectedIndex = index;
        }
    }

    void onDraw(graphics::Renderer& renderer) override {
        // Draw Tab Bar
        float tabWidth = m_bounds.width / (m_tabs.empty() ? 1 : m_tabs.size());
        for (size_t i = 0; i < m_tabs.size(); ++i) {
            graphics::Rect tabRect = {m_bounds.x + (i * tabWidth), m_bounds.y, tabWidth, 30};
            graphics::Color color = (static_cast<int>(i) == m_selectedIndex) 
                                    ? graphics::Color::White() 
                                    : graphics::Color{0.8f, 0.8f, 0.8f, 1.0f};
            
            renderer->drawRect(tabRect, color);
            renderer->drawText({tabRect.x + 5, tabRect.y + 5}, m_tabs[i].title, graphics::Color::Black());
        }

        // Draw Active Content
        if (m_selectedIndex != -1) {
            auto content = m_tabs[m_selectedIndex].content;
            // Set bounds for the content area (below the tab bar)
            content->setBounds(m_bounds.x, m_bounds.y + 30, m_bounds.width, m_bounds.height - 30);
            content->onDraw(renderer);
        }
    }

    bool onPointerDown(graphics::Point p) override {
        if (p.y < m_bounds.y + 30) { // Clicked in tab bar
            float tabWidth = m_bounds.width / (m_tabs.empty() ? 1 : m_tabs.size());
            int index = static_cast<int>((p.x - m_bounds.x) / tabWidth);
            if (index >= 0 && index < static_cast<int>(m_tabs.size())) {
                setSelectedIndex(index);
                return true;
            }
        }
        return false;
    }

private:
    std::vector<Tab> m_tabs;
    int m_selectedIndex;
};

} // namespace uxcpp::ui
