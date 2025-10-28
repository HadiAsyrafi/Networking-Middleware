#include <string>
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <thread>

#include <interface/Message.hpp>
#include <middleware/MessageBus.hpp>
#include <app/EngineSubscriber.hpp>

int main() {
    auto msgBus    = MessageBus();
    auto engineSub = makeEngineSubscriber();

    msgBus.subscribe("RPM", engineSub);

    for (int i = 0; i < 5; ++i) {
        int rpm = 1000 + rand() % 5000;
        auto msg = std::make_shared<RpmMessage>(rpm);
        msgBus.publish(msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}