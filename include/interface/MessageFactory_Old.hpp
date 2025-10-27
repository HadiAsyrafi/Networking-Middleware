#pragma once
#include <functional>
#include <unordered_map>
#include <string>
#include "Message.hpp"

/*
 * MessageFactory Class
 *
 * A simple factory for creating Message instances
 * Pattern: Factory
 * Should be instantiated as Singleton
 * 
 */
class MessageFactory {
public:
    using Creator = std::function<MessagePtr()>;

    static MessageFactory& instance() {
        static MessageFactory factory;
        return factory;
    }

    void registerType(const std::string& name, Creator creator) {
        creators_[name] = std::move(creator);
    }

    MessagePtr create(const std::string& name) {
        if (auto it = creators_.find(name); it != creators_.end())
            return it->second();
        return nullptr;
    }

private:
    std::unordered_map<std::string, Creator> creators_;
};
