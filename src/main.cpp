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

    // Use RAII container for automatic cleanup
    std::vector<std::unique_ptr<ECU>> ecus;
    ecus.emplace_back(std::make_unique<EngineControlECU>(msgBus));
    ecus.emplace_back(std::make_unique<DashboardECU>(msgBus));
    ecus.emplace_back(std::make_unique<RpmECU>(msgBus));
    ecus.emplace_back(std::make_unique<TempECU>(msgBus));

    // Start all ECUs
    for (auto& ecu : ecus) {
        ecu->start();
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Stop all ECUs - RAII will handle cleanup
    for (auto& ecu : ecus) {
        ecu->stop();
    }

    return 0;
}
