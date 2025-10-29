#include <string>
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <thread>

#include <middleware/MessageBus.hpp>
#include <app/EcuSim.hpp>
#include <app/EngineSubscriber.hpp>

int main() {
    // Create Bus
    auto msgBus    = MessageBus();

    // Simulate Subscriber
    auto engineSub = makeEngineSubscriber();
    msgBus.subscribe("RPM", engineSub);
    msgBus.subscribe("TEMP", engineSub);

    // ECU Simulation
    RpmECU rpmEcu(msgBus);
    rpmEcu.start();

    TempECU tempEcu(msgBus);
    tempEcu.start();

    // Let it run for 3 seconds
    std::this_thread::sleep_for(std::chrono::seconds(3));

    rpmEcu.stop();
    tempEcu.stop();

    return 0;
}
