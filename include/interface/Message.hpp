#pragma once
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <thread>

class Message
{
public:
    explicit Message(std::string topic, int value)
        : m_topic(std::move(topic)), m_value(value) {}

    virtual ~Message() = default;

    std::string getTopic() const { return m_topic; }
    int value() const { return m_value; }

private:
    std::string m_topic;
    int         m_value;
};

// Concrete Message
class RpmMessage : public Message {
public:
    explicit RpmMessage(int value) : Message("RPM", value), m_value(value) {}
    int value() const { return m_value; }

private:
    int m_value;
};

using MessagePtr = std::shared_ptr<Message>;

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

// Concrete Subscriber
class EngineSubscriber : public Subscriber {
public:
    void onMessage(const MessagePtr& msg) override
    {
        std::cout << "EngineSubscriber received message on topic: "
                  << msg->getTopic() << " with value: " << msg->value() << std::endl;
    }
};

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


int main() {
    auto msgBus    = MessageBus();
    auto engineSub = std::make_shared<EngineSubscriber>();

    msgBus.subscribe("RPM", engineSub);

    for (int i = 0; i < 5; ++i) {
        int rpm = 1000 + rand() % 5000;
        auto msg = std::make_shared<RpmMessage>(rpm);
        msgBus.publish(msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}