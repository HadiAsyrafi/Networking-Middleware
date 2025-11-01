#include <string>
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <thread>

#include "middleware/MessageBus.hpp"
#include "app/EcuImpl.hpp"

int main() {
    // Create Bus
    auto msgBus    = MessageBus();

    // Dashboard ECU
    EngineControlECU engineEcu(msgBus);
    DashboardECU     dashboardEcu(msgBus);
    RpmECU           rpmEcu(msgBus);
    TempECU          tempEcu(msgBus);

    // Start all ECUs
    engineEcu.start();
    dashboardEcu.start();
    rpmEcu.start();
    tempEcu.start();

    // Let it run for 3 seconds
    std::this_thread::sleep_for(std::chrono::seconds(3));

    engineEcu.stop();
    dashboardEcu.stop();
    rpmEcu.stop();
    tempEcu.stop();

    return 0;
}
