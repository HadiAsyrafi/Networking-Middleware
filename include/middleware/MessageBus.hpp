#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
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
class MessageBus {
public:
    static MessageBus& instance() {
        static MessageBus bus;
        return bus;
    }

    void subscribe(const std::string& topic, std::shared_ptr<Subscriber> sub) {
        std::scoped_lock lock(mutex_);
        subscribers_[topic].push_back(std::move(sub));
    }

    void publish(const MessagePtr& msg) {
        std::scoped_lock lock(mutex_);
        auto it = subscribers_.find(msg->type());
        if (it != subscribers_.end()) {
            for (auto& sub : it->second)
                sub->onMessage(msg);
        }
    }

private:
    std::unordered_map<std::string, std::vector<std::shared_ptr<Subscriber>>> subscribers_;
    std::mutex mutex_;
    MessageBus() = default;
};
