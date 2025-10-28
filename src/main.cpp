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

    // ECU Simulation
    MessageFactory* msgFactory = new RpmMsgFactory();

    for (int i = 0; i < 5; ++i) {
        auto msg = msgFactory->factoryMethod();
        msgBus.publish(msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}