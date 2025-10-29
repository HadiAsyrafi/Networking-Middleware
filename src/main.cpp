#include <string>
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <thread>

#include <interface/MessageFactory.hpp>
#include <middleware/MessageBus.hpp>
#include <app/EngineSubscriber.hpp>

int main() {
    // Create Bus
    auto msgBus    = MessageBus();

    // Create Subscriber
    auto engineSub = makeEngineSubscriber();
    msgBus.subscribe("RPM", engineSub);
    msgBus.subscribe("STATUS", engineSub);

    // ECU Simulation
    auto rpmFactory = std::make_unique<RpmMsgFactory>();
    auto statusFactory = std::make_unique<StatMsgFactory>();

    for (int i = 0; i < 5; ++i) {
        // Create RPM message
        auto rpmMsg = rpmFactory->factoryMethod();
        msgBus.publish(rpmMsg);

        // Create Status message
        auto statusMsg = statusFactory->factoryMethod();
        msgBus.publish(statusMsg);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}