cmake_minimum_required(VERSION 3.22)

# We use a simple main-based test for now, but CTest is enabled in root
add_executable(test_signals tests/test_signals.cpp)
target_link_libraries(test_signals PRIVATE UXCpp)
add_test(NAME SignalTest COMMAND test_signals)

add_executable(test_graphics_advanced tests/test_graphics_advanced.cpp)
target_link_libraries(test_graphics_advanced PRIVATE UXCpp)
add_test(NAME GraphicsAdvancedTest COMMAND test_graphics_advanced)

add_executable(test_services tests/test_services.cpp)
target_link_libraries(test_services PRIVATE UXCpp)
add_test(NAME ServicesTest COMMAND test_services)

add_executable(test_plugins tests/test_plugins.cpp)
target_link_libraries(test_plugins PRIVATE UXCpp)
add_test(NAME PluginTest COMMAND test_plugins)
