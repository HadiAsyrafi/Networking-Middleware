#pragma once

#include <memory>
#include <iostream>

#include "interface/Message.hpp"
#include "interface/PublisherSubscriber.hpp"

// Concrete Message
class RpmMessage : public Message {
public:
    explicit RpmMessage(int value) : Message("RPM", value), m_value(value) {}
    int value() const { return m_value; }

private:
    int m_value;
};

// Concrete Subscriber
class EngineSubscriber : public Subscriber {
public:
    void onMessage(const MessagePtr& msg) override
    {
        std::cout << "[" << msg->getTopic()
            << "] Engine RPM: " << msg->value() << std::endl;
    }
};

std::shared_ptr<EngineSubscriber> makeEngineSubscriber() {
    return std::make_shared<EngineSubscriber>();
}
