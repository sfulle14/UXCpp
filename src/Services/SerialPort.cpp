/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Services/SerialPort.h>

namespace uxcpp::services {

bool SerialPortImpl::open(const std::string& portName, int baudRate) {
    m_isOpen = true;
    return true;
}

bool SerialPortImpl::write(const std::string& data) {
    return m_isOpen;
}

SerialResponse SerialPortImpl::read() {
    if (!m_isOpen) return {false, ""};
    return {true, "Mock serial data"};
}

void SerialPortImpl::close() {
    m_isOpen = false;
}

} // namespace uxcpp::services