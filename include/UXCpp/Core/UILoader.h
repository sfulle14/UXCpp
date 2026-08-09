/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/UI\Widget.h>
#include <UXCpp/UI\Controls.h>
#include <UXCpp/UI\Layout.h>
#include <string>
#include <memory>
#include <map>
#include <sstream>

namespace uxcpp::core {

/**
 * @brief A simple declarative UI loader that parses a custom format to build widget trees.
 * 
 * Format example:
 * [Box, "Root", Vertical] {
 *   [Label, "WelcomeLbl", "Hello World"]
 *   [Button, "SubmitBtn", "Click Me"]
 * }
 */
class UILoader {
public:
    static std::shared_ptr<ui::Widget> loadFromString(const std::string& source) {
        std::stringstream ss(source);
        return parseWidget(ss);
    }

private:
    static std::shared_ptr<ui::Widget> parseWidget(std::stringstream& ss) {
        std::string token;
        if (!(ss >> token)) return nullptr;

        // Expecting '['
        if (token != "[") return nullptr;

        std::string type;
        ss >> type; // Widget Type
        
        // Consume comma
        std::string comma;
        ss >> comma;

        // Parse arguments based on type
        std::shared_ptr<ui::Widget> widget;
        if (type == "Label") {
            std::string name, text;
            ss >> std::quoted(name) >> comma >> std::quoted(text);
            widget = std::make_shared<ui::Label>(name, text);
        } else if (type == "Button") {
            std::string name, label;
            ss >> std::quoted(name) >> comma >> std::quoted(label);
            widget = std::make_shared<ui::Button>(name, label);
        } else if (type == "Box") {
            std::string name;
            std::string orientStr;
            ss >> std::quoted(name) >> comma >> std::quoted(orientStr);
            ui::LayoutOrientation orient = (orientStr == "Vertical") ? ui::LayoutOrientation::Vertical : ui::LayoutOrientation::Horizontal;
            widget = std::make_shared<ui::Box>(name, orient);
        }

        // Consume closing ']'
        ss >> token; // should be ']'

        // Check for children block '{ ... }'
        ss >> token;
        if (token == "{") {
            while (ss.peek() != '}' && ss.peek() != EOF) {
                auto child = parseWidget(ss);
                if (child) widget->addChild(child);
                if (ss.peek() == '}') break;
            }
            ss >> token; // consume '}'
        }

        return widget;
    }
};

} // namespace uxcpp::core
