#include "app/EngineSubscriber.hpp"
#include "interface/MessageFactory.hpp"
#include "middleware/MessageBus.hpp"

#include <iostream>
#include <thread>
#include <chrono>

int main() {
    MessageFactory::instance().registerType("RPM", [] {
        return std::make_shared<RpmMessage>(0);
    });

    auto engineSub = std::make_shared<EngineSubscriber>();
    MessageBus::instance().subscribe("RPM", engineSub);

    for (int i = 0; i < 5; ++i) {
        int rpm = 1000 + rand() % 5000;
        auto msg = std::make_shared<RpmMessage>(rpm);
        MessageBus::instance().publish(msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
