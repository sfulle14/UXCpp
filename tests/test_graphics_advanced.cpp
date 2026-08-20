/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Graphics/Renderer.h>
#include <UXCpp/Graphics/OpenGLRenderer.h>
#include <UXCpp/Graphics/MockRenderer.h>
#include <UXCpp/Graphics/SVGAsset.h>
#include <UXCpp/Graphics/Charts.h>
#include <iostream>
#include <cassert>

void test_svg_pipeline() {
    std::cout << "Testing SVG Pipeline..." << std::endl;
    uxcpp::graphics::MockRenderer renderer;
    auto asset = uxcpp::graphics::SVGPipeline::load("test.svg");
    assert(asset != nullptr);
    
    asset->render(renderer);
    // Verify that the mock renderer received draw calls
    assert(!renderer.getCalls().empty());
    std::cout << "SVG Pipeline: OK" << std::endl;
}

void test_charting() {
    std::cout << "Testing Charting Subsystem..." << std::endl;
    uxcpp::graphics::MockRenderer renderer;
    
    uxcpp::graphics::LineChart lineChart;
    uxcpp::graphics::ChartSeries series;
    series.name = "Test Series";
    series.color = uxcpp::graphics::Color::Blue();
    series.data = {{0, 0}, {1, 10}, {2, 5}, {3, 15}};
    lineChart.addSeries(series);

    lineChart.render(renderer, {0, 0, 100, 100});
    assert(!renderer.getCalls().empty());

    uxcpp::graphics::BarChart barChart;
    barChart.addSeries(series);
    barChart.render(renderer, {0, 0, 100, 100});
    assert(!renderer.getCalls().empty());

    uxcpp::graphics::PieChart pieChart;
    pieChart.addSeries(series);
    pieChart.render(renderer, {0, 0, 100, 100});
    assert(!renderer.getCalls().empty());

    std::cout << "Charting Subsystem: OK" << std::endl;
}

int main() {
    try {
        test_svg_pipeline();
        test_charting();
        std::cout << "All Advanced Graphics tests passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}