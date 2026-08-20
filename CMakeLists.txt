cmake_minimum_required(VERSION 3.22)
project(UXCpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags for strict hygiene
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

# ...existing code...
# Include directories
include_directories(include)

# Dependencies
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Project targets
add_library(UXCpp SHARED 
    src/Core/Application.cpp
    src/Core/SystemTray.cpp
    src/Core/MessageBox.cpp
    src/Core/NativeMenuService.cpp
    src/Graphics/SVGCache.cpp
    src/Services/Services.cpp
    src/Services/HttpClient.cpp
    src/Services/SqlDatabase.cpp
    src/Services/FileSystemWatcher.cpp
    src/Services/Process.cpp
    src/Services/Settings.cpp
    src/Services/SerialPort.cpp
)
target_link_libraries(UXCpp PUBLIC glfw OpenGL::GL)
target_include_directories(UXCpp PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Enable testing
enable_testing()
add_executable(test_platform_services tests/test_platform_services.cpp)
target_link_libraries(test_platform_services PUBLIC UXCpp glfw OpenGL::GL)
add_test(NAME PlatformServicesTest COMMAND test_platform_services)
add_subdirectory(tests)
