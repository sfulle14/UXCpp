cmake_minimum_required(VERSION 3.22)

# We use a simple main-based test for now, but CTest is enabled in root
add_executable(test_signals tests/test_signals.cpp)
target_link_libraries(test_signals PRIVATE UXCpp)

add_test(NAME SignalTest COMMAND test_signals)
