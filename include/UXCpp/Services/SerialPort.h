/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <string>
#include <vector>
#include <memory>

namespace uxcpp::services {

struct SerialResponse {
    bool success;
    std::string data;
};

class SerialPort {
public:
    virtual ~SerialPort() = default;

    /**
     * @brief Opens a serial port.
     */
    virtual bool open(const std::string& portName, int baudRate) = 0;

    /**
     * @brief Sends data over the serial port.
     */
    virtual bool write(const std::string& data) = 0;

    /**
     * @brief Reads data from the serial port.
     */
    virtual SerialResponse read() = 0;

    /**
     * @brief Closes the serial port.
     */
    virtual void close() = 0;
};

class SerialPortImpl : public SerialPort {
public:
    bool open(const std::string& portName, int baudRate) override {
        m_isOpen = true;
        return true;
    }

    bool write(const std::string& data) override {
        return m_isOpen;
    }

    SerialResponse read() override {
        if (!m_isOpen) return {false, ""};
        return {true, "Mock serial data"};
    }

    void close() override {
        m_isOpen = false;
    }

private:
    bool m_isOpen = false;
};

} // namespace uxcpp::services