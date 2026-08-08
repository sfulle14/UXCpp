#include <UXCpp/Core/Signal.h>
#include <cassert>
#include <string>

void test_basic_emission() {
    uxcpp::core::Signal<int, std::string> signal;
    int callCount = 0;
    int receivedInt = 0;
    std::string receivedStr = "";

    signal.connect([&](int i, std::string s) {
        callCount++;
        receivedInt = i;
        receivedStr = s;
    });

    signal.emit(42, "Hello UXCpp");

    assert(callCount == 1);
    assert(receivedInt == 42);
    assert(receivedStr == "Hello UXCpp");
    std::cout << "test_basic_emission passed!" << std::endl;
}

void test_multiple_slots() {
    uxcpp::core::Signal<void()> signal;
    int count1 = 0, count2 = 0;

    signal.connect([&]() { count1++; });
    signal.connect([&]() { count2++; });

    signal.emit();

    assert(count1 == 1);
    assert(count2 == 1);
    std::cout << "test_multiple_slots passed!" << std::endl;
}

void test_disconnection() {
    uxcpp::core::Signal<int> signal;
    int count = 0;

    auto id = signal.connect([&](int) { count++; });
    signal.emit(1);
    assert(count == 1);

    signal.disconnect(id);
    signal.emit(2);
    assert(count == 1); // Should not have increased
    std::cout << "test_disconnection passed!" << std::endl;
}

void test_disconnect_all() {
    uxcpp::core::Signal<int> signal;
    int count = 0;

    signal.connect([&](int) { count++; });
    signal.connect([&](int) { count++; });
    
    signal.emit(1);
    assert(count == 2);

    signal.disconnectAll();
    signal.emit(2);
    assert(count == 2); // Should not have increased
    std::cout << "test_disconnect_all passed!" << std::endl;
}

int main() {
    try {
        test_basic_emission();
        test_multiple_slots();
        test_disconnection();
        test_disconnect_all();
        std::cout << "All signal tests passed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
