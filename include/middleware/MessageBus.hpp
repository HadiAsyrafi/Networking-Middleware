#pragma once
#include <map>
#include <memory>
#include <vector>

#include "interface/PublisherSubscriber.hpp"
#include "interface/Message.hpp"

/*
 * MessageBus Class
 *
 * A simple publish-subscribe message bus
 * Pattern: Observer + Singleton
 * Should be instantiated as Singleton
 * 
 */
class MessageBus
{
public:

    void subscribe(const std::string& topic, std::shared_ptr<Subscriber> sub) {
        m_subscribers[topic].push_back(std::move(sub));
    }

    void publish(const MessagePtr& msg) {
        for (auto &sub : m_subscribers[msg->getTopic()])
        {
            sub->onMessage(msg);
        }
    }

private:
    std::map<std::string, std::vector<std::shared_ptr<Subscriber>>> m_subscribers;
};
