/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Core/Application.h>
#include <UXCpp/UI/Widget.h>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace uxcpp::ui {

struct MenuItem {
    std::string title;
    std::function<void()> action;
    bool enabled = true;

    MenuItem(std::string title, std::function<void()> action, bool enabled = true)
        : title(std::move(title)), action(std::move(action)), enabled(enabled) {}
};

class MenuPopup : public Widget {
public:
    explicit MenuPopup(std::string name) : Widget(std::move(name), "menu_popup") {}

    void addItem(MenuItem item) {
        m_items.push_back(std::move(item));
    }

    void setItemHeight(float height) { m_itemHeight = height; }

    void showAt(float x, float y, float width) {
        m_bounds = {x, y, width, m_itemHeight * static_cast<float>(m_items.size())};
        core::Application::getInstance().addOverlayWidget(shared_from_this());
    }

    void close() {
        core::Application::getInstance().removeOverlayWidget(shared_from_this());
    }

    void onDraw(graphics::Renderer& renderer) override {
        renderer.drawRect(m_bounds, graphics::Color::White());

        for (size_t i = 0; i < m_items.size(); ++i) {
            graphics::Rect itemRect{m_bounds.x, m_bounds.y + (i * m_itemHeight), m_bounds.width, m_itemHeight};
            const auto& item = m_items[i];
            const auto background = item.enabled ? graphics::Color{0.94f, 0.94f, 0.94f, 1.0f} : graphics::Color{0.85f, 0.85f, 0.85f, 1.0f};
            renderer.drawRect(itemRect, background);
            renderer.drawText({itemRect.x + 8.0f, itemRect.y + 4.0f}, item.title, graphics::Color::Black());
        }
    }

    bool onPointerDown(graphics::Point p) override {
        if (!m_bounds.contains(p)) {
            close();
            return false;
        }

        const int index = static_cast<int>((p.y - m_bounds.y) / m_itemHeight);
        if (index >= 0 && index < static_cast<int>(m_items.size()) && m_items[index].enabled) {
            if (m_items[index].action) {
                m_items[index].action();
            }
            close();
            return true;
        }
        return false;
    }

private:
    std::vector<MenuItem> m_items;
    float m_itemHeight = 26.0f;
};

class MenuBar : public Widget {
public:
    struct Menu {
        std::string title;
        std::shared_ptr<MenuPopup> popup;
    };

    explicit MenuBar(std::string name) : Widget(std::move(name), "menu_bar") {}

    std::shared_ptr<MenuPopup> addMenu(const std::string& title) {
        auto popup = std::make_shared<MenuPopup>(title + "Popup");
        m_menus.push_back({title, popup});
        return popup;
    }

    void setMenuWidth(float width) { m_menuWidth = width; }

    void onDraw(graphics::Renderer& renderer) override {
        renderer.drawRect(m_bounds, graphics::Color{0.18f, 0.18f, 0.18f, 1.0f});

        const float menuWidth = m_menuWidth;
        for (size_t i = 0; i < m_menus.size(); ++i) {
            graphics::Rect menuRect{m_bounds.x + (i * menuWidth), m_bounds.y, menuWidth, m_bounds.height};
            const bool selected = static_cast<int>(i) == m_openMenuIndex;
            renderer.drawRect(menuRect, selected ? graphics::Color{0.28f, 0.28f, 0.28f, 1.0f} : graphics::Color{0.22f, 0.22f, 0.22f, 1.0f});
            renderer.drawText({menuRect.x + 10.0f, menuRect.y + 6.0f}, m_menus[i].title, graphics::Color::White());
        }
    }

    bool onPointerDown(graphics::Point p) override {
        if (!m_bounds.contains(p)) {
            closePopup();
            return false;
        }

        const int index = static_cast<int>((p.x - m_bounds.x) / m_menuWidth);
        if (index >= 0 && index < static_cast<int>(m_menus.size())) {
            if (m_openMenuIndex == index) {
                closePopup();
            } else {
                openPopup(index);
            }
            return true;
        }
        return false;
    }

private:
    void openPopup(int index) {
        closePopup();
        m_openMenuIndex = index;
        auto popup = m_menus[m_openMenuIndex].popup;
        if (popup) {
            popup->showAt(m_bounds.x + (m_openMenuIndex * m_menuWidth), m_bounds.y + m_bounds.height, m_menuWidth);
        }
    }

    void closePopup() {
        if (m_openMenuIndex >= 0 && m_openMenuIndex < static_cast<int>(m_menus.size())) {
            auto popup = m_menus[m_openMenuIndex].popup;
            if (popup) {
                popup->close();
            }
        }
        m_openMenuIndex = -1;
    }

    std::vector<Menu> m_menus;
    int m_openMenuIndex = -1;
    float m_menuWidth = 140.0f;
};

} // namespace uxcpp::ui