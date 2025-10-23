#include <iostream>
#include <thread>
#include <chrono>
#include "middleware/MessageBus.hpp"
#include "MessageFactory.hpp"

class RPMMessage : public Message {
public:
    explicit RPMMessage(int value) : Message("RPM"), value_(value) {}
    int value() const { return value_; }
private:
    int value_;
};

class EngineSubscriber : public Subscriber {
public:
    void onMessage(const MessagePtr& msg) override {
        auto rpmMsg = std::dynamic_pointer_cast<RPMMessage>(msg);
        if (rpmMsg)
            std::cout << "[Subscriber] Engine RPM: " << rpmMsg->value() << "\n";
    }
};

int main() {
    MessageFactory::instance().registerType("RPM", [] {
        return std::make_shared<RPMMessage>(0);
    });

    auto engineSub = std::make_shared<EngineSubscriber>();
    MessageBus::instance().subscribe("RPM", engineSub);

    for (int i = 0; i < 5; ++i) {
        int rpm = 1000 + rand() % 5000;
        auto msg = std::make_shared<RPMMessage>(rpm);
        MessageBus::instance().publish(msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
