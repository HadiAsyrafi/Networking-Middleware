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

    // Dashboard ECU
    DashboardECU dashboardEcu(msgBus);
    msgBus.subscribe("RPM", dashboardEcu.getSubscriber());
    msgBus.subscribe("TEMP", dashboardEcu.getSubscriber());

    // ECU Simulation
    RpmECU rpmEcu(msgBus);
    TempECU tempEcu(msgBus);
    rpmEcu.start();
    tempEcu.start();

    // Let it run for 3 seconds
    std::this_thread::sleep_for(std::chrono::seconds(3));

    dashboardEcu.stop();
    rpmEcu.stop();
    tempEcu.stop();

    return 0;
}
