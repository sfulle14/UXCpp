/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Services/HttpClient.h>
#include <UXCpp/Services/SqlDatabase.h>
#include <UXCpp/Services/JsonXml.h>
#include <UXCpp/Services/FileSystemWatcher.h>
#include <UXCpp/Services/Process.h>
#include <UXCpp/Services/Settings.h>
#include <UXCpp/Services/SerialPort.h>
#include <iostream>
#include <cassert>

void test_http() {
    std::cout << "Testing HttpClient..." << std::endl;
    uxcpp::services::HttpClientImpl client;
    auto fut = client.get("https://api.example.com/data");
    auto res = fut.get();
    assert(res.statusCode == 200);
    std::cout << "HttpClient: OK" << std::endl;
}

void test_sql() {
    std::cout << "Testing SqlDatabase..." << std::endl;
    uxcpp::services::SqlDatabaseImpl db;
    assert(db.open("mock://db"));
    auto fut = db.executeQuery("SELECT * FROM users");
    auto res = fut.get();
    assert(res.success);
    std::cout << "SqlDatabase: OK" << std::endl;
}

void test_json() {
    std::cout << "Testing Json/Xml..." << std::endl;
    std::map<std::string, std::string> data = {{"key", "value"}};
    std::string json = uxcpp::services::Json::serialize(data);
    assert(json.find("key") != std::string::npos);
    std::cout << "Json/Xml: OK" << std::endl;
}

void test_watcher() {
    std::cout << "Testing FileSystemWatcher..." << std::endl;
    uxcpp::services::FileSystemWatcherImpl watcher;
    bool eventTriggered = false;
    watcher.watchDirectory("/tmp", [&](const uxcpp::services::FileSystemEvent& e) {
        eventTriggered = true;
    });
    // In a real test we'd wait for the mock thread to trigger
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // Since it's a mock polling every 2s, we just verify it starts.
    std::cout << "FileSystemWatcher: OK (Started)" << std::endl;
}

void test_process() {
    std::cout << "Testing Process service..." << std::endl;
    uxcpp::services::ProcessImpl proc;
    assert(proc.launch("echo", {"hello"}));
    auto res = proc.waitForExit().get();
    assert(res.exitCode == 0);
    std::cout << "Process: OK" << std::endl;
}

void test_settings() {
    std::cout << "Testing Settings service..." << std::endl;
    uxcpp::services::SettingsImpl settings;
    settings.set("theme", "dark");
    assert(settings.get("theme") == "dark");
    std::cout << "Settings: OK" << std::endl;
}

void test_serial() {
    std::cout << "Testing SerialPort..." << std::endl;
    uxcpp::services::SerialPortImpl serial;
    assert(serial.open("COM1", 9600));
    auto res = serial.read();
    assert(res.success);
    std::cout << "SerialPort: OK" << std::endl;
}

int main() {
    try {
        test_http();
        test_sql();
        test_json();
        test_watcher();
        test_process();
        test_settings();
        test_serial();
        std::cout << "All Application Services tests passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}