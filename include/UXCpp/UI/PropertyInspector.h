/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <map>
#include <memory>
#include <UXCpp/UI/Widget.h>

namespace uxcpp::ui {

class PropertyInspector {
public:
    struct Property {
        std::string name;
        std::string value;
        std::function<void(std::string)> setter;
    };

    /**
     * @brief Returns a list of editable properties for the given widget.
     */
    static std::vector<Property> getProperties(std::shared_ptr<Widget> widget) {
        std::vector<Property> props;
        
        props.push_back({"ID", widget->getId(), [widget](std::string v){ widget->setId(v); }});
        props.push_back({"Style Class", "...", nullptr}); // Simplified
        props.push_back({"Accessible Name", widget->getAccessibleName(), [widget](std::string v){ widget->setAccessibleName(v); }});
        
        return props;
    }
};

} // namespace uxcpp::ui