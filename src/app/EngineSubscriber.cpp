#include <iostream>
#include "Subscriber.hpp"
#include "Message.hpp"

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

// Expose one instance globally (not ideal long-term, but okay for demo)
std::shared_ptr<EngineSubscriber> makeEngineSubscriber() {
    return std::make_shared<EngineSubscriber>();
}
