/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI/Widget.h>
#include <vector>
#include <memory>
#include <string>

namespace uxcpp::ui {

/**
 * @brief A DockingContainer allows children to be pinned as panels or floated as windows.
 */
class DockingContainer : public Widget {
public:
    enum class DockPosition { Left, Right, Top, Bottom, Center };

    explicit DockingContainer(std::string name) 
        : Widget(std::move(name), "dock_container") {}

    void dockWidget(std::shared_ptr<Widget> widget, DockPosition pos) {
        m_dockedWidgets.push_back({widget, pos});
        performLayout();
    }

    void onDraw(graphics::Renderer& renderer) override {
        for (auto& entry : m_dockedWidgets) {
            entry.widget->onDraw(renderer);
        }
    }

private:
    struct DockEntry {
        std::shared_ptr<Widget> widget;
        DockPosition position;
    };

    std::vector<DockEntry> m_dockedWidgets;
};

} // namespace uxcpp::ui
