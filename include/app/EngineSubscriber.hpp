#pragma once

#include <memory>
#include <iostream>

#include "interface/Message.hpp"
#include "interface/PublisherSubscriber.hpp"

// Concrete Subscriber
class EngineSubscriber : public Subscriber {
public:
    void onMessage(const MessagePtr& msg) override
    {
        std::cout << "Engine ECU [" << msg->getTopic()
            << "] : " << msg->getValue() << std::endl;
    }
};

std::shared_ptr<EngineSubscriber> makeEngineSubscriber() {
    return std::make_shared<EngineSubscriber>();
}
