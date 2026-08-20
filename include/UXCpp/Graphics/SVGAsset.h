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
            } else if (elem.type == "circle") {
                renderer.drawCircle({elem.x, elem.y}, elem.width / 2, elem.fill);
            } else if (elem.type == "line") {
                renderer.drawLine(elem.points[0], elem.points[1], elem.stroke);
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
        // Improved simplified SVG parser. 
        // In a production environment, this would use pugixml or similar.
        size_t pos = 0;
        while ((pos = content.find('<', pos)) != std::string::npos) {
            size_t endPos = content.find('>', pos);
            if (endPos == std::string::npos) break;

            std::string tag = content.substr(pos + 1, endPos - pos - 1);
            if (tag.find("rect") == 0) {
                SVGElement rect;
                rect.type = "rect";
                rect.x = parseAttr(tag, "x");
                rect.y = parseAttr(tag, "y");
                rect.width = parseAttr(tag, "width");
                rect.height = parseAttr(tag, "height");
                rect.fill = parseColor(parseAttr(tag, "fill", "red"));
                m_elements.push_back(rect);
            } else if (tag.find("circle") == 0) {
                SVGElement circle;
                circle.type = "circle";
                circle.x = parseAttr(tag, "cx");
                circle.y = parseAttr(tag, "cy");
                circle.width = parseAttr(tag, "r") * 2; // Diameter for bounds
                circle.height = parseAttr(tag, "r") * 2;
                circle.fill = parseColor(parseAttr(tag, "fill", "blue"));
                m_elements.push_back(circle);
            } else if (tag.find("line") == 0) {
                SVGElement line;
                line.type = "line";
                line.points = {{parseAttr(tag, "x1"), parseAttr(tag, "y1")}, 
                               {parseAttr(tag, "x2"), parseAttr(tag, "y2")}}};
                line.stroke = parseColor(parseAttr(tag, "stroke", "black"));
                m_elements.push_back(line);
            }
            pos = endPos + 1;
        }
    }

    float parseAttr(const std::string& tag, const std::string& attr, float def = 0) {
        size_t start = tag.find(attr + "=\"");
        if (start == std::string::npos) return def;
        start += attr.length() + 2;
        size_t end = tag.find('"', start);
        return std::stof(tag.substr(start, end - start));
    }

    std::string parseAttrStr(const std::string& tag, const std::string& attr, const std::string& def) {
        size_t start = tag.find(attr + "=\"");
        if (start == std::string::npos) return def;
        start += attr.length() + 2;
        size_t end = tag.find('"', start);
        return tag.substr(start, end - start);
    }

    Color parseColor(const std::string& colorStr) {
        if (colorStr == "red") return Color::Red();
        if (colorStr == "blue") return Color::Blue();
        if (colorStr == "green") return Color::Green();
        if (colorStr == "black") return Color::Black();
        if (colorStr == "white") return Color::White();
        return Color::White();
    }

    std::vector<SVGElement> m_elements;
};

class SVGPipeline {
public:
    static std::shared_ptr<SVGAsset> load(const std::string& filePath) {
        auto it = m_cache.find(filePath);
        if (it != m_cache.end()) {
            return it->second;
        }

        // In a real implementation, read file and parse.
        auto asset = std::make_shared<SVGAsset>("<svg><rect x=\"10\" y=\"10\" width=\"50\" height=\"50\" fill=\"red\" /></svg>");
        m_cache[filePath] = asset;
        return asset;
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

private:
    static std::map<std::string, std::shared_ptr<SVGAsset>> m_cache;
};
};

} // namespace uxcpp::graphics