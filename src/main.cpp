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
    dashboardEcu.subscribeTopic("RPM");
    dashboardEcu.subscribeTopic("TEMP");

    // ECU Simulation
    RpmECU rpmEcu(msgBus);
    TempECU tempEcu(msgBus);

    // Start all ECUs
    dashboardEcu.start();
    rpmEcu.start();
    tempEcu.start();

    // Let it run for 3 seconds
    std::this_thread::sleep_for(std::chrono::seconds(3));

    dashboardEcu.stop();
    rpmEcu.stop();
    tempEcu.stop();

    return 0;
}
