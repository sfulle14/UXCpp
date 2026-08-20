/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Plugins/IPlugin.h>
#include <UXCpp/Plugins/PluginManager.h>
#include <iostream>
#include <cassert>

class MockPlugin : public uxcpp::plugins::IPlugin {
public:
    std::string getName() const override { return "MockPlugin"; }
    std::string getVersion() const override { return "1.0.0"; }
    void onInitialize() override { std::cout << "MockPlugin Initialized" << std::endl; }
    void onShutdown() override { std::cout << "MockPlugin Shutdown" << std::endl; }
};

int main() {
    try {
        auto& manager = uxcpp::plugins::PluginManager::getInstance();
        auto plugin = std::make_unique<MockPlugin>();
        manager.registerPlugin("mock", std::move(plugin));
        
        assert(manager.getLoadedPlugins().count("mock") == 1);
        std::cout << "Plugin System: OK" << std::endl;
        
        manager.unloadPlugin("mock");
        assert(manager.getLoadedPlugins().count("mock") == 0);
        std::cout << "Plugin Unloading: OK" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}