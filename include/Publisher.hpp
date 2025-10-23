#pragma once
#include "Message.hpp"

class Publisher {
public:
    virtual ~Publisher() = default;
    virtual void publish(const MessagePtr& msg) = 0;
};
