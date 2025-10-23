#pragma once
#include "Message.hpp"

class Subscriber {
public:
    virtual ~Subscriber() = default;
    virtual void onMessage(const MessagePtr& msg) = 0;
};
