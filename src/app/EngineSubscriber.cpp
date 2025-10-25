#include "app/EngineSubscriber.hpp"
#include <iostream>

RpmMessage::RpmMessage(int value) : Message("RPM"), value_(value) {}
int RpmMessage::value() const { return value_; }

void EngineSubscriber::onMessage(const MessagePtr& msg) {
    auto rpmMsg = std::dynamic_pointer_cast<RpmMessage>(msg);
    if (rpmMsg)
        std::cout << "[Subscriber] Engine RPM: " << rpmMsg->value() << "\n";
}

std::shared_ptr<EngineSubscriber> makeEngineSubscriber() {
    return std::make_shared<EngineSubscriber>();
}
