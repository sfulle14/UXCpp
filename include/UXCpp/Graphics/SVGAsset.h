/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <UXCpp/Graphics/Renderer.h>

namespace uxcpp::graphics {

struct SVGElement {
    std::string type;
    std::string id;
    float x = 0, y = 0, width = 0, height = 0;
    Color fill = Color::White();
    Color stroke = Color::Black();
    float strokeWidth = 1.0f;
    std::vector<Point> points; // For paths/polygons
};

class SVGAsset {
public:
    explicit SVGAsset(const std::string& content) {
        parse(content);
    }

    void render(Renderer& renderer) const {
        for (const auto& elem : m_elements) {
            if (elem.type == "rect") {
                renderer.drawRect({elem.x, elem.y, elem.width, elem.height}, elem.fill);
            } else if (elem.type == "path") {
                renderer.beginPath();
                for (const auto& p : elem.points) {
                    renderer.lineTo(p.x, p.y);
                }
                renderer.closePath();
                renderer.fillPath(elem.fill);
            }
        }
    }

private:
    void parse(const std::string& content) {
        // Simplified SVG parser for demonstration purposes. 
        // In a real implementation, this would use an XML parser like pugixml.
        if (content.find("<rect") != std::string::npos) {
            SVGElement rect;
            rect.type = "rect";
            rect.x = 10; rect.y = 10; rect.width = 50; rect.height = 50;
            rect.fill = Color::Red();
            m_elements.push_back(rect);
        }
    }

    std::vector<SVGElement> m_elements;
};

class SVGPipeline {
public:
    static std::shared_ptr<SVGAsset> load(const std::string& filePath) {
        // In a real implementation, read file and parse.
        return std::make_shared<SVGAsset>("<svg><rect ... /></svg>");
    }

    static void renderSVG(Renderer& renderer, const std::string& filePath, Rect bounds) {
        auto asset = load(filePath);
        if (asset) {
            renderer.pushTransform();
            renderer.translate(bounds.x, bounds.y);
            renderer.scale(bounds.width / 100.0f, bounds.height / 100.0f); // Assume 100x100 viewbox
            asset->render(renderer);
            renderer.popTransform();
        }
    }
};

} // namespace uxcpp::graphics