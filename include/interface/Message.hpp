#pragma once
#include <string>

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

using MessagePtr = std::shared_ptr<Message>;