#pragma once

#include <string>

// Pure abstract interface
class Message {
public:
    virtual ~Message() = default;
    virtual std::string getTopic() const = 0;
    virtual std::string getValue() const = 0;
};

// Concrete implementation
class RpmMessage : public Message {
    int m_value;
public:
    explicit RpmMessage(int value) : m_value(value) {}
    std::string getTopic() const override { return "RPM"; }
    std::string getValue() const override { return std::to_string(m_value); }
};

class StatusMessage : public Message {
    std::string m_status;
public:
    explicit StatusMessage(const std::string& status) : m_status(status) {}
    std::string getTopic() const override { return "STATUS"; }
    std::string getValue() const override { return m_status; }
};

using MessagePtr = std::shared_ptr<Message>;
