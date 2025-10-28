#pragma once
#include "interface/Message.hpp"

// Abstract Publisher
class Publisher {
public:
    virtual ~Publisher() = default;
    virtual void publish(const MessagePtr& msg) = 0;
};

// Abstract Subscriber
class Subscriber {
public:
    virtual ~Subscriber() = default;
    virtual void onMessage(const MessagePtr& msg) = 0;
};
