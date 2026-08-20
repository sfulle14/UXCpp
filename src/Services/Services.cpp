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

// In this clean-room implementation, the logic is currently contained in 
// header-only implementations for maximum portability and ease of integration.
// This file serves as a translation unit to ensure the services are linked
// and available in the shared library.

void initialize_services() {
    // Service initialization logic would go here.
}