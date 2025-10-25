#pragma once
#include "interface/Message.hpp"
#include "interface/PublisherSubscriber.hpp"
#include <memory>

class RpmMessage : public Message {
public:
    explicit RpmMessage(int value);
    int value() const;
private:
    int value_;
};

class EngineSubscriber : public Subscriber {
public:
    void onMessage(const MessagePtr& msg) override;
};

std::shared_ptr<EngineSubscriber> makeEngineSubscriber();
